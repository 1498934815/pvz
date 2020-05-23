/*
 * File      : src/features/features.cc
 * Project   :
 * Author    : ze00
 * Email     : zerozakiGeek@gmail.com
 * Date      : 2020-02-13
 * Module    :
 * License   : MIT
 */
#include "features/features.h"
#include "common/PvzUtils.h"
#include "common/common.h"
#include "features/ops.h"
#include "server/Pvz.h"
#include "server/PvzServer.h"
#include <map>
#include <math.h>
#include <stdlib.h>
#define DEFINE_FEATURE(name) extern "C" void name();
#define __FEATURE(offset, extracode, feature)                                  \
  { offset, extracode, (void *)feature, {0, 0}, 0 }
#define FEATURE(offset, feature) __FEATURE(offset, 0, feature)
#define FEATURE_END                                                            \
  { 0 }
#define FEATURE_GROUP(name, features)                                          \
  { name, features, false }
DEFINE_FEATURE(asm_features_region)
DEFINE_FEATURE(asm_features_region_end)
struct feature features_region =
    FEATURE(FEATURES_BUFFER_BASE, asm_features_region);

DEFINE_FEATURE(whenPlantDiying)
DEFINE_FEATURE(meanPlant)
DEFINE_FEATURE(zombieHypno)
DEFINE_FEATURE(zombieIce)
DEFINE_FEATURE(plantHurtting)
DEFINE_FEATURE(randomProjectile)
DEFINE_FEATURE(bringGrave)
DEFINE_FEATURE(noScaredMushroom)
struct feature plantFeatures[] = {
    FEATURE(0x255c88, whenPlantDiying),
    FEATURE(0x358490, whenPlantDiying),
    FEATURE(0x35adec, zombieHypno),
    FEATURE(0x35a860, zombieIce),
    FEATURE(0x238cd4, plantHurtting),
    FEATURE(0x254b38, randomProjectile),
    // FEATURE(0x1416d0, bringGrave),
    FEATURE(0x23e0cc, meanPlant),
    // bx lr
    __FEATURE(0x23a2b4, static_cast<int>(0xe12fff1e), noScaredMushroom),
    FEATURE(0x23a4b0, noScaredMushroom),
    FEATURE(0x23a454, noScaredMushroom),
    FEATURE_END,
};
DEFINE_FEATURE(zombieHat)
DEFINE_FEATURE(paperHP)
DEFINE_FEATURE(pogoNewLine)
DEFINE_FEATURE(pogoJumpOverTallnut)
DEFINE_FEATURE(pogoPaperDroped)
DEFINE_FEATURE(poleVaultingJumpTwice)
DEFINE_FEATURE(zombieEatting)
DEFINE_FEATURE(zombiePropertiesInit)
DEFINE_FEATURE(thiefQuickly)
struct feature zombieFeatures[] = {
    FEATURE(0x340d80, zombieHat),
    FEATURE(0x342338, paperHP),
    FEATURE(0x345c70, pogoNewLine),
    FEATURE(0x345c04, pogoNewLine),
    FEATURE(0x345bd0, pogoNewLine),
    FEATURE(0x34553c, pogoJumpOverTallnut), // 跳跳被高坚果绊倒后的效果
    FEATURE(0x359ef8, pogoPaperDroped),     // 跳跳报纸掉落后
    FEATURE(0x346b5c, poleVaultingJumpTwice),
    FEATURE(0x345084, thiefQuickly),
    // 暂时不使用
    // FEATURE(0x35838c, zombieEatting),
    // FEATURE(0x3409a0, zombiePropertiesInit),
    FEATURE_END,
};
DEFINE_FEATURE(adventureSixthLevelScene)
DEFINE_FEATURE(adventureSixthLevel)
DEFINE_FEATURE(replaceZombiesSeed)
DEFINE_FEATURE(cardsSelectionInit)
DEFINE_FEATURE(alterGardensZombiesType)
struct feature levelsFeatures[] = {
    FEATURE(0x1438c4, adventureSixthLevelScene),
    FEATURE(0x141ad4, replaceZombiesSeed),
    // push {lr}
    __FEATURE(0x24cb10, static_cast<int>(0xe52de004), adventureSixthLevel),
    FEATURE(0x1cf04c, cardsSelectionInit),
    FEATURE(0x1439cc, alterGardensZombiesType),
    FEATURE_END,
};
DEFINE_FEATURE(dropSeedsCard)
struct feature itemsFeatures[] = {
    FEATURE(0x18eec0, dropSeedsCard),
    FEATURE_END,
};
struct features_group features[] = {
    FEATURE_GROUP("植物属性修改", plantFeatures),
    FEATURE_GROUP("僵尸属性修改", zombieFeatures),
    FEATURE_GROUP("关卡特性修改", levelsFeatures),
    FEATURE_GROUP("掉落物品修改", itemsFeatures),
    {nullptr},
};
static void whenZombieBorn(int, int, void *zombie, int hp) {
  int code = getI32(incr(zombie, OFF_ZOMBIE_CODE));
  setI32(hp, incr(zombie, OFF_ZOMBIE_HP));
  if (code == PROP_SCREENDOOR_CODE || code == PROP_FLAG_CODE) {
#warning ZOMBIES SPEED
    setF32(0.80, incr(zombie, 0x40));
    // 加一个铁桶
    setI32(PROP_ZOMBIE_BUCKET_CODE, incr(zombie, OFF_ZOMBIE_ARMOR1_CODE));
    // 设置一类防具血量
    setI32(PROP_ZOMBIE_BUCKET_HP, incr(zombie, OFF_ZOMBIE_ARMOR1_HP));
    // 红白眼、高坚果概率变小
  } else if ((code == PROP_GARGANTUAR_CODE || code == PROP_RED_CODE ||
              code == PROP_TALLNUT_ZOMBIE) &&
             oneOfOneHundred()) {
    setF32(0.5, incr(zombie, OFF_ZOMBIE_SIZE));
  } else if (code == PROP_POGO_CODE) {
    // 跳跳诞生时加铁桶、铁门
    // setI32(PROP_ZOMBIE_PAPER_CODE, incr(zombie, OFF_ZOMBIE_ARMOR2_CODE));
    setI32(PROP_ZOMBIE_PAPER_HP, incr(zombie, OFF_ZOMBIE_ARMOR2_HP));
    // setI32(1000, incr(zombie, OFF_ZOMBIE_HP));
    setI32(PROP_ZOMBIE_BUCKET_CODE, incr(zombie, OFF_ZOMBIE_ARMOR1_CODE));
    setI32(PROP_ZOMBIE_POGO_BUCKET_HP, incr(zombie, OFF_ZOMBIE_ARMOR1_HP));
  } else if (code == PROP_ZOMBIE_JALAPENO_CODE ||
             code == PROP_DOLPHIN_RIDER_CODE) {
    setI32(PROP_ZOMBIE_CORNHEAD_CODE, incr(zombie, OFF_ZOMBIE_ARMOR1_CODE));
    setI32(PROP_ZOMBIE_CORNHEAD_HP, incr(zombie, OFF_ZOMBIE_ARMOR1_HP));
  }
}
// XXX unused function
static int whenZombiePropertiesInit(int, void *fp, int code, void *zombie) {
  if (code == PROP_BACKUP_DANCER_CODE) {
    int codes[] = {PROP_NEWSPAPER_CODE, PROP_POGO_CODE, PROP_TALLNUT_ZOMBIE,
                   PROP_FLAG_CODE};
    code = PROP_BACKUP_DANCER_CODE;
    if (oneOfThree())
      code = codes[rand() % (sizeof(codes) / sizeof(*(codes)))];
  }
  setI32(code, incr(zombie, OFF_ZOMBIE_CODE));
  return code;
}
// r2, 报纸血量
// r3，僵尸地址
static void whenPaperZombieBorn(int, int, int paperHp, void *zombie) {
  if (oneOfThree()) {
    // 本体1800，报纸0
    setI32(1800, incr(zombie, OFF_ZOMBIE_HP));
    setI32(0, incr(zombie, OFF_ZOMBIE_ARMOR2_HP));
  } else {
    setI32(1500, incr(zombie, OFF_ZOMBIE_HP));
    setI32(PROP_ZOMBIE_PAPER_HP, incr(zombie, OFF_ZOMBIE_ARMOR2_HP));
  }
}
static void produceHittingEffects(void *zombie, int hit, int flags) {
  switch (hit) {
  case PROP_PLANT_ICE_PEA_HIT:
    ops::ice(zombie, flags);
    break;
  case PROP_PLANT_FIRE_PEA_HIT:
  case PROP_PLANT_CARBAGE_PROJECTILE_HIT:
    ops::hypno(zombie, flags);
    break;
  case PROP_PLANT_PEA_PROJECTILE_HIT:
    ops::back(zombie, PROP_PER_COLUMN);
    break;
  case PROP_PLANT_ICE_MELON_PROJECTILE_HIT:
    ops::ice(zombie, flags);
    break;
  }
}
static decltype(oneOfThree) *detectRateFunction(int code, int hit) {
  switch (code) {
  case PROP_GARGANTUAR_CODE:
  case PROP_RED_CODE:
    return oneOfOneThousand;
  default:
    if (hit == PROP_PLANT_ICE_MELON_PROJECTILE_HIT)
      return rateOneHalf;
    if (hit == PROP_PLANT_CARBAGE_PROJECTILE_HIT ||
        hit == PROP_PLANT_ICE_PEA_HIT)
      return oneOfFifty;
    return oneOfOneHundred;
  }
}
static void judgeHittingEffects(int code, int hit, int flags, void *zombie) {
  int (*rateFunction)() = detectRateFunction(code, hit);
  switch (code) {
  case PROP_ZOMBIE_BOSS:
  case PROP_THIEF_CODE:
  case PROP_ZOMBONI_CODE:
    break;
  case PROP_ZOMBIE_JALAPENO_CODE:
  case PROP_CATAPULT_CODE:
    // 投篮、辣椒不可被魅惑
    if (hit == PROP_PLANT_CARBAGE_PROJECTILE_HIT ||
        hit == PROP_PLANT_FIRE_PEA_HIT)
      break;
  default:
    if (rateFunction())
      produceHittingEffects(zombie, hit, flags);
  }
}
// r2, 目标血量
// r3, 僵尸地址
// r4, 子弹伤害
static void whenZombieHitting(int isBodyHurt, int hit, int hp, void *zombie) {
  int code = getI32(incr(zombie, OFF_ZOMBIE_CODE));
  int flags = getI32(incr(zombie, OFF_ZOMBIE_FLAGS1));
  // 未被魅惑的高坚果僵尸临死时爆炸
  // 二爷变白眼
  if (hp <= 5 && oneOfFifteen() && !(flags & 1)) {
    switch (code) {
    case PROP_TALLNUT_ZOMBIE:
      setI32(PROP_JACKBOX_CODE, incr(zombie, OFF_ZOMBIE_CODE));
      setI32(PROP_ZOMBIE_JACKBOX_BOOM, incr(zombie, OFF_ZOMBIE_ACTION));
      break;
#if 0
    case PROP_NEWSPAPER_CODE:
#warning HERE, MACRO
      setI32(22, incr(zombie, OFF_ZOMBIE_CODE));
      break;
#endif
    }
  }
  judgeHittingEffects(code, hit, flags, zombie);
}
static void whenPoleVaultingJump(int, int, int action, void *zombie) {
  float x = getF32(incr(zombie, OFF_ZOMBIE_POS_X));
  setI32(action, incr(zombie, OFF_ZOMBIE_ACTION));
  if (x >= 200.0)
    ops::advance(zombie, PROP_PER_COLUMN);
}
// R0, 生成个数
// R2, 写入位置偏移
// R1, 僵尸代码
// R3, 写入位置基址
static void setSeed(void *codeaddr, int level, int wave, int fieldType,
                    int code) {
  extern int generateZombiesSeed(int fieldType, std::vector<int> &&seeds);
  static std::map<int, std::vector<std::vector<int>>> seeds = {
#include "features/seeds.inc"
  };
  if (code != PROP_FLAG_CODE && seeds.find(level) != seeds.end()) {
    switch (wave) {
    case 0 ... 3:
      setI32(generateZombiesSeed(fieldType, std::move(seeds[level][0])),
             codeaddr);
      break;
    case 10:
    case 20:
    case 30:
    case 40:
      setI32(generateZombiesSeed(fieldType, std::move(seeds[level][2])),
             codeaddr);
      break;
    default:
      setI32(generateZombiesSeed(fieldType, std::move(seeds[level][1])),
             codeaddr);
      break;
    }
  } else {
    setI32(code, codeaddr);
  }
}
static void __takeGrave() {
  static void (*takeGrave)(void *, int, int, void *) =
      (void (*)(void *, int, int, void *))incr(__getCoreLib(), 0x141724);
  for (int i = 0; i < 6; ++i) {
    takeGrave(__getStatus(), 1, i, incrStatus(0x140));
  }
}
static void giveSomeSun() {
  setI32(PROP_INITIAL_SUN_FOR_HARD_LEVEL, incrStatus(OFF_SUN));
}
static void unlockDavesCards() {
  void *selected = incr(getPtr(incrBase(OFF_CARDS_SELECTION)),
                        OFF_CARDS_SELECTION_FIRST_SELECT_BY_DAVE);
  for (size_t i = 0; i < PROP_PLANTS_COUNT; ++i) {
    setI32(0, selected);
    selected = incr(selected, OFF_CARDS_SELECTION_NEXT_SELECT_BY_DAVE);
  }
}
static void alterGridsAsPool(void *status) {
  void *grid = incr(status, OFF_BATTLEGROUND_GRID_TYPE);
  int types[] = {PROP_ROW_POOLZOMBIES, 1, 1, 1, 1, 0};
  memcpy(incr(status, OFF_ROW_ZOMBIESTYPE), types, sizeof(types));
  // void *grid = incr(status, OFF_BATTLEGROUND_GRID_TYPE + 5 * POINTERSIZE);
  // setI32(PROP_ROW_POOLZOMBIES, incr(status, OFF_ROW_ZOMBIESTYPE + 5 *
  // POINTERSIZE));
  for (unsigned cnt = 0; cnt < 9; ++cnt) {
    setI32(PROP_GRID_TYPE_POOL, grid);
    // 跳下一个右边的格子
    grid = incr(grid, 6 * POINTERSIZE);
  }
}
// R1,原本的场景代码
static void judgeAdventureScene(int, int, void *status, off_t sceneOff) {
  switch (getI32(incr(status, OFF_CURRENT_ADVENTURE_LEVEL))) {
  case 51 ... 60:
    setI32(fieldTypes::MOONNIGHT, incr(status, sceneOff));
    break;
  case 61 ... 70:
    // setI32(fieldTypes::DAY, incr(status, sceneOff));
    setI32(fieldTypes::GARDEN, incr(status, sceneOff));
    alterGridsAsPool(status);
    break;
  case 71 ... 80:
    setI32(fieldTypes::MUSHROOM_GARDEN, incr(status, sceneOff));
    alterGridsAsPool(status);
    break;
  }
}
static void judgeAdventureLevel(int, int, int level, void *saves) {
  switch (getI32(incr(saves, OFF_ADVENTURE_LEVEL))) {
  case 50:
    setI32(51, incr(saves, OFF_ADVENTURE_LEVEL));
    break;
  case 80:
    setI32(1, incr(saves, OFF_ADVENTURE_LEVEL));
    break;
  default:
    setI32(level, incr(saves, OFF_ADVENTURE_LEVEL));
    break;
  }
}
static void whenCardsSelectionInit() {
  unlockDavesCards();
  if (in_range(getI32(incrStatus(OFF_CURRENT_ADVENTURE_LEVEL)), 50, 80))
    giveSomeSun();
}
static void whenZombiesListInit(int count, int code, int pos, void *list) {
  // R1, 波数
  // 0xc7e88ac4: mul r1, r0, r1
  // 0xc7e88ac8: add r2, r1, r2
  // 0xc7e88acc: add r2, r2, #516
  void *codeaddr = incr(list, pos << 2);
  int wave = (pos - 516) / 50,
      level = getI32(incrStatus(OFF_CURRENT_ADVENTURE_LEVEL)),
      fieldType = getI32(incrStatus(OFF_FIELD_TYPE));
#ifdef DEBUG
  DEBUG_LOG("STATUS %p CODE %d COUNT %d POS(%d,%d), WAVE %d", list, code, count,
            pos, pos << 2, wave);
#endif
  switch (getI32(incrBase(OFF_MODE))) {
  case 0:
    setSeed(codeaddr, level, wave, fieldType, code);
    break;
  default:
    setI32(code, codeaddr);
    break;
  }
}
// R2, 跳跳的动作
// R3，僵尸地址
static void whenPogoJump(int, int, int action, void *zombie) {
  setI32(action, incr(zombie, OFF_ZOMBIE_ACTION));
  int flags = getI32(incr(zombie, OFF_ZOMBIE_FLAGS2));
  setI32(flags | 0x1000000, incr(zombie, OFF_ZOMBIE_FLAGS2));
}
static void whenPogoJumpOverTallnut(int, int, int action, void *zombie) {
  // 被绊倒加铁桶
  setI32(action, incr(zombie, OFF_ZOMBIE_ACTION));
  setI32(PROP_ZOMBIE_BUCKET_CODE, incr(zombie, OFF_ZOMBIE_ARMOR1_CODE));
  setI32(800, incr(zombie, OFF_ZOMBIE_ARMOR1_HP));
}
static void whenPogoPaperDroped(int, int, int status, void *zombie) {
  if (getI32(incr(zombie, OFF_ZOMBIE_CODE)) == PROP_POGO_CODE)
    setI32(0, incr(zombie, OFF_ZOMBIE_ACTION));
  else
    setI32(status, incr(zombie, OFF_ZOMBIE_ACTION));
}
static void *__shottingPlant;
static void (*__zombieCallback)(void *, float, float, float, float);
static void starfruitHurttingCallback(void *zombie, float px, float py,
                                      float zx, float zy) {
  float r = sqrt(pow(zx - px, 2) + pow(zy - py, 2));
  DEBUG_LOG("PZ %p/%p PX %f PY %f ZX %f ZY %f R %f", __shottingPlant, zombie,
            px, py, zx, zy, r);
  if (zx >= px && r < 200.0) {
    ops::back(zombie, 2 * PROP_PER_COLUMN);
    setF32(zx + PROP_PER_COLUMN, incr(zombie, OFF_ZOMBIE_POS_X));
  }
}
static void whenPlantHurttingCallback(Communicator *, void *zombie) {
  float px = (float)getI32(incr(__shottingPlant, OFF_PLANT_X));
  float py = (float)getI32(incr(__shottingPlant, OFF_PLANT_Y));
  float zx = getF32(incr(zombie, OFF_ZOMBIE_POS_X));
  float zy = getF32(incr(zombie, OFF_ZOMBIE_POS_Y));
  __zombieCallback(zombie, px, py, zx, zy);
}
static void whenPlantHurtting(int, int, int remain, void *plant) {
  setI32(remain, incr(plant, OFF_PLANT_SHOOT_REMAIN_TIME));
  if (remain == 0 &&
      getI32(incr(plant, OFF_PLANT_CODE)) == PROP_STARFRUIT_CODE &&
      oneOfFifty()) {
    __shottingPlant = plant;
    setI32(200, incr(plant, OFF_PLANT_LIGHT));
    __zombieCallback = starfruitHurttingCallback;
    eachZombie(nullptr, whenPlantHurttingCallback);
  }
}
// R2, 原本的子弹类型
// R3，植物
static void whenPlantShotting(int, int, int pcode, void *projectile) {
  // 全部需要-1
#define MOD(name) (name - 1)
  static int projectiles[] = {
      MOD(PROP_PLANT_ICE_MELON_PROJECTILE_CODE),
      MOD(PROP_PLANT_STARFRUIT_PROJECTILE_CODE),
      MOD(PROP_PLANT_ICE_PEA_CODE),
      MOD(PROP_PLANT_FIRE_PEA_CODE),
      MOD(PROP_PLANT_PEA_PROJECTILE_CODE),
      MOD(PROP_PLANT_CARBAGE_PROJECTILE_CODE),
      MOD(PROP_PLANT_CAT_PROJECTILE_CODE),
      MOD(PROP_PLANT_CORNBIT_PROJECTILE_CODE),
      MOD(PROP_PLANT_BUTTER_PROJECTILE_CODE),
  };
  if (pcode == MOD(PROP_PLANT_STARFRUIT_PROJECTILE_CODE) && oneOfFifteen()) {
    setI32(
        projectiles[__rate<(sizeof(projectiles) / sizeof(*(projectiles)))>()],
        incr(projectile, OFF_PROJECTILE_CODE));
    return;
  }
  setI32(pcode, incr(projectile, OFF_PROJECTILE_CODE));
}
#define HELPER(name) ((uintptr_t)name)
// 注意此处顺序与features.s中.word的顺序一致
uintptr_t helpers[] = {
    HELPER(whenZombieBorn),
    HELPER(whenPaperZombieBorn),
    HELPER(whenZombieHitting),
    HELPER(whenZombiesListInit),
    HELPER(whenPogoJump),
    HELPER(whenPogoJumpOverTallnut),
    HELPER(whenPogoPaperDroped),
    HELPER(whenPoleVaultingJump),
    HELPER(whenZombiePropertiesInit),
    HELPER(judgeAdventureLevel),
    HELPER(judgeAdventureScene),
    HELPER(whenCardsSelectionInit),
    HELPER(whenPlantHurtting),
    HELPER(whenPlantShotting),
    HELPER(oneOfThree),
    HELPER(oneOfFifteen),
    HELPER(oneOfOneHundred),
    0,
};
static void allocateBuffer(struct feature *feature) {
  extern struct feature features_region;
  feature->buffer = FEATURES_BUFFER_BASE +
                    POINTER_DIFFERENCE(feature->code, features_region.code);
}
static void initFeaturesCode() {
  extern struct feature features_region;
  static bool initialized = false;
  if (!initialized) {
    memcpy(incr(__getCoreLib(), features_region.offset), features_region.code,
           POINTER_DIFFERENCE(asm_features_region_end, asm_features_region));
    for (unsigned i = 0; helpers[i] != 0; ++i)
      setU32(helpers[i],
             incr(__getCoreLib(), features_region.offset + POINTERSIZE * i));
    initialized = true;
  }
}
// 启用隐藏小游戏
static void enableAllHidenGames() {
  uint32_t hidenGames[] = {
      35, 36, 37, 38, 39, 40, 41, /*42 冰冻关卡, 43, 花园 44 牛顿的力量,
      45 坟墓危险*/ 46,
      47, 48, 49};
  size_t gamesCount = (sizeof(hidenGames) / sizeof(*(hidenGames)));
  // 从第二个游戏包开始
  void *ptr = incr(__getCoreLib(), OFF_GAMEPACK2_GAMES);
  void *next = incr(ptr, PROP_NEXT_GAMES_REGION);
  for (int i = 0; i < gamesCount;) {
    for (int j = PROP_GAMES_COUNT_PER_PACK; j != 0 && i != gamesCount; --j) {
      if (getI32(ptr) == 0) {
        DEBUG_LOG("WRITING %d -> %p", hidenGames[i], ptr);
        setI32(hidenGames[i], ptr);
        ++i;
      }
      ptr = incr(ptr, POINTERSIZE);
    }
    ptr = next;
    next = incr(ptr, PROP_NEXT_GAMES_REGION);
  }
}
// 修改植物价格
static void alterPlantsPrice() {
  std::map<int, int> pricesMap{
      {PROP_PLANT_LITTLENUT, PROP_PLANT_LITTLENUT_PRICE},
      {PROP_PLANT_TALLNUT, PROP_PLANT_TALLNUT_PRICE},
      {PROP_PLANT_MARIGOLD, PROP_PLANT_MARIGOLD_PRICE},
      {PROP_PLANT_TORCHWOOD, PROP_PLANT_TORCHWOOD_PRICE},
      {PROP_PLANT_STARFRUIT, PROP_PLANT_STARFRUIT_PRICE},
      {PROP_PLANT_ICE_MELON, PROP_PLANT_ICE_MELON_PRICE},
      {PROP_PLANT_PEA, PROP_PLANT_PEA_PRICE},
      {PROP_PLANT_MELON, PROP_PLANT_MELON_PRICE},
  };
  void *ptr = incr(__getCoreLib(), OFF_PLANTS_SLOT_INFO);
  for (int i = 0; i < PROP_PLANTS_COUNT; ++i) {
    if (pricesMap.find(getI32(ptr)) != pricesMap.end()) {
      setI32(pricesMap[getI32(ptr)], incr(ptr, 0x10));
    }
    ptr = incr(ptr, PROP_PLANTS_SLOT_INFO_STEP);
  }
}
// 修改植物攻击
static void alterPlantsHit() {
  static std::map<int, int> hitsMap{
      {PROP_PLANT_ICE_PEA_CODE, PROP_PLANT_ICE_PEA_HIT},   // 冰豆改为
      {PROP_PLANT_FIRE_PEA_CODE, PROP_PLANT_FIRE_PEA_HIT}, // 火豆改为
      {PROP_PLANT_PEA_PROJECTILE_CODE, PROP_PLANT_PEA_PROJECTILE_HIT},
      {PROP_PLANT_CARBAGE_PROJECTILE_CODE, PROP_PLANT_CARBAGE_PROJECTILE_HIT},
      {PROP_PLANT_ICE_MELON_PROJECTILE_CODE,
       PROP_PLANT_ICE_MELON_PROJECTILE_HIT},
      {PROP_PLANT_MELON_PROJECTILE_CODE, PROP_PLANT_MELON_PROJECTILE_HIT},
  };
  void *ptr = incr(__getCoreLib(), OFF_PLANTS_HIT);
  for (int i = 0; i < PROP_PLANTS_HIT_COUNT; ++i) {
    // 此处注意,植物代码在incr(ptr, PROP_PLANTS_HIT_CODE_OFF)
    if (hitsMap.find(getI32(incr(ptr, PROP_PLANTS_HIT_CODE_OFF))) !=
        hitsMap.end()) {
      setI32(hitsMap[getI32(incr(ptr, PROP_PLANTS_HIT_CODE_OFF))], ptr);
    }
    ptr = incr(ptr, PROP_PLANTS_HIT_STEP);
  }
}
void loadPvzFeatures(Communicator *server) {
  initFeaturesCode();
  enableAllHidenGames();
  alterPlantsPrice();
  alterPlantsHit();
  for (struct features_group *group = features; group->name != nullptr;
       ++group) {
    if (server != nullptr)
      server->sendMessage(
          makeMsgPack(0, formatBuffer("%s 现在 %s", group->name,
                                      group->isEnabled ? "关" : "开")));
    group->isEnabled = !group->isEnabled;
    for (struct feature *feature = group->features; feature->offset != 0;
         ++feature) {
      if (feature->buffer == 0)
        allocateBuffer(feature);
      void *codeptr = incr(__getCoreLib(), feature->offset);
      void *buffer = incr(__getCoreLib(), feature->buffer);
      ptrdiff_t diff = POINTER_DIFFERENCE(buffer, codeptr) - 8;
      unsigned blCode = 0xeb000000 | (0xffffff & (diff >> 2));
      if (getI32(codeptr) != blCode) {
        if (feature->originalcode[0] == 0) {
          feature->originalcode[0] = getI32(codeptr);
#if DEBUG
          ptrdiff_t decode = ((0xffffff & blCode) << 2) | 0xff000000;
          DEBUG_LOG("CODEPTR %p codeptr %p code %x delta %x/%d decode %x\n",
                    codeptr, feature->code, blCode, diff, diff, decode);
          DEBUG_LOG("TO %p", buffer);
#endif
          if (feature->extracode != 0)
            feature->originalcode[1] = getI32(incr(codeptr, -POINTERSIZE));
          // memcpy((void *)buffer, (void *)feature->code, feature->codesize);
        }
        if (feature->extracode != 0)
          setI32(feature->extracode, incr(codeptr, -POINTERSIZE));
        setI32(blCode, codeptr);
      } else {
        if (feature->extracode != 0)
          setI32(feature->originalcode[1], incr(codeptr, -POINTERSIZE));
        setI32(feature->originalcode[0], codeptr);
      }
    }
  }
}
