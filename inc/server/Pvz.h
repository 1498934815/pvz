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
#define PVZ_CORE_LIB "libpvz.so"
#define PVZ_CORE_LIB_HELPER "Java_com_popcap_pvz_1na_PvZActivity_onPVZCreate"
#define OFF_BSS_MEM 0xc5e000
#define OFF_PRIVATE_STACK 0x5138c8
#define OFF_BASE 0x2dfa0

// Game Objects Property
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
#define OFF_LIFE2 0x2c
#define OFF_STORE_GOOD_START 0x350
#define OFF_STORE_GOOD_SLOT 0x384
#define OFF_STORE_GOOD_END 0x3fc
// -- Status
#define OFF_FLAGS_HELPER 0x294
#define OFF_FLAGS 0x6c
#define OFF_MODE 0x838
#define OFF_FREE_PLANTS 0x854
#define OFF_FIELD_TYPE 0x56a8
#define OFF_SUN 0x56bc
#define OFF_PASS_LEVEL 0x5760
// -- Goods
#define OFF_GOOD_PICKUP 0x5c
#define OFF_GOOD_TYPE 0x64
// -- Mowers
#define OFF_MOWER_TRIGGER 0x2c
// -- Base
#define OFF_ZOMBIES_LIST 0x810
// -- Zombies
#define OFF_ZOMBIE_BUTTER_COVER 0xbc

#define PROP_THIEF_CODE 20
#define PROP_LADDER_CODE 21
#define PROP_GARGANTUAR_CODE 23
#define PROP_RED_CODE 32

#define PROP_TRIGGER_MOWER 2
#define PROP_NINE_SLOT 3

#endif // INC_SERVER_PVZ_H
