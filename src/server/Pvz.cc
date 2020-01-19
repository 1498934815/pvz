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
#include <libgen.h>
#include <link.h>
#include <sys/mman.h>
extern "C" int dl_iterate_phdr(int (*)(struct dl_phdr_info *, size_t, void *),
                               void *);
int __detect_corelib_address(struct dl_phdr_info *info, size_t, void *corelib) {
  if (strcmp(basename(info->dlpi_name), PROP_PVZ_CORE_LIB) == 0) {
    *static_cast<void **>(corelib) = reinterpret_cast<void *>(info->dlpi_addr);
    return 1;
  }
  return 0;
}
static void *__corelib;
void *__getBase() {
  static void *base = nullptr;
  // Just detect once
  if (base != nullptr)
    return base;
  dl_iterate_phdr(__detect_corelib_address, &__corelib);
  // make it readable/writeable
  mprotect(__corelib, PROP_PVZ_CORE_LIB_LENGTH,
           PROT_READ | PROT_WRITE | PROT_EXEC);
  base = getPtr(incr(__corelib, OFF_BASE));
  return base;
}
void *__getCoreLib() {
  return __corelib;
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
