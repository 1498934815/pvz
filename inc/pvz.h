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
#include <setjmp.h>
#include "../inc/utils.h"
#define SPECIFIC_PACKAGE "com.popcap.pvz_na"
#define SPECIFIC_DYNAMIC_LIBRARIES "libpvz.so"
#define LIBPVZ_BASE_HELPER "Java_com_popcap_pvz_1na_PvZActivity_onPVZCreate"
#define LIBPVZ_BSS_OFF (0xc5d860)
#define PUMPKIN_CODE (30)
#define LILYPAD_CODE (16)
#define LADDER_CODE (21)
#define RED_CODE (32)
#define GARGANTUAR_CODE (23)

enum pvz_field {
  DAY,
  NIGHT,
  POOL,
  FOG,
  ROOF,
  MOONNIGHT,
};
struct {
  void *base;
  void *bss;
  int sock;
  int32_t val;
  pid_t pid;
  struct task *task;
} info;
extern jmp_buf env;

void pvz_write(void *, void *, size_t);
void pvz_read(void *, void *, size_t);
#define DEFINE_GET(type, name) type get##name(void *remote)
#define DEFINE_SET(type, name) void set##name(void *remote, type val)
DEFINE_GET(int32_t, I32);
DEFINE_GET(float, F32);
DEFINE_GET(void *, P32);
DEFINE_SET(int32_t, I32);
DEFINE_SET(float, F32);

void initBase(void);
void initDynamicBase(void);
void initBssBase(void);
void doInit(void);
#endif //__PVZ__H
