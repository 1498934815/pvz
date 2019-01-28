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
public:
  PvzServer(const char *, int);
  PvzServer(int);
  virtual void fetchInfos() override;
};

#endif // __PVZSERVER__H
