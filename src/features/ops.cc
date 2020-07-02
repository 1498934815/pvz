/*
 * File      : src/features/common.cc
 * Project   :
 * Author    : ze00
 * Email     : zerozakiGeek@gmail.com
 * Date      : 2020-04-21
 * Module    :
 * License   : MIT
 */
#include "features/ops.h"
#include "server/Pvz.h"
namespace ops {

void hypno(void *zombie, int flags) {
  setI32(flags | 1, incr(zombie, OFF_ZOMBIE_FLAGS1));
}
void fly(void *zombie, int flags) {
  setI32(flags | 0x100, incr(zombie, OFF_ZOMBIE_FLAGS1));
}
void ice(void *zombie, int flags) {
  setI32(flags | 0x10000, incr(zombie, OFF_ZOMBIE_FLAGS1));
  setI32(PROP_ZOMBIE_ICE_TIME, incr(zombie, OFF_ZOMBIE_ICE));
}
void iceLongTime(void *zombie, int flags) {
  setI32(flags | 0x10000, incr(zombie, OFF_ZOMBIE_FLAGS1));
  setI32(PROP_ZOMBIE_ICE_LONG_TIME, incr(zombie, OFF_ZOMBIE_ICE));
}

void back(void *zombie, float distance) {
  void *zx = incr(zombie, OFF_ZOMBIE_POS_X);
  setF32(getF32(zx) + distance, zx);
}
void advance(void *zombie, float distance) {
  back(zombie, -distance);
}
void butter(void *zombie) {
  setI32(500, incr(zombie, OFF_ZOMBIE_BUTTER_COVER));
}
} // namespace ops
