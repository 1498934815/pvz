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
/*
void whenPlantDead() {
  asm("cmp r2, #50;"
  "bge whenPlantDead_original_code;"
  "ldr r2, [r3, #0x30];"
  "cmp r2, #23;"
  "bne whenPlantDead_original_code;"
  "mov r2, #17;"
  "str r2, [r3, #0x30];"
  "bx lr");
  void *plant;
  asm("mov %0, r3\n"
      :"=r"(plant));
  if (getI32(incr(plant, OFF_PLANT_CODE)) == 23) {
    setI32(incr(plant, OFF_PLANT_CODE), 17);
  } else {
    asm("str r2, [r3, #76]":::"r2");
  }
}
extern "C" void whenPlantDead_original_code() {
  asm("str r2, [r3, #76]");
}
void whenPlantDead_end(){}
*/
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
