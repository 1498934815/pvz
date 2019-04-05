/*
 * File    : src/server/PvzCheats.cc
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-03-28
 * Module  :
 * License : MIT
 */
#include <common/communicator.h>
#include <common/options.h>
#include <server/Pvz.h>
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
DEFINE_OBJECT_CHEAT(zombiesButterCover) {
  setI32(incr(object, OFF_ZOMBIE_BUTTER_COVER), 5000);
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
  // Nineth plant slot
  setI32(incrSaves(OFF_STORE_GOOD_SLOT), PROP_NINE_SLOT);
}
DEFINE_OBJECT_CHEAT(printObject) {
  static char buf[255];
  sprintf(buf, "OBJECT %p", object);
  com->sendMessage(makeMsgPack(0, buf));
}
bool isProper(int fieldType, int seed) {
  switch (fieldType) {
  // 泳池模式得到海豚、潜水
  case 0xe:
  case 0xb:
    return in_range(fieldType, POOL, FOG);
  // 非屋顶、月夜得到舞王、矿工
  case 0x8:
  case 0x11:
    return !in_range(fieldType, ROOF, MOONNIGHT);
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
  for (size_t wave = 0; wave < 20; ++wave) {
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
  if (!in_range(msg->val, DAY, MOONNIGHT)) {
    com->sendMessage(makeMsgPack(0, "Invalid code of type of field",
                                 msgStatus::REMOTE_ERROR));
  }
  setI32(incrStatus(OFF_FIELD_TYPE), msg->val);
}
DEFINE_OBJECT_CHEAT(__pickupGood) {
  if (in_range(getI32(incr(object, OFF_GOOD_TYPE)), 1, 4))
    setI32(incr(object, OFF_GOOD_PICKUP), 1);
}
DEFINE_DAEMON_CHEAT(autoPickup) {
  eachGood(com, __pickupGood);
  usleep(500000);
}
DEFINE_OBJECT_CHEAT(triggerMowers) {
  setI32(incr(object, OFF_MOWER_TRIGGER), PROP_TRIGGER_MOWER);
}
#endif
DEFINE_EXTERNAL_OPTIONS(
    DEFINE_OPTION(GAMING | GETINT, "设置阳光", nullptr, setSun),
    DEFINE_OPTION(GETINT, "设置金币", nullptr, setCoin),
    DEFINE_OPTION(GAMING, "免费植物", nullptr, freePlants),
    DEFINE_OPTION(GAMING, "禁用免费植物", nullptr, disableFreePlants),
    DEFINE_OPTION(GAMING | ZOMBIES_CALLBACK, "黄油糊脸", nullptr,
                  .object_callback = zombiesButterCover),
    DEFINE_OPTION(GAMING, "只出梯子", nullptr, callLadders),
    DEFINE_OPTION(GAMING, "只出巨人", nullptr, callGargantuar),
    DEFINE_OPTION(GAMING, "通过本关", nullptr, passLevel),
    DEFINE_OPTION(GAMING | GETINT, "设置旗数", nullptr, setFlags),
    DEFINE_OPTION(GAMING | GETINT, "设置模式", nullptr, switchMode),
    DEFINE_OPTION(GAMING | GETINT, "设置场景", nullptr, switchField),
    DEFINE_OPTION(DAEMON_CALLBACK, "自动拾取", nullptr,
                  .daemon_callback = autoPickup),
    DEFINE_OPTION(CANCEL_DAEMON_CALLBACK, "取消自动拾取", nullptr, nullptr),
    DEFINE_OPTION(NONE, "解锁各游戏模式", nullptr, unlockAll),
    DEFINE_OPTION(GAMING | MOWERS_CALLBACK, "触发推车", nullptr,
                  .object_callback = triggerMowers),
    DEFINE_OPTION(GAMING | ZOMBIES_CALLBACK, "打印僵尸对象", nullptr,
                  .object_callback = printObject),
    DEFINE_OPTION(GAMING | PLANTS_CALLBACK, "打印植物对象", nullptr,
                  .object_callback = printObject))
