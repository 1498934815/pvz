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
#include <time.h>
#include "../inc/pvz.h"
#include "../inc/cheat.h"
#include "../inc/pvz_offset.h"
#include "../inc/base.h"

void letZombiesFragile(void *rp) {
  struct Hp hp = {
      .curHp = 10,
      .totalHp = 10,
      .armor = 0,
  };
  pvz_write(rp + ZOM_HP_OFF, &hp, sizeof(hp));
}
void coverZombies(void *rp) { setI32(rp + 0xbc, 5000); }
void increaseZombies(void *rp) {
  setI32(rp + ZOM_HP_OFF, getI32(rp + ZOM_HP_OFF) * 2);
}
void increaseCabbagePult() {
  char *p = baseInfo.base + getOffset("cabbage");
  setI32(p + 8, 45);
}

void *getField() {
  void *helper = getP32(baseInfo.bss + getOffset("base"));
  return getOffset("field") + helper;
}
void *getStatus() {
  void *status = getP32(getField() + getOffset("status"));
  return status;
}
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
#define ROW(lp) (getI32(lp + getOffset("zombies_row")) + 1)
#define COL(lp) (getF32(lp + getOffset("zombies_pos_y")))
#define HP(lp) (getI32(lp + ZOM_HP_OFF))
#define CODE(lp) (getI32(lp + getOffset("zombies_type")))
void reportZombies(void *rp) {
  printf("Found at %p(row@%d x pos_y@%f)(hp:%d code:%d)\n", rp, ROW(rp),
         COL(rp), HP(rp), CODE(rp));
}
#undef ROW
#undef COL
#undef HP
#undef CODE
void increasePlants(void *remote) {
  setI32(remote + PLAN_HP_OFF, getI32(remote + PLAN_HP_OFF) * 2);
}
void increasePlantsAttack(void *remote) {
  setI32(remote + PLAN_ATT_TOTAL_OFF, getI32(remote + PLAN_ATT_TOTAL_OFF) / 2);
}
void putLadder(void *remote) {

  if (baseInfo.task != NULL) {
    int32_t type = getI32(remote + getOffset("zombies_type"));
    if (type == 21) {
      float f = baseInfo.task->col * 100;
      int32_t row = baseInfo.task->row - 1;
      if (f > getF32(remote + getOffset("zombies_pos_x")))
        return;
      setI32(remote + getOffset("zombies_row"), row);
      setF32(remote + getOffset("zombies_pos_x"), f);
      setF32(remote + getOffset("zombies_pos_y"), f);
      printf("put ladder on %d:%d\n", baseInfo.task->row, baseInfo.task->col);
      pop(&baseInfo.task);
    }
  }
}
#define ROW(lp) (getI32(lp + getOffset("plants_row")) + 1)
#define COL(lp) (getI32(lp + getOffset("plants_col")) + 1)
#define HP(lp) (getI32(lp + PLAN_HP_OFF))
#define CODE(lp) (getI32(lp + getOffset("plants_type")))
#define ATTACK(lp) (getI32(lp + getOffset("plants_attack")))
void reportPlants(void *remote) {
  printf("Found at %p (row@%d x col@%d)(hp:%d code:%d)\n", remote, ROW(remote),
         COL(remote), HP(remote), CODE(remote));
}
void fuck_LilyPad_Pumpkin(void *remote) {
  if (has(baseInfo.task, ROW(remote), COL(remote))) {
    switch (CODE(remote)) {
    case LILYPAD_CODE:
      setI32(remote + getOffset("plants_vis"), 0);
      break;
    case PUMPKIN_CODE:
      setI32(remote + getOffset("plants_hp"), 1332);
    }
  }
}
void plants_freeze(void *remote) {
  if (ATTACK(remote) == 0)
    return;
  insert_images(&baseInfo.images, ATTACK(remote),
                remote + getOffset("plants_attack"));
  setI32(remote + getOffset("plants_attack"), 0);
}
void plants_attack(void *remote) {
  recover_images(baseInfo.images);
  destroy((__list **)&baseInfo.images, NULL);
}
void setSun() { setI32(getStatus() + getOffset("sun"), baseInfo.val); }
void pass() { setI32(getStatus() + getOffset("pass"), 1); }
void setFlags() {
  setI32(getP32(getStatus() + getOffset("flags_helper")) + getOffset("flags"),
         baseInfo.val);
}
void doLimits() {
  uint32_t *zom = getStatus() + getOffset("zombies_list");
  // 普僵 红眼 小丑 气球 冰车 舞王 海豚 橄榄
  static uint32_t candidate[] = {0, 0x20, 0x10, 0xf, 0xc, 0x8, 0xe, 0x7};
  static uint32_t which;
  srand(time(NULL));
  for (size_t iidx = 0; iidx < 20; ++iidx) {
    for (size_t jidx = 0; jidx < 50; ++jidx) {
      do {
        which = rand() % 7;
        // 如果在非泳池模式得到海豚
        // 重新生成一次
      } while(which == 6 && getI32(getStatus() + getOffset("mode")) != 13);
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
void freePlants() { setI32(getField() + getOffset("free_plants"), 1); }
void switchMode() { setI32(getField() + getOffset("mode"), baseInfo.val); }
#undef ROW
#undef COL
#undef HP
#undef CODE
#undef ATTACK
#endif //__CHEATER__H
