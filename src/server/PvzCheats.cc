/*
 * File    : src/server/PvzCheats.cc
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-03-28
 * Module  :
 * License : MIT
 */
#include <common/options.h>
#include <server/Pvz.h>
#include <server/PvzServer.h>

#ifdef SERVER
#define DEFINE_NORMAL_CHEAT(name) void name(Communicator *com, msgPack *msg)
#define DEFINE_OBJECT_CHEAT(name) void name(Communicator *com, void *object)
#define DEFINE_DAEMON_CHEAT(name) void name(Communicator *com, bool *on)
DEFINE_NORMAL_CHEAT(setSun) {
  setI32(incrStatus(OFF_SUN), msg->val);
}
DEFINE_NORMAL_CHEAT(freePlants) {
  setI32(incrBase(OFF_FREE_PLANTS), 1);
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
  setI32(incrSaves(OFF_STORE_GOOD_SLOT), 3);
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
int generateZombiesSeed(int fieldType, int seeds[], unsigned seedsCount) {
  uint32_t which;
  do {
    which = rand() % seedsCount;
  } while (!isProper(seeds[which],
                     fieldType)); // 如果得到的僵尸不合适,重新生成
  return seeds[which];
}
void replaceSeeds(int seeds[], unsigned seedsCount, unsigned end, bool hasThief,
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
        setI32(seedAddr, generateZombiesSeed(fieldType, seeds, seedsCount));
      seedAddr = incr(seedAddr, POINTERSIZE);
    }
  }
}
DEFINE_OBJECT_CHEAT(callLadders) {
  int seeds[] = {PROP_LADDER_CODE};
  replaceSeeds(seeds, 1, 10, false, false);
}
DEFINE_OBJECT_CHEAT(callGargantuar) {
  int seeds[] = {PROP_GARGANTUAR_CODE};
  replaceSeeds(seeds, 1, 10, false, false);
}
#endif
DEFINE_EXTERNAL_OPTIONS(
    DEFINE_OPTION(GAMING | GETINT, "SetSun", nullptr, setSun),
    DEFINE_OPTION(GETINT, "SetCoin", nullptr, setCoin),
    DEFINE_OPTION(GAMING, "FreePlants", nullptr, freePlants),
    DEFINE_OPTION(GAMING | ZOMBIES_CALLBACK, "ButterCovered", nullptr,
                  .object_callback = zombiesButterCover),
    DEFINE_OPTION(GAMING | ZOMBIES_CALLBACK, "CallLadders", nullptr,
                  .object_callback = callLadders),
    DEFINE_OPTION(GAMING | ZOMBIES_CALLBACK, "CallGargantuar", nullptr,
                  .object_callback = callGargantuar),
    DEFINE_OPTION(NONE, "UnlockAll", nullptr, unlockAll),
    DEFINE_OPTION(GAMING | ZOMBIES_CALLBACK, "PrintZombies", nullptr,
                  .object_callback = printObject),
    DEFINE_OPTION(GAMING | PLANTS_CALLBACK, "PrintPlants", nullptr,
                  .object_callback = printObject))
