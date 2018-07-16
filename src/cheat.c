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
#include <time.h>
#include <pthread.h>
#include "../inc/pvz.h"
#include "../inc/cheat.h"
#include "../inc/pvz_offset.h"

void *by_ptr(void *ptr, const char *name) { return ptr + getOffset(name); }
void *by_field(const char *name) { return by_ptr(getField(), name); }
void *by_status(const char *name) { return by_ptr(getStatus(), name); }
void *by_saves(const char *name) { return by_ptr(getSaves(), name); }

void *__getField() {
  void *heap = getP32(by_ptr(info.bss, "heap"));
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

void *getSaves() { return getP32(by_field("saves_entry")); }

int32_t __getUserId() { return getI32(by_saves("user_id")); }

/* 作弊器功能 */
#define ROW(remote) (getI32(by_ptr(remote, "zombies_row")) + 1)
#define COL(remote) (getF32(by_ptr(remote, "zombies_pos_y")))
#define CODE(remote) (getI32(by_ptr(remote, "zombies_type")))
void forEachZombies(cheat_function callback) {
  size_t zcnt = getI32(by_status("zombies_count"));
  int32_t *entry = getP32(by_status("zombies_entry"));
  void *zp;
  for (size_t idx = 0; idx < zcnt;) {
    // 在僵尸地址前
    // 有一些小的数据
    // 不知道干嘛的
    zp = getP32(entry);
    if (zp > (void *)0x10000000) {
      callback(NULL, zp);
      idx++;
      // 僵尸地址后面有一个指针
      // 同不知道干嘛的
      entry++;
    }
    entry++;
  }
}

pvz_cheat_decl(coverZombies) { setI32(by_ptr(remote, "zombies_butter"), 5000); }

pvz_cheat_decl(putLadder) {
  if (info.task != NULL) {
    if (CODE(remote) == LADDER_CODE) {
      float f = info.task->col * 100;
      int32_t row = info.task->row - 1;
      if (f > getF32(by_ptr(remote, "zombies_pos_x")))
        return;
      setI32(by_ptr(remote, "zombies_row"), row);
      setF32(by_ptr(remote, "zombies_pos_x"), f);
      setF32(by_ptr(remote, "zombies_pos_y"), f);
      // printf("put ladder on %d:%d\n", info.task->row, info.task->col);
      pop(&info.task);
    }
  }
}
int isProper(int code, int fieldType) {
  switch (code) {
  // 泳池模式得到海豚、潜水
  case 0xe:
  case 0xb:
    return IN_RANGE(fieldType, 2, 3);
  // 非屋顶、月夜得到舞王、矿工
  case 0x8:
  case 0x11:
    return !IN_RANGE(fieldType, 4, 5);
  }
  return 1;
}
pvz_cheat_decl(doLimits) {
  uint32_t *zom = by_status("zombies_list");
  // 普僵 铁桶 红眼 小丑 气球 冰车 舞王 海豚 橄榄 篮球 潜水 矿工
  static uint32_t candidate[] = {0,   0x4, 0x20, 0x10, 0xf, 0xc,
                                 0x8, 0xe, 0x7,  0x16, 0xb, 0x11};
  static uint32_t which;
  static uint32_t fieldType;
  fieldType = getI32(by_status("field_type"));
  srand(time(NULL));
  for (size_t iidx = 0; iidx < 20; ++iidx) {
    for (size_t jidx = 0; jidx < 50; ++jidx) {
      do {
        which = rand() % ARRAY_SIZE(candidate);
      } while (!isProper(candidate[which],
                         fieldType)); // 如果得到的僵尸不合适,重新生成
      setI32(zom, candidate[which]);
      ++zom;
    }
  }
}

void put10(int32_t code) {
  uint32_t *zom = by_status("zombies_list");
  int32_t c;
  for (size_t iidx = 0; iidx < 20; ++iidx) {
    c = code;
    for (size_t jidx = 0; jidx < 50; ++jidx) {
      // 每波十只
      if (jidx == 10)
        c = -1;
      setI32(zom, c);
      ++zom;
    }
  }
}

pvz_cheat_decl(callLadder) {
  put10(LADDER_CODE);
}

pvz_cheat_decl(callGargantuar) {
  put10(GARGANTUAR_CODE);
}

#undef ROW
#undef COL
#undef CODE

#define ROW(remote) (getI32(by_ptr(remote, "plants_row")) + 1)
#define COL(remote) (getI32(by_ptr(remote, "plants_col")) + 1)
#define CODE(remote) (getI32(by_ptr(remote, "plants_type")))
void forEachPlants(cheat_function callback) {
  size_t pcnt = getI32(by_status("plants_count"));
  int32_t *entry = getP32(by_status("plants_entry"));
  void *pp;
  for (size_t idx = 0; idx < pcnt;) {
    pp = getP32(entry);
    if (pp > (void *)0x10000000) {
      callback(NULL, pp);
      entry++;
      idx++;
    }
    entry++;
  }
}

pvz_cheat_decl(fuck_LilyPad_Pumpkin) {
  if (has(info.task, ROW(remote), COL(remote))) {
    switch (CODE(remote)) {
    case LILYPAD_CODE:
      setI32(by_ptr(remote, "plants_vis"), 0);
      break;
    case PUMPKIN_CODE:
      setI32(by_ptr(remote, "plants_hp"), 1332);
    }
  }
}

pvz_cheat_decl(freePlants) { setI32(by_field("free_plants"), 1); }
#undef ROW
#undef COL
#undef CODE

#define set_by_val(p) setI32((p), info.val)
pvz_cheat_decl(switchMode) { set_by_val(by_field("mode")); }

pvz_cheat_decl(switchFieldType) { set_by_val(by_status("field_type")); }

pvz_cheat_decl(setSun) { set_by_val(by_status("sun")); }

pvz_cheat_decl(setFlags) {
  // clang-format off
  set_by_val(by_ptr(
        getP32(by_status("flags_helper")),
        "flags"));
  // clang-format on
}

pvz_cheat_decl(pass) { setI32(by_status("pass"), 1); }

pvz_cheat_decl(changeCoins) { set_by_val(by_saves("coins")); }

pvz_cheat_decl(jump) { set_by_val(by_saves("adventure_level")); }

pvz_cheat_decl(pass2life) { setI32(by_saves("2life"), 2); }

pvz_cheat_decl(moveSaves) {
  // 形如/storage/emulated/0/Android/data/com.popcap.pvz_na/files/userdata/users.dat
  void *helper = by_field("userloc_helper");
  const char *locs = dirname(helper);
  int32_t uid = __getUserId();
  system(to_string("cd %s ; cp game%d_%d.dat game%d_13.dat", locs, uid,
                   info.val, uid));
}

pvz_cheat_decl(changeCardCode) {
  void *card = getP32(by_status("cards_entry"));
#define first_card_code 0x74
  set_by_val(card + first_card_code);
#undef first_card_code
}
pthread_t collectTid;
int enableCollect;
#define check_status()                                                         \
  if (getStatus() == NULL)                                                     \
    return;
void __collect() {
  check_status();
  size_t gcnt = getI32(by_status("goods_count"));
  int32_t *entry = getP32(by_status("goods_entry"));
  void *gp;
  for (size_t idx = 0; idx < gcnt;) {
    gp = getP32(entry);
    if (gp > (void *)0x10000000) {
      // 银币/金币/钻石/阳光
      if (IN_RANGE(getI32(by_ptr(gp, "good_type")), 1, 4))
        setI32(by_ptr(gp, "good_collect"), 1);
      idx++;
      entry++;
    }
    entry++;
  }
}
void *__autoCollect(void *__pvz_unused p) {
  while (enableCollect) {
    __collect();
    usleep(WAIT_USECONDS);
  }
  pthread_exit(NULL);
}
pvz_cheat_decl(autoCollect) {
  enableCollect = 1;
  pthread_create(&collectTid, NULL, __autoCollect, NULL);
}
pvz_cheat_decl(cancelAutoCollect) {
  enableCollect = 0;
  if (collectTid) {
    pthread_join(collectTid, NULL);
    collectTid = 0;
  }
}
#undef set_by_val
