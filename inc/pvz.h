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
#ifndef SPECIFIC_PACKAGE
#define SPECIFIC_PACKAGE "com.popcap.pvz_na"
#endif
#ifndef SPECIFIC_DYNAMIC_LIBRARIES
#define SPECIFIC_DYNAMIC_LIBRARIES "libpvz.so"
#endif
#define PUMPKIN_CODE (30)
#define LILYPAD_CODE (16)
#define LADDER_CODE (21)
struct Hp {
  uint32_t curHp;
  uint32_t totalHp;
  uint32_t armor;
};
extern jmp_buf env;
void *getBase(const char *, int, void (*)(void *, void *), void **);
void *getDynamicBase();
void getBssBase();
void pvz_write(void *, void *, size_t);
void pvz_read(void *, void *, size_t);
int32_t getI32(void *);
void *getP32(void *);
float getF32(void *);
void setI32(void *, int32_t);
void setF32(void *, float);
void doInit();
#endif //__PVZ__H
