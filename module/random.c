/*
 * File    : module/random.c
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2018-10-16
 * Module  :
 * License : MIT
 */
#include "../inc/cheat.h"
#include "../inc/pvz.h"
#include <stdlib.h>
// 确保每次只对一个对象产生作用
bool applied;
#define proper() (rand() % 12 == 6)
pvz_cheat_decl(__zombies_random_callback) {
#define hypno 0x1
#define fly 0x100
#define ice 0x10000
#define butter 0x1000000
  if (applied && !proper())
    return;
  uint32_t flags = getU32(fromPtr(remote, "zombies_flags"));
  // 已被魅惑的僵尸
  if (flags & hypno)
    return;
  enum {
    BUTTER,
    ICE,
    HYPNO,
    END,
  };
  switch (rand() % END) {
  case BUTTER:
    setI32(fromPtr(remote, "zombies_butter"), 500);
    break;
  case ICE:
    setI32(fromPtr(remote, "zombies_ice"), 500);
    break;
  case HYPNO:
    setI32(fromPtr(remote, "zombies_flags"), flags | hypno);
    break;
  default:;
  }
  applied = true;
}
pvz_cheat_decl(__plants_random_callback) {
  if (applied && !proper())
    return;
  applied = true;
}
pvz_cheat_decl(__vases_random_callback) {
  if (applied && !proper())
    return;
  switch (getI32(fromPtr(remote, "vases_type"))) {
    case 1:
      setI32(fromPtr(remote, "vases_plant_code"), rand() % 48);
      break;
    case 2:
      setI32(fromPtr(remote, "vases_zombie_code"), rand() % 25);
      break;
  }
  setI32(fromPtr(remote, "vases_vis"), 500);
  applied = true;
}
extern void forEachValue(cheat_function, const char *, const char *);
void produce_effects(void) {
  enum {
    PLANTS,
    ZOMBIES,
    VASE,
    END,
  };
  applied = false;
  switch (rand() % END) {
  case PLANTS:
    forEachPlants(__plants_random_callback);
    break;
  case ZOMBIES:
    forEachZombies(__zombies_random_callback);
    break;
  case VASE:
    forEachValue(__vases_random_callback, "vases_count", "vases_entry");
  default:;
  }
}
pvz_cheat_decl(randomEffects) {
  produce_effects();
  sleep(5);
}
