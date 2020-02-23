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
#include <stdlib.h>
#define DEFINE_FEATURE(name)                                                   \
  extern "C" void name();                                                      \
  extern "C" void name##_end();
DEFINE_FEATURE(whenPlantDead)
#define FEATURE(name, offset, feature, buffer)                                 \
  {                                                                            \
    name, offset, feature, (uintptr_t)feature##_end - (uintptr_t)feature, 0,   \
        buffer                                                                 \
  }
struct feature features[] = {
    FEATURE("小坚果被咬死前爆炸", 0x255c88, whenPlantDead, 0x8e5ddc),
    FEATURE("高坚果被咬死前变窝瓜", 0x358490, whenPlantDead, 0x8e5ddc),
    {0, 0, 0},
};
