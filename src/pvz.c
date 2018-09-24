/*
 * File    : src/pvz.c
 * Project : PVZ
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2018-04-12
 * Module  :
 * License : MIT
 */
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include "../inc/pvz.h"
jmp_buf env;
void initDynamicBase(void) {
  Dl_info dl;
  // 得到进程空间中的SPECIFIC_DYNAMIC_LIBRARIES
  void *handle = dlopen(SPECIFIC_DYNAMIC_LIBRARIES, RTLD_NOW);
  // 获取SPECIFIC_DYNAMIC_LIBRARIES的映射地址
  dladdr(dlsym(handle, LIBPVZ_BASE_HELPER), &dl);

  info.base = dl.dli_fbase;
}
void initBssBase(void) {
  void *exec = info.base;
  info.bss = (void *)PAGE_END((uint32_t)(exec + LIBPVZ_BSS_OFF));
}
void pvz_write(void *lp, void *buf, size_t len) {
  memmove(lp, buf, len);
}
void pvz_read(void *lp, void *buf, size_t len) {
  memmove(buf, lp, len);
}
#define IMPL_GET(type, name)                                                   \
  DEFINE_GET(type, name) {                                                     \
    static type val;                                                           \
    pvz_read(remote, &val, sizeof(val));                                       \
    return val;                                                                \
  }
#define IMPL_SET(type, name)                                                   \
  DEFINE_SET(type, name) {                                                     \
    pvz_write(remote, &val, sizeof(val));                                      \
  }
IMPL_GET(int32_t, I32);
IMPL_GET(float, F32);
IMPL_GET(void *, P32);
IMPL_SET(int32_t, I32);
IMPL_SET(float, F32);

void initBase(void) {
  info.base = NULL;
  info.bss = NULL;
  info.task = NULL;
  info.pid = 0;
}
void doInit(void) {
  initBase();
  initDynamicBase();
  initBssBase();
  // 设置随机数种子
  srand(time(NULL));
}
