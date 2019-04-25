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
  uierror("Trying reconnect to server");
  disconnect();
  startSocket();
  asClient();
}
void PvzClient::sendMessage(const msgPack &msg) {
  if (Communicator::sendMessage(msg) == -1) {
    restartClient();
    Communicator::sendMessage(msg);
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
  if (msgs.empty())
    return makeMsgPack(0, nullptr);
  return msgs.front();
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
pid_t PvzClient::getPid() {
  return sendBuiltinsCommand(BuiltinsCommand::GETPID).val;
}
int PvzClient::getVersion() {
  return sendBuiltinsCommand(BuiltinsCommand::GETVERSION).val;
}

void PvzClient::printDebugInfo() {
  uinoticef("PID:%d 基址:%p 游戏状态入口:%p 用户信息入口:%p\n", getPid(),
            getBase(), getStatus(), getSaves());
}
