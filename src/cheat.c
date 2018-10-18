/*
 * File    : src/cheat.c
 * Project : PVZ
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2018-04-12
 * Module  :
 * License : MIT
 */

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <stdbool.h>
#include <pthread.h>
#include "../inc/pvz.h"
#include "../inc/cheat.h"

void *fromPtr(void *ptr, const char *name) {
  return ptr + getOffset(name);
}
void *__getField(void) {
  void *heap = getP32(fromPtr(info->bss, "heap"));
  struct pvz_offset *off = __getOffset("field_offset");
  enum {
    NEED_ZERO,
    NEED_DD81,
    NEED_ADDRESS,
  } state = NEED_ZERO;
  int v;
  // [0,0xdd81,base]
#define bound 0x30
  for (int i = -bound; i <= bound; i += sizeof(int32_t)) {
    v = getI32(heap + off->offset + i);
    if (v == 0) {
      state = NEED_DD81;
    } else if (v == 0xdd81 && state == NEED_DD81) {
      state = NEED_ADDRESS;
    } else if (state == NEED_ADDRESS) {
      off->offset += i;
      break;
    } else {
      state = NEED_ZERO;
    }
  }
#undef bound
  return heap + off->offset;
}

void *getField(void) {
  static void *field = NULL;
  if (field == NULL)
    field = __getField();
  return field;
}

void *getStatus(void) {
  void *status = getP32(fromField("status"));
  return status;
}

void *getSaves(void) {
  return getP32(fromField("saves_entry"));
}

void forEachValue(cheat_function callback, const char *count_name,
                  const char *entry_name) {
  size_t cnt = getU32(fromStatus(count_name));
  int32_t *entry = getP32(fromStatus(entry_name));
  void *rp;
  for (size_t idx = 0; idx < cnt;) {
    // 有一些小的数据
    // 不知道干嘛的
    rp = getP32(entry);
    if (rp > (void *)0x10000000) {
      callback(rp);
      ++idx;
      // 地址后面有一个指针
      // 同不知道干嘛的
      ++entry;
    }
    ++entry;
  }
}

/* 作弊器功能 */
#define ROW(remote) (getU32(fromPtr(remote, "zombies_row")) + 1)
#define COL(remote) (getF32(fromPtr(remote, "zombies_pos_y")))
#define CODE(remote) (getU32(fromPtr(remote, "zombies_type")))
void forEachZombies(cheat_function callback) {
  forEachValue(callback, "zombies_count", "zombies_entry");
}

pvz_cheat_decl(coverZombies) {
  setI32(fromPtr(remote, "zombies_butter"), 5000);
}

pvz_cheat_decl(putLadder) {
  if (info->task != NULL) {
    if (CODE(remote) == LADDER_CODE) {
      float f = info->task->col * 100;
      int32_t row = info->task->row - 1;
      if (f > getF32(fromPtr(remote, "zombies_pos_x")))
        return;
      setI32(fromPtr(remote, "zombies_row"), row);
      setF32(fromPtr(remote, "zombies_pos_x"), f);
      setF32(fromPtr(remote, "zombies_pos_y"), f);
      // printf("put ladder on %d:%d\n", info->task->row, info->task->col);
      pop(&info->task);
    }
  }
}
bool isProper(int code, enum pvz_field fieldType) {
  switch (code) {
  // 泳池模式得到海豚、潜水
  case 0xe:
  case 0xb:
    return IN_RANGE(fieldType, POOL, FOG);
  // 非屋顶、月夜得到舞王、矿工
  case 0x8:
  case 0x11:
    return !IN_RANGE(fieldType, ROOF, MOONNIGHT);
  }
  return true;
}
uint32_t generateCandidate(uint32_t seeds[], size_t seedSize, size_t i,
                           enum pvz_field fieldType) {
  uint32_t which;
  do {
    which = rand() % seedSize;
  } while (!isProper(seeds[which],
                     fieldType)); // 如果得到的僵尸不合适,重新生成
  return seeds[which];
}
void replaceSeed(uint32_t seeds[], size_t seedSize, size_t end, bool hasThief,
                 bool hasRed) {
  uint32_t *zom = fromStatus("zombies_list");
  enum pvz_field fieldType = (enum pvz_field)getU32(fromStatus("field_type"));
  for (size_t wave = 0; wave < 20; ++wave) {
    for (size_t i = 0; i < 50; ++i) {
      // 如果尾数是9
      if (i >= end)
        setI32(zom, -1);
      else if (hasThief && (wave % 10 == 9) && i < 3)
        setI32(zom, THIEF_CODE);
      else if (hasRed && i >= 40)
        setI32(zom, RED_CODE);
      else
        setI32(zom, generateCandidate(seeds, seedSize, i, fieldType));
      ++zom;
    }
  }
}
pvz_cheat_decl(doLimits) {
  // 普僵 铁桶 小丑 气球 冰车 舞王 海豚 橄榄 篮球 潜水 矿工 跳跳 撑杆
  // 白眼 红眼 梯子
  static uint32_t seeds[] = {
      0,        0x4,        0x10, 0xf,  0xc,  0x8, 0xe,
      0x7,      0x16,       0xb,  0x11, 0x12, 0x3, GARGANTUAR_CODE,
      RED_CODE, LADDER_CODE};
  replaceSeed(seeds, ARRAY_SIZE(seeds), 50, true, true);
}

