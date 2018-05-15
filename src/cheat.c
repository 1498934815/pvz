/*
 * File    : src/cheat.c
 * Project : PVZ
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2018-04-12
 * Module  :
 * License : MIT
 */
#ifndef __CHEATER__H
#define __CHEATER__H
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <time.h>
#include "../inc/pvz.h"
#include "../inc/cheat.h"
#include "../inc/pvz_offset.h"
#include "../inc/base.h"

void *by_field(const char *name) { return getField() + getOffset(name); }
void *by_status(const char *name) { return getStatus() + getOffset(name); }
void *by_saves(const char *name) { return getSaves() + getOffset(name); }
#define ROW(lp) (getI32(lp + getOffset("zombies_row")) + 1)
#define COL(lp) (getF32(lp + getOffset("zombies_pos_y")))
#define CODE(lp) (getI32(lp + getOffset("zombies_type")))
void forEachZombies(void (*op)(void *)) {
  size_t zcnt = getI32(by_status("zombies_count"));
  int32_t *entry = getP32(by_status("zombies_entry"));
  void *zp;
  for (size_t idx = 0; idx < zcnt;) {
    // 在僵尸地址前
    // 有一些小的数据
    // 不知道干嘛的
    zp = getP32(entry);
    if (zp > (void *)0x10000000) {
      op(zp);
      idx++;
      // 僵尸地址后面有一个指针
      // 同不知道干嘛的
      entry++;
    }
    entry++;
  }
}

void coverZombies(void *remote) { setI32(remote + 0xbc, 5000); }

void putLadder(void *remote) {
  if (info.task != NULL) {
    if (CODE(remote) == LADDER_CODE) {
      float f = info.task->col * 100;
      int32_t row = info.task->row - 1;
      if (f > getF32(remote + getOffset("zombies_pos_x")))
        return;
      setI32(remote + getOffset("zombies_row"), row);
      setF32(remote + getOffset("zombies_pos_x"), f);
      setF32(remote + getOffset("zombies_pos_y"), f);
      printf("put ladder on %d:%d\n", info.task->row, info.task->col);
      pop(&info.task);
    }
  }
}

void doLimits() {
  uint32_t *zom = by_status("zombies_list");
  // 普僵 红眼 小丑 气球 冰车 舞王 海豚 橄榄
  static uint32_t candidate[] = {0, 0x20, 0x10, 0xf, 0xc, 0x8, 0xe, 0x7};
  static uint32_t which;
  static uint32_t lawnType;
  lawnType = getI32(by_status("lawn_type"));
  srand(time(NULL));
  for (size_t iidx = 0; iidx < 20; ++iidx) {
    for (size_t jidx = 0; jidx < 50; ++jidx) {
      do {
        which = rand() % ARRAY_SIZE(candidate);
        // 如果在非泳池模式得到海豚
        // 重新生成一次
      } while (candidate[which] == 0xe &&
               !IN_RANGE(lawnType, 2, 3)); // 白天泳池/雾夜
      setI32(zom, candidate[which]);
      ++zom;
    }
  }
}

void callLadder() {
  uint32_t *zom = by_status("zombies_list");
  for (size_t idx = 0; idx < 2000; ++idx) {
    setI32(zom, 0x15);
    ++zom;
  }
}
#undef ROW
#undef COL
#undef CODE

#define ROW(lp) (getI32(lp + getOffset("plants_row")) + 1)
#define COL(lp) (getI32(lp + getOffset("plants_col")) + 1)
#define CODE(lp) (getI32(lp + getOffset("plants_type")))
void forEachPlants(void (*op)(void *)) {
  size_t pcnt = getI32(by_status("plants_count"));
  int32_t *entry = getP32(by_status("plants_entry"));
  void *pp;
  for (size_t idx = 0; idx < pcnt;) {
    pp = getP32(entry);
    if (pp > (void *)0x10000000) {
      op(pp);
      entry++;
      idx++;
    }
    entry++;
  }
}

void fuck_LilyPad_Pumpkin(void *remote) {
  if (has(info.task, ROW(remote), COL(remote))) {
    switch (CODE(remote)) {
    case LILYPAD_CODE:
      setI32(remote + getOffset("plants_vis"), 0);
      break;
    case PUMPKIN_CODE:
      setI32(remote + getOffset("plants_hp"), 1332);
    }
  }
}

void freePlants() { setI32(by_field("free_plants"), 1); }
#undef ROW
#undef COL
#undef CODE

void *__getField() {
  void *heap = getP32(info.bss + getOffset("heap"));
  struct pvz_offset *off = __getOffset("field_offset");
  enum {
    NEED_ZERO,
    NEED_DD81,
    NEED_ADDRESS,
  } state = NEED_ZERO;
  int v;
  // [0,0xdd81,base]
#define round 0x30
  for (int i = -round; i <= round; i += sizeof(int32_t)) {
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
  return heap + off->offset;
}
void *getField() {
  static void *field = 0;
  if (field == 0)
    field = __getField();
  return field;
}
void *getStatus() {
  void *status = getP32(by_field("status"));
  return status;
}
#define set_by_val(p) setI32((p), info.val)
void switchMode() { set_by_val(by_field("mode")); }

void setSun() { set_by_val(by_status("sun")); }

void setFlags() {
  set_by_val(getP32(by_status("flags_helper")) + getOffset("flags"));
}

void pass() { setI32(by_status("pass"), 1); }

void *getSaves() { return getP32(by_field("saves_entry")); }

int32_t __getUserId() { return getI32(by_saves("user_id")); }

void moveSaves() {
  // 形如/storage/emulated/0/Android/data/com.popcap.pvz_na/files/userdata/users.dat
  void *helper = by_field("userloc_helper");
  const char *locs = dirname(helper);
  int32_t uid = __getUserId();
  system(to_string("cd %s ; cp game%d_%d.dat game%d_13.dat", locs, uid,
                   info.val, uid));
}
#endif //__CHEATER__H
