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
#include <initializer_list>
#include <iterator>
#include <stdlib.h>
void *__getCoreLib();
void *__getBase();
void *__getStatus();
void *__getSaves();
bool __isGaming();
void *incr(void *, off_t);
void *incrBase(off_t);
void *incrStatus(off_t);
void *incrSaves(off_t);

inline void __writeMem(void *ptr, void *src, size_t size) {
  memcpy(ptr, src, size);
}
template <typename... Args> inline void *__calculate(void *ptr, Args... args) {
  const auto &offs = {args...};
  auto it = offs.begin();
  for (; std::next(it) != offs.end(); ++it)
    __writeMem(&ptr, incr(ptr, *it), sizeof(ptr));
  return incr(ptr, *it);
}
inline void *__calculate(void *ptr) {
  return ptr;
}
template <typename Ty, typename... Args>
inline void writeMem(const Ty &value, void *ptr, Args... args) {
  __writeMem(__calculate(ptr, args...), (void *)&value, sizeof(value));
}
template <typename Ty, typename... Args>
inline Ty &readMem(Ty &value, void *ptr, Args... args) {
  __writeMem((void *)&value, __calculate(ptr, args...), sizeof(value));
  return value;
}
#define implGet(name, type)                                                    \
  template <typename... Args> inline type get##name(void *ptr, Args... args) { \
    static type buf;                                                           \
    return readMem<type, Args...>(buf, ptr, args...);                          \
  }
#define implSet(name, type)                                                    \
  template <typename... Args>                                                  \
  inline void set##name(const type &value, void *ptr, Args... args) {          \
    writeMem<type, Args...>(value, ptr, args...);                              \
  }
#define implGetSet(name, type) implGet(name, type) implSet(name, type)
implGetSet(Byte, unsigned char);
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
#define eachItem(com, callback)                                                \
  eachObject(com, OFF_ITEMS_ENTRY, OFF_ITEMS_COUNT, callback)
#define eachMower(com, callback)                                               \
  eachObject(com, OFF_MOWERS_ENTRY, OFF_MOWERS_COUNT, callback)

// Game Private Property
#define PROP_PVZ_CORE_LIB "libpvz.so"
#define PROP_PVZ_CORE_LIB_LENGTH 0xc17000
#define OFF_BASE 0xceb8cc

#define OFF_CHOMPER_TIME 0x23bf98
#define OFF_PLANTS_SLOT_INFO 0xc43fcc
#define OFF_PLANTS_HIT 0xc45630
#define OFF_GAMEPACK1_GAMES 0xc4b284
#define OFF_GAMEPACK2_GAMES 0xc4b314

// Game Objects Properties
#define OFF_GAME_STATUS 0x7c8
#define OFF_SAVES_ENTRY 0x868
#define OFF_PLANTS_ENTRY 0xd0
#define OFF_PLANTS_COUNT 0xe0
#define OFF_ZOMBIES_ENTRY 0xb4
#define OFF_ZOMBIES_COUNT 0xc4
#define OFF_ITEMS_ENTRY 0x108
#define OFF_ITEMS_COUNT 0x118
#define OFF_MOWERS_ENTRY 0x124
#define OFF_MOWERS_COUNT 0x134
#define OFF_PROJECTILES_ENTRY 0xec
#define OFF_PROJECTILES_COUNT 0xfc

