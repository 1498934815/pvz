/*
 * File    : src/client/PvzClient.cc
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-01-28
 * Module  :
 * License : MIT
 */
#include "client/PvzClient.h"
PvzClient::PvzClient(const char *addr, int port) : Communicator(addr, port) {
  asClient();
}
void PvzClient::restartClient() {
  uinotice("游戏已退出,尝试重新寻找进程....");
  disconnect();
  startSocket();
  asClient();
  uinotice("成功找到新进程");
}
void PvzClient::asClient() {
  error<>(connect(fd, (struct sockaddr *)&sin, sizeof(sin)))
      .except(-1, "找不到可用的进程!必须启动配套的PvZ客户端打开本程序");
}
void PvzClient::sendMessage(const msgPack &msg) {
  if (Communicator::sendMessage(msg) == -1) {
    restartClient();
    sendMessage(msg);
  }
}
error<int, msgPack *> PvzClient::recvMessage() {
  auto &&ret = Communicator::recvMessage();
  if (ret == 0) {
    restartClient();
    return error<int, msgPack *>(0);
  }
  return ret;
}
std::vector<msgPack> PvzClient::recvMessages() {
  std::vector<msgPack> result;
  while (msgPack *pack = recvMessage().getValue()) {
    if (pack->status == msgStatus::EOR)
      break;
    result.emplace_back(std::move(*pack));
  }
  return result;
}
msgPack PvzClient::sendBuiltinsCommand(BuiltinsCommand command) {
  this->sendMessage(makeMsgPack(command, nullptr, msgStatus::COMMAND));
  auto &&msgs = this->recvMessages();
  return msgs.empty() ? makeMsgPack(0) : msgs.front();
}
void *PvzClient::getBase() {
  return sendBuiltinsCommand(BuiltinsCommand::GETBASE).ptr;
}
void *PvzClient::getStatus() {
  return sendBuiltinsCommand(BuiltinsCommand::GETSTATUS).ptr;
}
void *PvzClient::getSaves() {
  return sendBuiltinsCommand(BuiltinsCommand::GETSAVES).ptr;
}
void *PvzClient::getCoreLib() {
  return sendBuiltinsCommand(BuiltinsCommand::GETCORELIB).ptr;
}
pid_t PvzClient::getPid() {
  return sendBuiltinsCommand(BuiltinsCommand::GETPID).val;
}
int PvzClient::getVersion() {
  return sendBuiltinsCommand(BuiltinsCommand::GETVERSION).val;
}
int PvzClient::getWave() {
  return sendBuiltinsCommand(BuiltinsCommand::GETWAVE).val;
}
int PvzClient::getTotalHitpoint() {
  return sendBuiltinsCommand(BuiltinsCommand::GET_TOTAL_HITPOINT).val;
}
int PvzClient::getHitpointBoundary() {
  return sendBuiltinsCommand(BuiltinsCommand::GET_HITPOINT_BOUNDARY).val;
}
int PvzClient::getTotalFreshCountdown() {
  return sendBuiltinsCommand(BuiltinsCommand::GET_TOTAL_FRESH_COUNTDOWN).val;
}
int PvzClient::getFreshCountdown() {
  return sendBuiltinsCommand(BuiltinsCommand::GET_FRESH_COUNTDOWN).val;
}

void PvzClient::printDebugInfo() {
  void *status = getStatus();
#define STATICLY_GAME_INFO_TITLE "静态游戏信息 "
#define DYNAMICLY_GAME_INFO_TITLE "动态游戏信息 "
  uinoticef(STATICLY_GAME_INFO_TITLE
            "PID:%d 核心库:%p 基址:%p 用户信息入口:%p\n",
            getPid(), getCoreLib(), getBase(), getSaves());
  if (status != nullptr) {
    uinoticef(DYNAMICLY_GAME_INFO_TITLE "游戏中状态入口:%p 当前波数:%d\n",
              status, getWave());
    uinoticef(DYNAMICLY_GAME_INFO_TITLE "本波总血量:%d 达到%d后刷新\n",
              getTotalHitpoint(), getHitpointBoundary());
    uinoticef(DYNAMICLY_GAME_INFO_TITLE "本波总倒计时:%dcs 刷新倒计时:%dcs\n",
              getTotalFreshCountdown(), getFreshCountdown());
  } else
    uinoticef(DYNAMICLY_GAME_INFO_TITLE "未在游戏中\n");
}
