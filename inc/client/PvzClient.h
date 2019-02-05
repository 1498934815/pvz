/*
 * File    : inc/client/PvzClient.h
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-01-28
 * Module  :
 * License : MIT
 */
#ifndef __PVZCLIENT__H
#define __PVZCLIENT__H
#include <common/common.h>
#include <common/PvzCommon.h>
#include <common/communicator.h>
class PvzClient : public Singleton<PvzClient>,
                  public Communicator,
                  public PvzCommon {
public:
  PvzClient(const char *, int);
  void printDebugInfo();
  virtual void *getBase() override;
  virtual void *getStatus() override;
  virtual pid_t getPid() override;
};
#endif // __PVZCLIENT__H