pvz_cheat_decl(callLadder) {
  static uint32_t seeds[] = {LADDER_CODE};
  replaceSeed(seeds, ARRAY_SIZE(seeds), 10, false, false);
}

pvz_cheat_decl(callGargantuar) {
  static uint32_t seeds[] = {GARGANTUAR_CODE};
  replaceSeed(seeds, ARRAY_SIZE(seeds), 10, false, false);
}

#undef ROW
#undef COL
#undef CODE

#define ROW(remote) (getU32(fromPtr(remote, "plants_row")) + 1)
#define COL(remote) (getU32(fromPtr(remote, "plants_col")) + 1)
#define CODE(remote) (getU32(fromPtr(remote, "plants_type")))
void forEachPlants(cheat_function callback) {
  forEachValue(callback, "plants_count", "plants_entry");
}

pvz_cheat_decl(fuck_LilyPad_Pumpkin) {
  if (has(info->task, ROW(remote), COL(remote))) {
    switch (CODE(remote)) {
    case LILYPAD_CODE:
      setI32(fromPtr(remote, "plants_vis"), 0);
      break;
    case PUMPKIN_CODE:
      setI32(fromPtr(remote, "plants_hp"), 1332);
    }
  }
}

pvz_cheat_decl(freePlants) {
  setI32(fromField("free_plants"), 1);
}
pvz_cheat_decl(shutdownFreePlants) {
  setI32(fromField("free_plants"), 0);
}
#undef ROW
#undef COL
#undef CODE

#define set_from_val(p) setI32((p), info->val)
pvz_cheat_decl(switchMode) {
  set_from_val(fromField("mode"));
}

pvz_cheat_decl(switchFieldType) {
  set_from_val(fromStatus("field_type"));
}

pvz_cheat_decl(setSun) {
  set_from_val(fromStatus("sun"));
}

pvz_cheat_decl(setFlags) {
  // clang-format off
  set_from_val(fromPtr(
        getP32(fromStatus("flags_helper")),
        "flags"));
  // clang-format on
}

pvz_cheat_decl(pass) {
  setI32(fromStatus("pass"), 1);
}

pvz_cheat_decl(changeCoins) {
  set_from_val(fromSaves("coins"));
}

pvz_cheat_decl(jump) {
  set_from_val(fromSaves("adventure_level"));
}

pvz_cheat_decl(pass2life) {
  setI32(fromSaves("2life"), 2);
}

pvz_cheat_decl(changeCardCode) {
  void *card = getP32(fromStatus("cards_entry"));
#define first_card_code 0x74
  set_from_val(card + first_card_code);
#undef first_card_code
}
pvz_cheat_decl(__collect_callback) {
  if (IN_RANGE(getU32(fromPtr(remote, "goods_type")), 1, 4))
    setI32(fromPtr(remote, "goods_collect"), 1);
}

pvz_cheat_decl(autoCollect) {
  forEachValue(__collect_callback, "goods_count", "goods_entry");
  usleep(WAIT_USECONDS);
}
pvz_cheat_decl(__triggerMowers_callback) {
  // 表示触发推车
  setI32(fromPtr(remote, "mowers_trigger"), 2);
}
pvz_cheat_decl(triggerMowers) {
  forEachValue(__triggerMowers_callback, "mowers_count", "mowers_entry");
}
#undef set_from_val
