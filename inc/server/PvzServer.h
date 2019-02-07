/*
 * File    : inc/server/PvzServer.h
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-01-28
 * Module  :
 * License : MIT
 */
#ifndef __PVZSERVER__H
#define __PVZSERVER__H
#include <common/common.h>
#include <common/communicator.h>
#include <common/PvzCommon.h>
class PvzServer : public Singleton<PvzServer>,
                  public Communicator,
                  public PvzCommon {
  static thread_local PvzServer *localInstance;

public:
  PvzServer(const char *, int);
  PvzServer(int);
  void handleBuiltinsCommand(msgPack *);
  static PvzServer *getLocalInstance();
  virtual void *getBase() override;
  virtual void *getStatus() override;
  virtual pid_t getPid() override;
  virtual int getVersion() override;
};

#endif // __PVZSERVER__H