// Some Misc Things
// -- Saves
#define OFF_COIN 0x4
#define OFF_ADVENTURE_LEVEL 0x28
#define OFF_LIFE2 0x2c
#define OFF_STORE_ITEM_START 0x350
#define OFF_STORE_ITEM_SLOT 0x384
#define OFF_STORE_ITEM_END 0x3fc
// -- Base
#define OFF_MODE 0x838
#define OFF_FREE_PLANTS 0x854
// -- Status
#define OFF_CARDS_ENTRY 0x168
#define OFF_BATTLEGROUND_GRID_TYPE 0x2c4
#define OFF_FLAGS_HELPER 0x294
#define OFF_FLAGS 0x6c
#define OFF_ROW_ZOMBIESTYPE 0x734
#define OFF_ZOMBIES_LIST 0x810
#define OFF_CURRENT_ADVENTURE_LEVEL 0x56ac
#define OFF_SUN 0x56bc
#define OFF_PASS_LEVEL 0x5760
#define OFF_FIELD_TYPE 0x56a8
#define OFF_CURRENT_WAVE 0x56d8
#define OFF_FRESH_COUNTDOWN_TOTAL 0x56fc
#define OFF_FRESH_COUNTDOWN_REMAIN 0x56f8
#define OFF_WAVE_HITPOINT_TOTAL 0x56f4
#define OFF_WAVE_HITPOINT_BOUNDARY 0x56f0
// -- Items
#define OFF_ITEM_PICKUP 0x5c
#define OFF_ITEM_TYPE 0x64
// -- Mowers
#define OFF_MOWER_TRIGGER 0x2c
// -- Projectile
#define OFF_PROJECTILE_CODE 0x68
// -- Zombies
#define OFF_ZOMBIE_CODE 0x30
#define OFF_ZOMBIE_HP 0xd4
#define OFF_ZOMBIE_ARMOR1_CODE 0xd0
#define OFF_ZOMBIE_ARMOR1_HP 0xdc
#define OFF_ZOMBIE_ARMOR2_CODE 0xe4
#define OFF_ZOMBIE_ARMOR2_HP 0xe8
#define OFF_ZOMBIE_ROW 0x1c
#define OFF_ZOMBIE_POS_X 0x38
#define OFF_ZOMBIE_POS_Y 0x3c
#define OFF_ZOMBIE_ACTION_REMAIN_TIME 0x74
#define OFF_ZOMBIE_BUTTER_COVER 0xbc
#define OFF_ZOMBIE_ICE 0xc0
#define OFF_ZOMBIE_FLAGS1 0xc4
#define OFF_ZOMBIE_FLAGS2 0xc8
#define OFF_ZOMBIE_ACTION 0x34
#define OFF_ZOMBIE_SIZE 0x128
// -- Plants
#define OFF_PLANT_CODE 0x30
#define OFF_PLANT_HP 0x4c
#define OFF_PLANT_X 0x8
#define OFF_PLANT_Y 0xc
#define OFF_PLANT_SHOOT_REMAIN_TIME 0x64
#define OFF_PLANT_LIGHT 0xc4

// -- Properties
#define PROP_FLAG_CODE 1
#define PROP_SCREENDOOR_CODE 6
#define PROP_NEWSPAPER_CODE 5
#define PROP_POGO_CODE 18
#define PROP_THIEF_CODE 20
#define PROP_LADDER_CODE 21
#define PROP_GARGANTUAR_CODE 23
#define PROP_ZOMBIE_BOSS 25
#define PROP_TALLNUT_ZOMBIE 31
#define PROP_RED_CODE 32
#define PROP_JACKBOX_CODE 15
#define PROP_ZOMBONI_CODE 12
#define PROP_CATAPULT_CODE 22
#define PROP_ZOMBIE_JALAPENO_CODE 28
#define PROP_DOLPHIN_RIDER_CODE 14
#define PROP_BACKUP_DANCER_CODE 9
// 防具相关
#define PROP_ZOMBIE_CORNHEAD_CODE 1
#define PROP_ZOMBIE_CORNHEAD_HP 300
#define PROP_ZOMBIE_BUCKET_CODE 2
#define PROP_ZOMBIE_BUCKET_HP 1100
#define PROP_ZOMBIE_POGO_BUCKET_HP 500
#define PROP_ZOMBIE_SCREENDOOR_CODE 1
#define PROP_ZOMBIE_POGO_SCREENDOOR_HP 500
#define PROP_ZOMBIE_PAPER_CODE 2
#define PROP_ZOMBIE_PAPER_HP 300
// 僵尸新设定
#define PROP_ZOMBIE_THIEF_TIME 200
// 状态代码
#define PROP_ZOMBIE_JACKBOX_BOOM 16
// 冰豆冰冻时间
#define PROP_ZOMBIE_ICE_TIME 300

