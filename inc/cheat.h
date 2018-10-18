/*
 * File    : inc/cheat.h
 * Project : PVZ
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2018-04-12
 * Module  :
 * License : MIT
 */
#ifndef __CHEAT__H
#define __CHEAT__H
#include "../inc/defs.h"

void forEachPlants(cheat_function);
void forEachZombies(cheat_function);
void *getSaves(void);
void *getField(void);
void *getStatus(void);

void *fromPtr(void *, const char *);
#define fromField(name) fromPtr(getField(), name)
#define fromStatus(name) fromPtr(getStatus(), name)
#define fromSaves(name) fromPtr(getSaves(), name)

pvz_cheat(coverZombies);
pvz_cheat(putLadder);
pvz_cheat(fuck_LilyPad_Pumpkin);
pvz_cheat(setSun);
pvz_cheat(pass);
pvz_cheat(setFlags);
pvz_cheat(doLimits);
pvz_cheat(callLadder);
pvz_cheat(callGargantuar);
pvz_cheat(freePlants);
pvz_cheat(shutdownFreePlants);
pvz_cheat(switchMode);
pvz_cheat(switchFieldType);
pvz_cheat(changeCoins);
pvz_cheat(jump);
pvz_cheat(changeCardCode);
pvz_cheat(pass2life);
pvz_cheat(autoCollect);
pvz_cheat(triggerMowers);
pvz_cheat(randomEffects);
#endif //__CHEAT__H
