/*
 * File    : src/server/PvzCheats.cc
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-03-28
 * Module  :
 * License : MIT
 */
#include "common/PvzUtils.h"
#include "common/communicator.h"
#include "common/options.h"
#include "server/Pvz.h"
#include <vector>

#ifdef SERVER
#define DEFINE_NORMAL_CHEAT(name) void name(Communicator *com, msgPack *msg)
#define DEFINE_OBJECT_CHEAT(name) void name(Communicator *com, void *object)
#define DEFINE_DAEMON_CHEAT(name) void name(Communicator *com)
DEFINE_NORMAL_CHEAT(setSun) {
  setI32(incrStatus(OFF_SUN), msg->val);
}
DEFINE_NORMAL_CHEAT(freePlants) {
  setI32(incrBase(OFF_FREE_PLANTS), 1);
}
DEFINE_NORMAL_CHEAT(disableFreePlants) {
  setI32(incrBase(OFF_FREE_PLANTS), 0);
}
DEFINE_NORMAL_CHEAT(setCoin) {
  setI32(incrSaves(OFF_COIN), msg->val);
}
DEFINE_NORMAL_CHEAT(unlockAll) {
  setI32(incrSaves(OFF_LIFE2), 2);
  void *start = incrSaves(OFF_STORE_GOOD_START),
       *end = incrSaves(OFF_STORE_GOOD_END);
  while (start != end) {
    setI32(start, 1);
    start = incr(start, POINTERSIZE);
  }
  // Nineth slot of plants
  setI32(incrSaves(OFF_STORE_GOOD_SLOT), PROP_NINETH_SLOT);
}
DEFINE_OBJECT_CHEAT(printPlantsObject) {
  com->sendMessage(
      makeMsgPack(0, formatBuffer("PLANT@%p TYPE:%d X:%d Y:%d HP:%d", object,
                                  getI32(incr(object, OFF_PLANT_CODE)),
                                  getI32(incr(object, OFF_PLANT_X)),
                                  getI32(incr(object, OFF_PLANT_Y)),
                                  getI32(incr(object, OFF_PLANT_HP)))));
}
DEFINE_OBJECT_CHEAT(printZombiesObject) {
  com->sendMessage(
      makeMsgPack(0, formatBuffer("ZOMBIE@%p TYPE:%d X:%f Y:%f HP:%d", object,
                                  getI32(incr(object, OFF_ZOMBIE_CODE)),
                                  getF32(incr(object, OFF_ZOMBIE_POS_X)),
                                  getF32(incr(object, OFF_ZOMBIE_POS_Y)),
                                  getI32(incr(object, OFF_ZOMBIE_HP)))));
}
bool isProper(int seed, int fieldType) {
  switch (seed) {
  // 泳池模式得到海豚、潜水
  case 0xe:
  case 0xb:
    return in_range(fieldType, POOL, FOG);
  // 非屋顶、月夜得到舞王、矿工
  case 0x8:
  case 0x11:
    return !in_range(fieldType, ROOF, MOONNIGHT);
  case 0xc:
    return fieldType != NIGHT;
  }
  return true;
}
int generateZombiesSeed(int fieldType, std::vector<int> &&seeds) {
  uint32_t which;
  do {
    which = rand() % seeds.size();
  } while (!isProper(seeds[which],
                     fieldType)); // 如果得到的僵尸不合适,重新生成
  return seeds[which];
}
void replaceSeeds(std::vector<int> &&seeds, unsigned end, bool hasThief,
                  bool hasRed) {
  void *seedAddr = incrStatus(OFF_ZOMBIES_LIST);
  int fieldType = getI32(incrStatus(OFF_FIELD_TYPE));
  for (size_t wave = 0; wave < 40; ++wave) {
    for (size_t i = 0; i < 50; ++i) {
      if (i >= end)
        setI32(seedAddr, -1);
      // 如果尾数是9
      else if (hasThief && (wave % 10 == 9) && i < 3)
        setI32(seedAddr, PROP_THIEF_CODE);
      else if (hasRed && i >= 40)
        setI32(seedAddr, PROP_RED_CODE);
      else
        setI32(seedAddr, generateZombiesSeed(fieldType, std::move(seeds)));
      seedAddr = incr(seedAddr, POINTERSIZE);
    }
  }
}
DEFINE_NORMAL_CHEAT(callLadders) {
  replaceSeeds({PROP_LADDER_CODE}, 10, false, false);
}
DEFINE_NORMAL_CHEAT(callGargantuar) {
  replaceSeeds({PROP_GARGANTUAR_CODE}, 10, false, false);
}
DEFINE_NORMAL_CHEAT(doLimits) {
  replaceSeeds({0, 0x4, 0x10, 0xf, 0xc, 0x8, 0xe, 0x7, 0x16, 0xb, 0x11, 0x12,
                0x3, PROP_GARGANTUAR_CODE, PROP_RED_CODE, PROP_LADDER_CODE},
               50, true, true);
}
DEFINE_NORMAL_CHEAT(setZombiesList) {
  auto &&seeds = parseInts(msg->msg);
  for (auto &&seed : seeds) {
    if (!in_range(seed, 0, PROP_RED_CODE)) {
      com->sendMessage(makeMsgPack(0, "Invalid code that out of range(0-32)",
                                   msgStatus::REMOTE_ERROR));
      return;
    }
  }
  replaceSeeds(std::move(seeds), 50, false, false);
}
DEFINE_NORMAL_CHEAT(passLevel) {
  setI32(incrStatus(OFF_PASS_LEVEL), 1);
}
DEFINE_NORMAL_CHEAT(setFlags) {
  setI32(incr(getPtr(incrStatus(OFF_FLAGS_HELPER)), OFF_FLAGS), msg->val);
}
DEFINE_NORMAL_CHEAT(switchMode) {
  setI32(incrBase(OFF_MODE), msg->val);
}
DEFINE_NORMAL_CHEAT(switchField) {
  if (!in_range(msg->val, DAY, GARDEN)) {
    com->sendMessage(makeMsgPack(0, "Invalid code of type of field",
                                 msgStatus::REMOTE_ERROR));
    return;
  }
  setI32(incrStatus(OFF_FIELD_TYPE), msg->val);
}
DEFINE_NORMAL_CHEAT(setAdventureLevel) {
  setI32(incrSaves(OFF_ADVENTURE_LEVEL), msg->val);
}
DEFINE_NORMAL_CHEAT(setCards) {
  void *card = getPtr(incrStatus(OFF_CARDS_ENTRY));
  size_t cnt = getU32(incr(card, PROP_CARD_COUNT));
  std::vector<int> &&seeds = parseInts(msg->msg);
  if (seeds.size() > cnt) {
    com->sendMessage(
        makeMsgPack(0, "Seeds count is more than your brought slot count",
                    msgStatus::REMOTE_ERROR));
    return;
  }
  card = incr(card, PROP_FIRST_CARD_ENTRY);
  for (auto &&seed : seeds) {
    setI32(incr(card, PROP_CARD_SEED), seed);
    card = incr(card, PROP_CARD_OFFSET);
  }
}
DEFINE_OBJECT_CHEAT(__pickupGood) {
  if (in_range(getI32(incr(object, OFF_GOOD_TYPE)), 1, 4))
    setI32(incr(object, OFF_GOOD_PICKUP), 1);
}
DEFINE_DAEMON_CHEAT(autoPickup) {
  eachGood(com, __pickupGood);
  usleep(500000);
}
static std::vector<PvzPoint> *__ladderPoints;
DEFINE_OBJECT_CHEAT(__putLadder) {
  if (__ladderPoints->empty() ||
      getI32(incr(object, OFF_ZOMBIE_CODE)) != PROP_LADDER_CODE)
    return;
  auto &&point = __ladderPoints->back();
  float y = point.y * 100;
  setF32(incr(object, OFF_ZOMBIE_POS_X), y);
  setF32(incr(object, OFF_ZOMBIE_POS_Y), y);
  setI32(incr(object, OFF_ZOMBIE_ROW), point.x - 1);
  __ladderPoints->pop_back();
}
DEFINE_NORMAL_CHEAT(putLadder) {
  std::vector<PvzPoint> &&ladderPoints = parsePoints(msg->msg);
  __ladderPoints = &ladderPoints;
  eachZombie(com, __putLadder);
}
DEFINE_OBJECT_CHEAT(triggerMowers) {
  setI32(incr(object, OFF_MOWER_TRIGGER), PROP_TRIGGER_MOWER);
}
DEFINE_OBJECT_CHEAT(zombiesButterCover) {
  setI32(incr(object, OFF_ZOMBIE_BUTTER_COVER), 5000);
}
#endif
#define HINT_INT "请输入一个数据"
#define HINT_INTS "请输入一串以英文逗号分割的数据"
#define HINT_POINTS "请输入行列数据(多个行列以,分割,行与列以.分割)"
DEFINE_EXTERNAL_OPTIONS(
    DEFINE_OPTION(GAMING | GETINT, "设置阳光", HINT_INT, setSun),
    DEFINE_OPTION(GETINT, "设置金币", HINT_INT, setCoin),
    DEFINE_OPTION(NONE, "免费植物", nullptr, freePlants),
    DEFINE_OPTION(NONE, "禁用免费植物", nullptr, disableFreePlants),
    DEFINE_OPTION(GAMING, "只出梯子", nullptr, callLadders),
    DEFINE_OPTION(GAMING, "只出巨人", nullptr, callGargantuar),
    DEFINE_OPTION(GAMING, "阵型压力测试", nullptr, doLimits),
    DEFINE_OPTION(GAMING | GETINTS, "自定义出怪列表", HINT_INTS,
                  setZombiesList),
    DEFINE_OPTION(GAMING, "通过本关", nullptr, passLevel),
    DEFINE_OPTION(GAMING | GETINT, "设置旗数", HINT_INT, setFlags),
    DEFINE_OPTION(GAMING | GETINT, "设置模式", HINT_INT, switchMode),
    DEFINE_OPTION(GAMING | GETINT, "设置场景", HINT_INT, switchField),
    DEFINE_OPTION(GAMING | GETINTS, "设置卡槽", HINT_INTS, setCards),
    DEFINE_OPTION(GETINT, "设置冒险关卡", HINT_INT, setAdventureLevel),
    DEFINE_OPTION(DAEMON_CALLBACK, "自动拾取", nullptr,
                  .daemon_callback = autoPickup),
    DEFINE_OPTION(CANCEL_DAEMON_CALLBACK, "取消自动拾取", nullptr, nullptr),
    DEFINE_OPTION(NONE, "解锁各游戏模式", nullptr, unlockAll),
    DEFINE_OPTION(GAMING | GETPOINTS, "移动场上梯子僵尸", HINT_POINTS,
                  putLadder),
    DEFINE_OPTION(GAMING | MOWERS_CALLBACK, "触发推车", nullptr,
                  .object_callback = triggerMowers),
    DEFINE_OPTION(GAMING | ZOMBIES_CALLBACK, "黄油糊脸", nullptr,
                  .object_callback = zombiesButterCover),
    DEFINE_OPTION(GAMING | ZOMBIES_CALLBACK, "打印僵尸信息", nullptr,
                  .object_callback = printZombiesObject),
    DEFINE_OPTION(GAMING | PLANTS_CALLBACK, "打印植物信息", nullptr,
                  .object_callback = printPlantsObject))