#define PROP_SUNFLOWER_CODE 1
#define PROP_TWICE_SUNFLOWER_CODE 41
#define PROP_SUN_MUSHROOM_CODE 9
#define PROP_STARFRUIT_CODE 29
// 子弹伤害属性
#define PROP_PLANT_PEA_PROJECTILE_CODE 1
#define PROP_PLANT_PEA_PROJECTILE_HIT 30
#define PROP_PLANT_ICE_PEA_HIT 21
#define PROP_PLANT_ICE_PEA_CODE 2
#define PROP_PLANT_ICE_PEA_HIT 21
#define PROP_PLANT_FIRE_PEA_CODE 7
#define PROP_PLANT_FIRE_PEA_HIT 41 // 一个特殊值，帮助判断是哪种子弹
#define PROP_PLANT_CARBAGE_PROJECTILE_CODE 3
#define PROP_PLANT_CARBAGE_PROJECTILE_HIT 45
#define PROP_PLANT_ICE_MELON_PROJECTILE_CODE 6
#define PROP_PLANT_ICE_MELON_PROJECTILE_HIT 61
#define PROP_PLANT_MELON_PROJECTILE_CODE 4
#define PROP_PLANT_MELON_PROJECTILE_HIT 140
#define PROP_PLANT_STARFRUIT_PROJECTILE_CODE 8
#define PROP_PLANT_CAT_PROJECTILE_CODE 9
#define PROP_PLANT_CORNBIT_PROJECTILE_CODE 11
#define PROP_PLANT_BUTTER_PROJECTILE_CODE 13
// 植物血量
#define PROP_PLANT_SUNFLOWER_HP 600
// 植物价格
#define PROP_PLANT_PEA 0
#define PROP_PLANT_PEA_PRICE 75
#define PROP_PLANT_LITTLENUT 3
#define PROP_PLANT_LITTLENUT_PRICE 100
#define PROP_PLANT_TALLNUT 23
#define PROP_PLANT_TALLNUT_PRICE 150
#define PROP_PLANT_MARIGOLD 38
#define PROP_PLANT_MARIGOLD_PRICE 75
#define PROP_PLANT_TORCHWOOD 22
#define PROP_PLANT_TORCHWOOD_PRICE 225
#define PROP_PLANT_STARFRUIT PROP_STARFRUIT_CODE
#define PROP_PLANT_STARFRUIT_PRICE 225
#define PROP_PLANT_MELON 39
#define PROP_PLANT_MELON_PRICE 350
#define PROP_PLANT_ICE_MELON 44
#define PROP_PLANT_ICE_MELON_PRICE 50

// Internal values
#define PROP_INITIALZE_SUN_FOR_HARD_LEVEL 150
#define PROP_PER_COLUMN 80.0
#define PROP_GRID_TYPE_POOL 3
#define PROP_ROW_POOLZOMBIES 2

#define PROP_TRIGGER_MOWER 2
#define PROP_NINETH_SLOT 3

#define PROP_FIRST_CARD_ENTRY 0x34
#define PROP_CARD_COUNT 0x30
#define PROP_CARD_SEED 0x40
#define PROP_CARD_OFFSET 0x5c

#define PROP_PLANTS_SLOT_INFO_STEP 0x24
#define PROP_PLANTS_HIT_COUNT 14
#define PROP_PLANTS_HIT_STEP 0xc
#define PROP_PLANTS_HIT_CODE_OFF 0x4

#define PROP_NEXT_GAMES_REGION 0x90
#define PROP_GAMES_COUNT_PER_PACK 7

#endif // INC_SERVER_PVZ_H
