/*
 * File    : src/server/Pvz.cc
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-01-28
 * Module  :
 * License : MIT
 */
#include <dlfcn.h>
#include <common/common.h>
#include <server/Pvz.h>
struct PvzOffset offsets[] = {
#include <server/PvzOffset.inc>
};
PvzOffset *__getOffset(const char *name) {
  for (auto &o : offsets) {
    if (strcmp(o.name, name) == 0)
      return &o;
  }
  return nullptr;
}
off_t getOffset(const char *name) {
  return __getOffset(name)->offset;
}
void *__getBase() {
  static void *base = nullptr;
  if (base != nullptr)
    return base;
  void *handle, *mapaddr, *bss, *progdata, *helper;
  struct PvzOffset *off;
  Dl_info dl;
  handle = dlopen(PVZ_CORE_LIB, RTLD_NOW);
  mapaddr = (dladdr(dlsym(handle, PVZ_CORE_LIB_HELPER), &dl), dl.dli_fbase);
  bss = incr(mapaddr, PVZ_CORE_LIB_BSS_MEM_OFF);
  progdata = getPtr(incrFrom(bss, "private_stack"));
  // [-0x20, 0x20]
  off = __getOffset("base_offset");
#define bound 0x20
#define magicNumber "\0\0\0\0\0\0\xdd\x81"
  helper = incr(progdata, off->offset);
  for (int i = -bound; i <= bound; i += POINTERSIZE) {
    if (strncmp(reinterpret_cast<const char *>(incr(helper, i)), magicNumber,
                2 * POINTERSIZE) == 0)
      base = incr(helper, i + 2 * POINTERSIZE);
  }
  if (base == nullptr)
    abort();
  return base;
}
void *__getStatus() {
  return getPtr(incrFrom(__getBase(), "game_status"));
}
void *incr(void *ptr, long off) {
  uintptr_t uiptr = reinterpret_cast<intptr_t>(ptr);
  return reinterpret_cast<void *>(uiptr + off);
}
void *incrFrom(void *ptr, const char *name) {
  return incr(ptr, getOffset(name));
}
void *incrFromBase(const char *name) {
  return incrFrom(__getBase(), name);
}
void *incrFromStatus(const char *name) {
  return incrFrom(__getStatus(), name);
}
