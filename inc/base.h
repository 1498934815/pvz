/*
 * File    : base.h
 * Project : PVZ
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2017-08-15
 * Module  :
 * License : MIT
 */
#ifndef __BASE__H
#define __BASE__H
#include <stdint.h>
#include "utils.h"
struct {
  void *base;
  void *bss;
  int sock;
  int32_t val;
  pid_t pid;
  __task *task;
  __images *images;
} baseInfo;
void initBase();
#endif //__BASE__H
