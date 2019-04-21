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
#include "common/common.h"
#include <pthread.h>
#include <unistd.h>

class PvzCommon {
public:
  virtual void *getBase() = 0;
  virtual void *getStatus() = 0;
  virtual void *getSaves() = 0;
  virtual pid_t getPid() = 0;
  virtual int getVersion() = 0;
};
enum BuiltinsCommand {
  GETVERSION,
  GETPID,
  GETBASE,
  GETSTATUS,
  GETSAVES,
};

enum fieldTypes {
  DAY,
  NIGHT,
  POOL,
  FOG,
  ROOF,
  MOONNIGHT,
  MUSHROOM_GARDEN,
  GARDEN,
};

struct PvzPoint {
  int x;
  int y;
};

class Communicator;
typedef void (*normal_callback)(Communicator *, msgPack *);
typedef void (*object_callback)(Communicator *, void *);
typedef void (*daemon_callback)(Communicator *);
#endif // INC_COMMON_PVZCOMMON_H
