/*
 * File    : src/pvz.c
 * Project : PVZ
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2018-04-12
 * Module  :
 * License : MIT
 */
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
// #include <android/log.h>
#include "../inc/base.h"
#include "../inc/pvz.h"
jmp_buf env;
void getDynamicBase() {
  Dl_info dl;
  // 得到进程空间中的SPECIFIC_DYNAMIC_LIBRARIES
  void *handle = dlopen(SPECIFIC_DYNAMIC_LIBRARIES, RTLD_NOLOAD);
  // 获取SPECIFIC_DYNAMIC_LIBRARIES的映射地址
  dladdr(dlsym(handle, LIBPVZ_BASE_HELPER), &dl);

  info.base = dl.dli_fbase;
}
void getBssBase() {
  void *exec = info.base;
  info.bss = (void *)PAGE_END((uint32_t)(exec + LIBPVZ_BSS_OFF));
}
void pvz_write(void *lp, void *buf, size_t len) { memmove(lp, buf, len); }
void pvz_read(void *lp, void *buf, size_t len) { memmove(buf, lp, len); }
int32_t getI32(void *rp) {
  static int32_t val;
  pvz_read(rp, &val, sizeof(val));
  return val;
}
void *getP32(void *rp) {
  static void *val;
  pvz_read(rp, &val, sizeof(uint32_t));
  return val;
}
float getF32(void *rp) {
  static float val;
  pvz_read(rp, &val, sizeof(val));
  return val;
}
void setI32(void *rp, int32_t v) { pvz_write(rp, &v, sizeof(v)); }
void setF32(void *rp, float v) { pvz_write(rp, &v, sizeof(v)); }
void initBase() {
  info.base = NULL;
  info.task = NULL;
}
void doInit() {
  getDynamicBase();
  getBssBase();
}
