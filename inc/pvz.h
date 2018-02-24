/*
 * File    : pvz.h
 * Project : PVZ
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2017-08-15
 * Module  :
 * License : MIT
 */
#ifndef __PVZ__H
#define __PVZ__H
#include <sys/cdefs.h>
#include <pthread.h>
#include <setjmp.h>
#ifndef SPECIFIC_PACKAGE
#define SPECIFIC_PACKAGE "com.popcap.pvz_na"
#endif
#ifndef SPECIFIC_DYNAMIC_LIBRARIES
#define SPECIFIC_DYNAMIC_LIBRARIES "libpvz.so"
#endif
#define COINS_HELPER_BUFF 256
#define COINS_HELPER_OFF 0x7aa300
#define CARDS_PLAN_TPYE_OFF 0x8
#define PUMPKIN_CODE (30)
#define LILYPAD_CODE (16)
struct {
  size_t curHp;
  size_t totalHp;
  size_t armor;
} Hp;
struct {
  size_t newHp;
  size_t totalHp;
  size_t armor;
} newHp;
jmp_buf env;
pthread_mutex_t mutex;
#endif //__PVZ__H
