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
DEFINE_NORMAL_CHEAT(switchFreePlants) {
  void *freePlants = incrBase(OFF_FREE_PLANTS);
  bool on = getI32(freePlants) == 1;
  setI32(freePlants, on ? 0 : 1);
  com->sendMessage(makeMsgPack(0, on ? "现在 关" : "现在 开"));
}
DEFINE_NORMAL_CHEAT(setCoin) {
  setI32(incrSaves(OFF_COIN), msg->val);
}
DEFINE_NORMAL_CHEAT(unlockAll) {
  setI32(incrSaves(OFF_LIFE2), 2);
  void *start = incrSaves(OFF_STORE_ITEM_START),
       *end = incrSaves(OFF_STORE_ITEM_END);
  while (start != end) {
    setI32(start, 1);
    start = incr(start, POINTERSIZE);
  }
  // Nineth slot of plants
  setI32(incrSaves(OFF_STORE_ITEM_SLOT), PROP_NINETH_SLOT);
}
DEFINE_OBJECT_CHEAT(printItemssObject) {
  com->sendMessage(
      makeMsgPack(0, formatBuffer("ITEMS@%p TYPE:%d", object,
                                  getI32(incr(object, OFF_ITEM_TYPE)))));
}
DEFINE_OBJECT_CHEAT(printPlantsObject) {
  int code = getI32(incr(object, OFF_PLANT_CODE));
  com->sendMessage(makeMsgPack(
      0, formatBuffer("PLANT@%p TYPE:%d(%s) X:%d Y:%d HP:%d", object, code,
                      cardsCodesMap[code], getI32(incr(object, OFF_PLANT_X)),
                      getI32(incr(object, OFF_PLANT_Y)),
                      getI32(incr(object, OFF_PLANT_HP)))));
}
DEFINE_OBJECT_CHEAT(printZombiesObject) {
  int code = getI32(incr(object, OFF_ZOMBIE_CODE));
  com->sendMessage(
      makeMsgPack(0, formatBuffer("ZOMBIE@%p TYPE:%d(%s) X:%f Y:%f HP:%d",
                                  object, code, zombiesCodesMap[code],
                                  getF32(incr(object, OFF_ZOMBIE_POS_X)),
                                  getF32(incr(object, OFF_ZOMBIE_POS_Y)),
                                  getI32(incr(object, OFF_ZOMBIE_HP)))));
}
DEFINE_NORMAL_CHEAT(printProperties) {
  eachZombie(com, printZombiesObject);
  eachPlant(com, printPlantsObject);
  eachItem(com, printItemssObject);
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
  int code;
  do {
    which = rand() % seeds.size();
    code = seeds[which];
  } while (!isProper(code,
                     fieldType)); // 如果得到的僵尸不合适,重新生成
  // 如果确实输入了僵尸博士的话
  // 只出一个
  if (code == PROP_ZOMBIE_BOSS)
    seeds[which] = 0;
  return code;
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
DEFINE_OBJECT_CHEAT(__pickupItems) {
  if (in_range(getI32(incr(object, OFF_ITEM_TYPE)), 1, 4))
    setI32(incr(object, OFF_ITEM_PICKUP), 1);
}
DEFINE_DAEMON_CHEAT(autoPickup) {
  eachItem(com, __pickupItems);
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
DEFINE_NORMAL_CHEAT(switchOnOffExtraEndlessEntires) {
  void *gameEntries = incr(__getCoreLib(), OFF_GAMEPACK1_GAMES);
  uint32_t extraGames[] = {11, 12, 13, 14, 15, 0x13, 0x17, 0x1b};
  uint32_t originalGames[] = {0x13, 0x17, 0x1b, 0, 0, 0, 0, 0};
  bool on = getI32(gameEntries) == extraGames[0];
  memcpy(gameEntries, on ? originalGames : extraGames, sizeof(originalGames));
  com->sendMessage(makeMsgPack(0, on ? "现在 关" : "现在 开"));
}
DEFINE_NORMAL_CHEAT(switchChomperFast) {
  void *chomperFast = incr(__getCoreLib(), OFF_CHOMPER_TIME);
  bool on = getByte(chomperFast) == 0;
  setByte(chomperFast, on ? 0xfa : 0);
  com->sendMessage(makeMsgPack(0, on ? "现在 关" : "现在 开"));
}
DEFINE_OBJECT_CHEAT(enforceStarFruit) {
  void *codeaddr = incr(object, OFF_PLANT_CODE);
  int code = rand() % 48;
  switch (code) {
  case PROP_SUNFLOWER_CODE:
  case PROP_TWICE_SUNFLOWER_CODE:
  case PROP_SUN_MUSHROOM_CODE:
    return;
  }
  if (getI32(codeaddr) == PROP_STARFRULT_CODE)
    setI32(codeaddr, rand() % 2 == 1 ? 44 : 43);
}
DEFINE_OBJECT_CHEAT(enforceNewsPaper) {
  void *codeaddr = incr(object, OFF_ZOMBIE_CODE);
  int code = rand() % 33;
  if (code == PROP_ZOMBIE_BOSS)
    return;
  if (getI32(codeaddr) == PROP_NEWSPAPER_CODE)
    setI32(codeaddr, code);
}
#endif

const char *cardsCodesMap[] = {
    "豌豆射手", "向日葵",     "樱桃炸弹",   "坚果",          "土豆地雷",
    "寒冰射手", "食人花",     "双发射手",   "小喷菇",        "阳光菇",
    "大喷菇",   "墓碑吞噬者", "迷惑菇",     "胆小菇",        "冰冻菇",
    "毁灭菇",   "睡莲",       "窝瓜",       "三线射手",      "海藻",
    "火爆辣椒", "地刺",       "火炬树桩",   "高坚果",        "海蘑菇",
    "灯笼草",   "仙人掌",     "三叶草",     "双向射手",      "杨桃",
    "南瓜套",   "磁力菇",     "卷心菜投手", "花盆",          "玉米投手",
    "咖啡豆",   "大蒜",       "叶子保护伞", "金盏花",        "西瓜投手",
    "机枪豌豆", "双子向日葵", "忧郁蘑菇",   "猫尾草",        "冰冻西瓜",
    "吸金瓷",   "地刺王",     "玉米大炮",   "模仿者",        "爆炸坚果",
    "巨型坚果", "树苗",       "反双发射手", [55] = "炸弹坑", "换植物卡",
    "阳光",     "钻石",       "潜水僵尸",   "奖杯",          [61] = "普通僵尸",
    "路障僵尸", "撑杆僵尸",   "铁桶僵尸",   "梯子僵尸",      "矿工僵尸",
    "蹦极僵尸", "橄榄球僵尸", "气球僵尸",   "铁门僵尸",      "冰车僵尸",
    "跳跳僵尸", "舞王僵尸",   "巨人僵尸",   "小鬼僵尸",
};

const char *gamesCodesMap[] = {
    "冒险",
    "白天生存模式 (5面旗帜)",
    "黑夜生存模式 (5面旗帜)",
    "泳池生存模式 (5面旗帜)",
    "浓雾生存模式 (5面旗帜)",
    "屋顶生存模式 (5面旗帜)",
    "白天生存模式 (10面旗帜)",
    "黑夜生存模式 (10面旗帜)",
    "泳池生存模式 (10面旗帜)",
    "浓雾生存模式 (10面旗帜)",
    "屋顶生存模式 (10面旗帜)",
    "白天无尽",
    "黑夜无尽",
    "泳池无尽",
    "浓雾无尽",
    "屋顶无尽",
    "植物僵尸",
    "坚果保龄球",
    "老虎机",
    "雨中种植物",
    "宝石迷阵",
    "隐形食脑者",
    "看星星",
    "僵尸水族馆",
    "宝石迷阵转转看",
    "小僵尸大麻烦",
    "保护传送门",
    "你看.他们像柱子一样",
    "雪橇区",
    "僵尸快跑",
    "锤僵尸",
    "谁笑到最后",
    "植物僵尸2",
    "坚果保龄球2",
    "跳跳舞会",
    "僵王博士的复仇",
    "当艺术遇上坚果",
    "晴天",
    "不毛之地",
    "重要时间",
    "当艺术遇上向日葵",
    "空袭",
    "急冻先锋",
    "休闲花园",
    "超乎寻常的压力",
    "坟墓危险",
    "你能找到它吗",
    "暴风雨之夜",
    "飞贼闪电战",
    "松鼠",
    "花瓶终结者",
    "1路向左",
    "第3个花瓶",
    "连锁反应",
    "金属狂潮",
    "胆怯的陶制工",
    "戏法时刻",
    "连锁反应2",
    "破坏之王",
    "花瓶终结者(无限)",
    "僵尸公敌",
    "僵尸公敌2",
    "地道战",
    "完全防御",
    "死亡气球",
    "力拔山河",
    "太空舞步",
    "飞檐走壁",
    "你所有的大脑都是属于我的",
    "僵尸公敌(无限)",
    "追加销售",
    "白天1-1(快速关)",
    "白天1-2(快速关)",
    "白天1-3(快速关)",
    "白天1-4(快速关)",
    "白天1-5(快速关)",
    "白天1-6(快速关)",
    "白天1-7(快速关)",
    "白天1-8(快速关)",
    "白天1-9(快速关)",
    "白天1-10(快速关)",
    "黑夜2-1(快速关)",
    "黑夜2-2(快速关)",
    "黑夜2-3(快速关)",
    "黑夜2-4(快速关)",
    "黑夜2-5(快速关)",
    "黑夜2-6(快速关)",
    "黑夜2-7(快速关)",
    "黑夜2-8(快速关)",
    "黑夜2-9(快速关)",
    "黑夜2-10(快速关)",
    "泳池3-1(快速关)",
    "泳池3-2(快速关)",
    "泳池3-3(快速关)",
    "泳池3-4(快速关)",
    "泳池3-5(快速关)",
    "泳池3-6(快速关)",
    "泳池3-7(快速关)",
    "泳池3-8(快速关)",
    "泳池3-9(快速关)",
    "泳池3-10(快速关)",
    "迷雾4-1(快速关)",
    "迷雾4-2(快速关)",
    "迷雾4-3(快速关)",
    "迷雾4-4(快速关)",
    "迷雾4-5(快速关)",
    "迷雾4-6(快速关)",
    "迷雾4-7(快速关)",
    "迷雾4-8(快速关)",
    "迷雾4-9(快速关)",
    "迷雾4-10(快速关)",
    "屋顶5-1(快速关)",
    "屋顶5-2(快速关)",
    "屋顶5-3(快速关)",
    "屋顶5-4(快速关)",
    "屋顶5-5(快速关)",
    "屋顶5-6(快速关)",
    "屋顶5-7(快速关)",
    "屋顶5-8(快速关)",
    "屋顶5-9(快速关)",
    "屋顶5-10(快速关)",
    "谁笑到最后(白天)",
    "谁笑到最后(黑夜)",
    "谁笑到最后(泳池)",
    "谁笑到最后(迷雾)",
    "谁笑到最后(屋顶)",
    "谁笑到最后泳池(无限)",
    "Com2",
    "R",
};

const char *zombiesCodesMap[] = {
    "普通",   "旗帜", "路障", "撑杆",   "铁桶", "读报", "铁栅门",
    "橄榄球", "舞王", "伴舞", "鸭子",   "潜水", "冰车", "雪橇",
    "海豚",   "小丑", "气球", "矿工",   "跳跳", "雪人", "蹦极",
    "扶梯",   "篮球", "白眼", "小鬼",   "僵博", "豌豆", "坚果",
    "辣椒",   "机枪", "窝瓜", "高坚果", "红眼"};

const char *fieldsCodesMap[] = {"白天", "黑夜", "泳池",   "雾夜",
                                "屋顶", "月夜", "蘑菇园", "花园"};
#define JOIN_STRING(S0, S1) S0 "," S1
#define HINT_INT "请输入一个数据"
#define HINT_INTS "请输入一串以英文逗号分割的数据"
#define HINT_POINTS "请输入行列数据(行与列以.分割,多个行列以,分割)"
#define HINT_DO_LIMITS "非准确的极限出怪"
#define HINT_UNLOCK_ALL                                                        \
  "手动通过五个生存模式后可解锁泳池无尽,可配合设置Flag使用"
#define HINT_USED_TO_DEBUG "调试使用,可配合GameGuardian"
#define HINT_PASSLEVEL "某些模式会在通过后闪退"
#define HINT_SETFLAGS JOIN_STRING(HINT_INT, "有时需手动完成最后一个Flag")
#define HINT_SETMODE JOIN_STRING(HINT_INT, "有些模式会闪退,见" CODE_TXT)
#define HINT_SETFIELD JOIN_STRING(HINT_INT, "0-7,场景代码见" CODE_TXT)
#define HINT_PLANTS_ZOMBIES_LIST                                               \
  JOIN_STRING(HINT_INTS, "图鉴里对应顺序减一,见" CODE_TXT)
#define HINT_SET_SLOTS                                                         \
  JOIN_STRING(HINT_INTS, "普通模式中见对应植物图鉴顺序,见" CODE_TXT)
#define HINT_ADVANTURE JOIN_STRING(HINT_INT, "计算方式为 (大关-1)*10+小关")
#define HINT_CALL_LADDERS                                                      \
  JOIN_STRING(HINT_POINTS, "将当前场上的梯子僵尸移动到输入的坐标")
#define HINT_SET_ZOMBIES_LIST                                                              \
  JOIN_STRING(HINT_INTS, "图鉴顺序减一,根据输入的数据随机生成僵尸,用[" \
                         "通过本关]恢复出怪列表,特殊僵尸见" CODE_TXT)
DEFINE_EXTERNAL_OPTIONS(
    DEFINE_OPTION(GAMING | GETINT, "设置阳光", HINT_INT, setSun),
    DEFINE_OPTION(GETINT, "设置金币", HINT_INT, setCoin),
    DEFINE_OPTION(NONE, "开关免费植物", nullptr, switchFreePlants),
    DEFINE_OPTION(GAMING, "只出十只梯僵", nullptr, callLadders),
    DEFINE_OPTION(GAMING, "只出十只巨人", nullptr, callGargantuar),
    DEFINE_OPTION(GAMING, "阵型压力测试", HINT_DO_LIMITS, doLimits),
    DEFINE_OPTION(GAMING | GETINTS | ZOMBIES_CODE, "自定义出怪列表",
                  HINT_SET_ZOMBIES_LIST, setZombiesList),
    DEFINE_OPTION(GAMING, "通过本关", HINT_PASSLEVEL, passLevel),
    DEFINE_OPTION(GAMING | GETINT, "设置旗数", HINT_SETFLAGS, setFlags),
    DEFINE_OPTION(GAMING | GETINT | GAMES_CODE, "设置模式", HINT_SETMODE,
                  switchMode),
    DEFINE_OPTION(GAMING | GETINT | FIELDS_CODE, "设置场景", HINT_SETFIELD,
                  switchField),
    DEFINE_OPTION(GAMING | GETINTS | CARDS_CODE, "设置卡槽", HINT_SET_SLOTS,
                  setCards),
    DEFINE_OPTION(GETINT, "设置冒险关卡", HINT_ADVANTURE, setAdventureLevel),
    DEFINE_OPTION(DAEMON_CALLBACK | CANCEL_DAEMON_CALLBACK, "开关自动拾取",
                  nullptr, .daemon_callback = autoPickup),
    DEFINE_OPTION(NONE, "解锁各游戏模式", HINT_UNLOCK_ALL, unlockAll),
    DEFINE_OPTION(GAMING | GETPOINTS, "移动场上梯子僵尸", HINT_CALL_LADDERS,
                  putLadder),
    DEFINE_OPTION(GAMING | MOWERS_CALLBACK, "触发推车", nullptr,
                  .object_callback = triggerMowers),
    DEFINE_OPTION(GAMING | ZOMBIES_CALLBACK, "黄油糊脸", nullptr,
                  .object_callback = zombiesButterCover),
    DEFINE_OPTION(NONE, "开关额外无尽入口", nullptr,
                  switchOnOffExtraEndlessEntires),
    DEFINE_OPTION(NONE, "开关食人花秒吞", nullptr, switchChomperFast),
    DEFINE_OPTION(GAMING | PLANTS_CALLBACK, "增强杨桃", nullptr,
                  .object_callback = enforceStarFruit),
    DEFINE_OPTION(GAMING | ZOMBIES_CALLBACK, "增强读纸僵尸", nullptr,
                  .object_callback = enforceNewsPaper),
    DEFINE_OPTION(GAMING, "打印场上调试信息", HINT_USED_TO_DEBUG,
                  printProperties))
