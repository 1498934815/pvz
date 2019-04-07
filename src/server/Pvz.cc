/*
 * File    : src/server/Pvz.cc
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-01-28
 * Module  :
 * License : MIT
 */
#include "server/Pvz.h"
#include "common/PvzCommon.h"
#include "common/common.h"
#include <dlfcn.h>
void *__getBase() {
  static void *base = nullptr;
  // Just detect once
  if (base != nullptr)
    return base;
  void *handle, *mapaddr, *bss, *helper;
  Dl_info dl;
  handle = dlopen(PVZ_CORE_LIB, RTLD_NOW);
  mapaddr = (dladdr(dlsym(handle, PVZ_CORE_LIB_HELPER), &dl), dl.dli_fbase);
  bss = incr(mapaddr, OFF_BSS_MEM);
  helper = incr(getPtr(incr(bss, OFF_PRIVATE_STACK)), OFF_BASE);
  // [-0x20, 0x20]
#define bound 0x20
#define magicNumber "\0\0\0\0\0\0\xdd\x81"
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
  return getPtr(incrBase(OFF_GAME_STATUS));
}
void *__getSaves() {
  return getPtr(incrBase(OFF_SAVES_ENTRY));
}
bool __isGaming() {
  return __getStatus() != nullptr;
}
void *incr(void *ptr, off_t off) {
  uintptr_t uiptr = reinterpret_cast<intptr_t>(ptr);
  return reinterpret_cast<void *>(uiptr + off);
}
void *incrBase(off_t off) {
  return incr(__getBase(), off);
}
void *incrStatus(off_t off) {
  return incr(__getStatus(), off);
}
void *incrSaves(off_t off) {
  return incr(__getSaves(), off);
}

void eachObject(Communicator *com, off_t entry_off, off_t count_off,
                object_callback callback) {
  size_t cnt = getU32(incrStatus(count_off));
  int32_t *entry = reinterpret_cast<int32_t *>(getPtr(incrStatus(entry_off)));
  void *rp;
  while (cnt != 0) {
    // 有一些小的数据
    // 不知道干嘛的
    rp = getPtr(entry);
    if (rp > (void *)0x10000000) {
      callback(com, rp);
      --cnt;
      ++entry;
    }
    ++entry;
  }
}
