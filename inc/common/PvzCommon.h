/*
 * File    : inc/common/PvzCommon.h
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-01-28
 * Module  :
 * License : MIT
 */
#ifndef INC_COMMON_PVZCOMMON_H
#define INC_COMMON_PVZCOMMON_H
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
#endif // INC_COMMON_PVZCOMMON_H
