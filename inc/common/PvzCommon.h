/*
 * File    : inc/common/PvzCommon.h
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-01-28
 * Module  :
 * License : MIT
 */
#ifndef __PVZCOMMON__H
#define __PVZCOMMON__H
#include <unistd.h>
class PvzCommon {
public:
  pid_t pid;
  void *base;
  void *status;
  virtual void fetchInfos() = 0;
};
#endif // __PVZCOMMON__H
