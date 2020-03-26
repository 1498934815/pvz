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
#include "server/Pvz.h"
#include "server/PvzServer.h"
#include <stdlib.h>
#define DEFINE_FEATURE(name)                                                   \
  extern "C" void name();                                                      \
  extern "C" void name##_end();
#define FEATURE(offset, feature)                                               \
  { offset, (void *)feature, POINTER_DIFFERENCE(feature##_end, feature), 0, 0 }
#define FEATURE_END                                                            \
  { 0 }
#define FEATURE_GROUP(name, features)                                          \
  { name, features, false }
DEFINE_FEATURE(asm_features_region);
struct feature features_region =
    FEATURE(FEATURES_BUFFER_BASE, asm_features_region);

DEFINE_FEATURE(whenPlantDiying)
struct feature plantFeatures[] = {
    FEATURE(0x255c88, whenPlantDiying),
    FEATURE(0x358490, whenPlantDiying),
    FEATURE_END,
};
DEFINE_FEATURE(zombieHat)
struct feature zombieFeatures[] = {
    FEATURE(0x340d80, zombieHat),
    FEATURE_END,
};
DEFINE_FEATURE(adventureSixthLevelScene)
struct feature levelsFeatures[] = {
    FEATURE(0x1438c0, adventureSixthLevelScene),
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
int oneOfOneHundred() {
  return rand() % 15 == 1;
}
#define HELPER(name) ((uintptr_t)name)
uintptr_t helpers[] = {
    HELPER(oneOfOneHundred),
    0,
};
static void allocateBuffer(struct feature *feature) {
  extern struct feature features_region;
  feature->buffer = FEATURES_BUFFER_BASE +
                    POINTER_DIFFERENCE(feature->code, features_region.code);
#if DEBUG
  DEBUG_LOG("ALLOCATED BUFFER %#lx %u", feature->buffer, feature->codesize);
#endif
}
static void initFeaturesCode() {
  extern struct feature features_region;
  static bool initialized = false;
  if (!initialized) {
    memcpy(incr(__getCoreLib(), features_region.offset), features_region.code,
           features_region.codesize);
    for (unsigned i = 0; helpers[i] != 0; ++i)
      setU32(helpers[i],
             incr(__getCoreLib(), features_region.offset + POINTERSIZE * i));
    initialized = true;
  }
}
void loadPvzFeatures(Communicator *server) {
  initFeaturesCode();
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
        if (feature->originalcode == 0) {
          feature->originalcode = getI32(codeptr);
#if DEBUG
          ptrdiff_t decode = ((0xffffff & blCode) << 2) | 0xff000000;
          DEBUG_LOG("CODEPTR %p codeptr %p code %x delta %x/%d decode %x\n",
                    codeptr, feature->code, blCode, diff, diff, decode);
          DEBUG_LOG("TO %p/%d", buffer, feature->codesize);
#endif
          // memcpy((void *)buffer, (void *)feature->code, feature->codesize);
        }
        setI32(blCode, codeptr);
      } else {
        setI32(feature->originalcode, codeptr);
      }
    }
  }
}
