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
void coverZombies(void *);
void forEachPlants(void (*)(void *));
void forEachZombies(void (*)(void *));
void putLadder(void *remote);
void fuck_LilyPad_Pumpkin(void *);
void setSun();
void pass();
void setFlags();
void doLimits();
void callLadder();
void freePlants();
void switchMode();
void moveSaves();
void changeCoins();
void jump();
void changeCardCode();
void pass2life();
void *getSaves();
void *getField();
void *getStatus();
#endif //__CHEAT__H
