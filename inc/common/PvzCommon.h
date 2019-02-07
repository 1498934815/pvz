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
  virtual void *getBase() = 0;
  virtual void *getStatus() = 0;
  virtual pid_t getPid() = 0;
  virtual int getVersion() = 0;
};
enum BuiltinsCommand {
  GETVERSION,
  GETPID,
  GETBASE,
  GETSTATUS,
};
#endif // __PVZCOMMON__H
