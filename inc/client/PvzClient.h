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
class PvzClient : public Singleton<PvzClient>, public Communicator {
public:
  PvzClient(const char *, int);
  void printDebugInfo();
  std::vector<msgPack> sendBuiltinsCommandEx(BuiltinsCommand);
  msgPack sendBuiltinsCommand(BuiltinsCommand);
  void asClient();
  void restartClient();
  void sendMessage(const msgPack &);
  error<int, msgPack *> recvMessage();
  std::vector<msgPack> recvMessages();
  void *getBase();
  void *getStatus();
  void *getSaves();
  void *getCoreLib();
  pid_t getPid();
  int getVersion();
  int getWave();
  int getTotalHitpoint();
  int getHitpointBoundary();
  int getTotalFreshCountdown();
  int getFreshCountdown();
};
#endif // INC_CLIENT_PVZCLIENT_H
