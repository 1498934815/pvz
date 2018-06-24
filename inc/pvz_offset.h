/*
 * File    : pvz_offset.h
 * Project : PVZ
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2017-08-15
 * Module  :
 * License : MIT
 */
#ifndef __PVZ_OFFSET__H
#define __PVZ_OFFSET__H
#include <string.h>
#include <sys/cdefs.h>
struct pvz_offset {
  const char *name;
  off_t offset;
};
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
    // 植物攻击力
    {"plants_attack", 0x54},
    // 物品
    {"good_collect", 0x5c},
    // bss + 0x...
    {"heap", 0x5138c8},
    {"userloc_helper", 0x7a9fcc},
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
    {"zombies_list", 0x810},
    {"plants_entry", 0xd0},
    {"plants_count", 0xe0},
    {"goods_entry", 0x108},
    {"goods_count", 0x118},
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
};
struct pvz_offset *__getOffset(const char *name) {
  struct pvz_offset *pf;
  for (size_t i = 0; i < sizeof(pvz_off_tbl) / sizeof(pvz_off_tbl[0]); ++i) {
    pf = &pvz_off_tbl[i];
    if (strcmp(name, pf->name) == 0) {
      break;
    }
  }
  return pf;
}
off_t getOffset(const char *name) { return __getOffset(name)->offset; }
#endif //__PVZ_OFFSET__H
