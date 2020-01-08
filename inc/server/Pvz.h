/*
 * File    : inc/server/Pvz.h
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-01-25
 * Module  :
 * License : MIT
 */
#ifndef INC_SERVER_PVZ_H
#define INC_SERVER_PVZ_H
#include "common/PvzCommon.h"
#include <stdlib.h>

inline void __writeMem(void *ptr, void *src, size_t size) {
  memcpy(ptr, src, size);
}
template <typename Ty> void writeMem(void *ptr, Ty value) {
  __writeMem(ptr, reinterpret_cast<void *>(&value), sizeof(value));
}
template <typename Ty> Ty readMem(void *ptr, Ty &value) {
  __writeMem(reinterpret_cast<void *>(&value), ptr, sizeof(value));
  return value;
}
void *__getBase();
void *__getStatus();
void *__getSaves();
bool __isGaming();
void *incr(void *, off_t);
void *incrBase(off_t);
void *incrStatus(off_t);
void *incrSaves(off_t);
#define implGet(name, type)                                                    \
  inline type get##name(void *ptr, type &value) {                              \
    return readMem<type>(ptr, value);                                          \
  }                                                                            \
  inline type get##name(void *ptr) {                                           \
    static type buf;                                                           \
    return get##name(ptr, buf);                                                \
  }
#define implSet(name, type)                                                    \
  inline void set##name(void *ptr, type value) {                               \
    writeMem<type>(ptr, value);                                                \
  }
#define implGetSet(name, type) implGet(name, type) implSet(name, type)
implGetSet(I32, int32_t);
implGetSet(U32, uint32_t);
implGetSet(F32, float);
implGetSet(Ptr, void *);
#undef implGet
#undef implSet
#undef implGetset

class Communicator;
void eachObject(Communicator *, off_t, off_t, object_callback);
#define eachPlant(com, callback)                                               \
  eachObject(com, OFF_PLANTS_ENTRY, OFF_PLANTS_COUNT, callback)
#define eachZombie(com, callback)                                              \
  eachObject(com, OFF_ZOMBIES_ENTRY, OFF_ZOMBIES_COUNT, callback)
#define eachGood(com, callback)                                                \
  eachObject(com, OFF_GOODS_ENTRY, OFF_GOODS_COUNT, callback)
#define eachMower(com, callback)                                               \
  eachObject(com, OFF_MOWERS_ENTRY, OFF_MOWERS_COUNT, callback)

// Game Private Property
#define PROP_PVZ_CORE_LIB "libpvz.so"
#define OFF_BASE 0xceb8cc

// Game Objects Properties
#define OFF_GAME_STATUS 0x7c8
#define OFF_SAVES_ENTRY 0x868
#define OFF_PLANTS_ENTRY 0xd0
#define OFF_PLANTS_COUNT 0xe0
#define OFF_ZOMBIES_ENTRY 0xb4
#define OFF_ZOMBIES_COUNT 0xc4
#define OFF_GOODS_ENTRY 0x108
#define OFF_GOODS_COUNT 0x118
#define OFF_MOWERS_ENTRY 0x124
#define OFF_MOWERS_COUNT 0x134

// Some Misc Things
// -- Saves
#define OFF_COIN 0x4
#define OFF_ADVENTURE_LEVEL 0x28
#define OFF_LIFE2 0x2c
#define OFF_STORE_GOOD_START 0x350
#define OFF_STORE_GOOD_SLOT 0x384
#define OFF_STORE_GOOD_END 0x3fc
// -- Base
#define OFF_MODE 0x838
#define OFF_FREE_PLANTS 0x854
#define OFF_FIELD_TYPE 0x56a8
// -- Status
#define OFF_CARDS_ENTRY 0x168
#define OFF_FLAGS_HELPER 0x294
#define OFF_FLAGS 0x6c
#define OFF_SUN 0x56bc
#define OFF_PASS_LEVEL 0x5760
#define OFF_CURRENT_WAVE 0x56d8
#define OFF_FRESH_COUNTDOWN_TOTAL 0x56fc
#define OFF_FRESH_COUNTDOWN_REMAIN 0x56f8
#define OFF_WAVE_HITPOINT_TOTAL 0x56f4
#define OFF_WAVE_HITPOINT_BOUNDARY 0x56f0
// -- Goods
#define OFF_GOOD_PICKUP 0x5c
#define OFF_GOOD_TYPE 0x64
// -- Mowers
#define OFF_MOWER_TRIGGER 0x2c
// -- Base
#define OFF_ZOMBIES_LIST 0x810
// -- Zombies
#define OFF_ZOMBIE_CODE 0x30
#define OFF_ZOMBIE_HP 0xd4
#define OFF_ZOMBIE_ROW 0x1c
#define OFF_ZOMBIE_POS_X 0x38
#define OFF_ZOMBIE_POS_Y 0x3c
#define OFF_ZOMBIE_BUTTER_COVER 0xbc
// -- Plants
#define OFF_PLANT_CODE 0x30
#define OFF_PLANT_HP 0x4c
#define OFF_PLANT_X 0x8
#define OFF_PLANT_Y 0xc

// -- Properties
#define PROP_THIEF_CODE 20
#define PROP_LADDER_CODE 21
#define PROP_GARGANTUAR_CODE 23
#define PROP_RED_CODE 32

#define PROP_TRIGGER_MOWER 2
#define PROP_NINETH_SLOT 3

#define PROP_FIRST_CARD_ENTRY 0x34
#define PROP_CARD_COUNT 0x30
#define PROP_CARD_SEED 0x40
#define PROP_CARD_OFFSET 0x5c

#endif // INC_SERVER_PVZ_H
