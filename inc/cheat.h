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
void letZombiesFragile(void *);
void coverZombies(void *);
void increaseZombies(void *);
void increaseCabbagePult();
void forEachPlants(void (*)(void *));
void forEachZombies(void (*)(void *));
void increasePlants(void *);
void increasePlantsAttack(void *);
void putLadder(void *remote);
void reportPlants(void *);
void fuck_LilyPad_Pumpkin(void *);
void plants_freeze(void *);
void plants_attack(void *);
void setSun();
void pass();
void setFlags();
void doLimits();
void callLadder();
void freePlants();
void *getField();
void *getStatus();
#endif //__CHEAT__H
