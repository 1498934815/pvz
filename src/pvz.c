/*
 * File    : src/pvz.c
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2018-04-12
 * Module  :
 * License : MIT
 */
#include <stdio.h>
#include <stdlib.h>
#include "../inc/base.h"
#include "../inc/pvz.h"
jmp_buf env;
void *getBase(const char *spec, int findFirst, void (*action)(void *, void *),
              void **end) {
  void *base;
  FILE *maps = fopen("/proc/self/maps", "r");
  BufferType buf;
  while (fgets(buf, BUFSIZE, maps) != NULL) {
    if (strstr(buf, spec)) {
      sscanf(buf, "%p-%p", &base, end);
      if (action != NULL)
        action(base, *end);
      if (findFirst)
        break;
    }
  }
  fclose(maps);
  return base;
}
void *getDynamicBase() {
  void *v;
  return getBase(SPECIFIC_DYNAMIC_LIBRARIES, 1, NULL, (void **)&v);
}
void getBssBase() {
  getBase(SPECIFIC_DYNAMIC_LIBRARIES, 0, NULL, (void **)&baseInfo.bss);
}
void pvz_write(void *lp, void *buf, size_t len) { memcpy(lp, buf, len); }
void pvz_read(void *lp, void *buf, size_t len) { memcpy(buf, lp, len); }
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
  baseInfo.base = NULL;
  baseInfo.task = NULL;
  baseInfo.images = NULL;
}
void doInit() {
  baseInfo.base = getDynamicBase();
  getBssBase();
}
