/*
 * File    : inc/server/PvzServer.h
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-01-28
 * Module  :
 * License : MIT
 */
#ifndef INC_SERVER_PVZSERVER_H
#define INC_SERVER_PVZSERVER_H
#include "common/PvzCommon.h"
#include "common/common.h"
#include "common/communicator.h"
class PvzServer : public Singleton<PvzServer>, public Communicator {
  static thread_local PvzServer *localInstance;

public:
  PvzServer(const char *, int);
  PvzServer(int);
  void handleBuiltinsCommand(msgPack *);
  static PvzServer *getLocalInstance();
};

#endif // INC_SERVER_PVZSERVER_H
