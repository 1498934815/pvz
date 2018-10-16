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
struct pvzinfo *info;
void initDynamicBase(void) {
  Dl_info dl;
  // 得到进程空间中的SPECIFIC_DYNAMIC_LIBRARIES
  void *handle = dlopen(SPECIFIC_DYNAMIC_LIBRARIES, RTLD_NOW);
  // 获取SPECIFIC_DYNAMIC_LIBRARIES的映射地址
  dladdr(dlsym(handle, LIBPVZ_BASE_HELPER), &dl);

  info->base = dl.dli_fbase;
}
void initBssBase(void) {
  void *exec = info->base;
  info->bss = (void *)PAGE_END((uint32_t)(exec + LIBPVZ_BSS_OFF));
}

struct pvz_offset pvz_off_tbl[] = {
    // 僵尸类型代码
    {"zombies_type", 0x30},
    // 僵尸x坐标
    // 相对于格子的坐标
    {"zombies_pos_x", 0x38},
    // 僵尸y坐标
    {"zombies_pos_y", 0x3c},
    // 僵尸的行
    {"zombies_row", 0x1c},
    // 僵尸血量
    {"zombies_hp", 0xd4},
    // 僵尸黄油
    {"zombies_butter", 0xbc},
    // 植物是否可见
    {"plants_vis", 0x18},
    // 植物的列
    {"plants_col", 0x34},
    // 植物的行
    {"plants_row", 0x1c},
    // 植物类型代码
    {"plants_type", 0x30},
    // 植物血量
    {"plants_hp", 0x4c},
    // 物品
    {"goods_collect", 0x5c},
    {"goods_type", 0x64},
    // 推车
    {"mowers_trigger", 0x2c},
    // bss + 0x...
    {"heap", 0x5138c8},
    // *(*heap + field_offset)
    {"field_offset", 0x2dfa0},
    // field + 0x...
    {"status", 0x7c8},
    {"saves_entry", 0x868},
    {"free_plants", 0x854},
    {"mode", 0x838},
    // status + ...
    {"zombies_entry", 0xb4},
    {"zombies_count", 0xc4},
    {"plants_entry", 0xd0},
    {"plants_count", 0xe0},
    {"goods_entry", 0x108},
    {"goods_count", 0x118},
    {"mowers_entry", 0x124},
    {"mowers_count", 0x134},
    {"zombies_list", 0x810},
    {"field_type", 0x56a8},
    {"sun", 0x56bc},
    {"pass", 0x5760},
    {"flags_helper", 0x294},
    {"cards_entry", 0x168},
    // *flags_helper + 0x6c
    {"flags", 0x6c},
    // *saves_entry + ...
    {"coins", 0x4},
    {"user_id", 0x20},
    {"adventure_level", 0x28},
    {"2life", 0x2c},
    // end
    {NULL, 0},
};
struct pvz_offset *__getOffset(const char *name) {
#define for_each_term(off)                                                     \
  for (struct pvz_offset *off = pvz_off_tbl; off->name != NULL; ++off)
  for_each_term(off) {
    if (strcmp(name, off->name) == 0)
      return off;
  }
  return NULL;
}
off_t getOffset(const char *name) {
  return __getOffset(name)->offset;
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
  static struct pvzinfo gCtx;
  info = &gCtx;
  memset(info, 0, sizeof(struct pvzinfo));
}
void doInit(void) {
  initBase();
  initDynamicBase();
  initBssBase();
  // 设置随机数种子
  srand(time(NULL));
}
