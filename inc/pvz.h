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
#define SPECIFIC_PACKAGE "com.popcap.pvz_na"
#define SPECIFIC_DYNAMIC_LIBRARIES "libpvz.so"
#define LIBPVZ_BASE_HELPER "Java_com_popcap_pvz_1na_PvZActivity_onPVZCreate"
#define LIBPVZ_BSS_OFF (0xc5d860)
#define PUMPKIN_CODE (30)
#define LILYPAD_CODE (16)
#define LADDER_CODE (21)

extern jmp_buf env;
void getDynamicBase();
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
