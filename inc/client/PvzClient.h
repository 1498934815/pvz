/*
 * File    : inc/client/PvzClient.h
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-01-28
 * Module  :
 * License : MIT
 */
#ifndef INC_CLIENT_PVZCLIENT_H
#define INC_CLIENT_PVZCLIENT_H
#include "common/PvzCommon.h"
#include "common/common.h"
#include "common/communicator.h"
#include <vector>
class PvzClient : public Singleton<PvzClient>,
                  public Communicator,
                  public PvzCommon {
public:
  PvzClient(const char *, int);
  void printDebugInfo();
  msgPack sendBuiltinsCommand(BuiltinsCommand);
  void restartClient();
  void sendMessage(const msgPack &);
  error<int, msgPack *> recvMessage();
  std::vector<msgPack> recvMessages();
  virtual void *getBase() override;
  virtual void *getStatus() override;
  virtual void *getSaves() override;
  virtual pid_t getPid() override;
  virtual int getVersion() override;
};
#endif // INC_CLIENT_PVZCLIENT_H
