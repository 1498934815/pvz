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

#define ROW(lp) (getI32(lp + getOffset("zombies_row")) + 1)
#define COL(lp) (getF32(lp + getOffset("zombies_pos_y")))
#define CODE(lp) (getI32(lp + getOffset("zombies_type")))
void forEachZombies(void (*op)(void *)) {
  size_t zcnt = getI32(getStatus() + getOffset("zombies_count"));
  int32_t *entry = getP32(getStatus() + getOffset("zombies_entry"));
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
  uint32_t *zom = getStatus() + getOffset("zombies_list");
  // 普僵 红眼 小丑 气球 冰车 舞王 海豚 橄榄
  static uint32_t candidate[] = {0, 0x20, 0x10, 0xf, 0xc, 0x8, 0xe, 0x7};
  static uint32_t which;
  static uint32_t mode;
  mode = getI32(getStatus() + getOffset("mode"));
  srand(time(NULL));
  for (size_t iidx = 0; iidx < 20; ++iidx) {
    for (size_t jidx = 0; jidx < 50; ++jidx) {
      do {
        which = rand() % 8;
        // 如果在非泳池模式得到海豚
        // 重新生成一次
      } while (which == 6 && !IN_RANGE(mode, 13, 14));
      setI32(zom, candidate[which]);
      ++zom;
    }
  }
}

void callLadder() {
  uint32_t *zom = getStatus() + getOffset("zombies_list");
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
  size_t pcnt = getI32(getStatus() + getOffset("plants_count"));
  int32_t *entry = getP32(getStatus() + getOffset("plants_entry"));
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

void freePlants() { setI32(getField() + getOffset("free_plants"), 1); }
#undef ROW
#undef COL
#undef CODE

void *getField() {
  void *helper = getP32(info.bss + getOffset("base"));
  return getOffset("field") + helper;
}
void *getStatus() {
  void *status = getP32(getField() + getOffset("status"));
  return status;
}
void switchMode() { setI32(getField() + getOffset("mode"), info.val); }
void setSun() { setI32(getStatus() + getOffset("sun"), info.val); }
void pass() { setI32(getStatus() + getOffset("pass"), 1); }
void setFlags() {
  setI32(getP32(getStatus() + getOffset("flags_helper")) + getOffset("flags"),
         info.val);
}
void moveSaves() {
  // 形如/storage/emulated/0/Android/data/com.popcap.pvz_na/files/userdata/users.dat
  void *helper = getField() + getOffset("saves_helper");
  const char *locs = dirname(helper);
  system(to_string("cd %s ; cp game1_%d.dat game1_13.dat", locs, info.val));
}
#endif //__CHEATER__H
