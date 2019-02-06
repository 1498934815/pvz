/*
 * File    : src/client/PvzClient.cc
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-01-28
 * Module  :
 * License : MIT
 */
#include <client/PvzClient.h>
PvzClient::PvzClient(const char *addr, int port) : Communicator(addr, port) {
  asClient();
}
msgPack &&PvzClient::sendBuiltinsCommand(BuiltinsCommand command) {
  sendMessage(makeMsgPack(command, nullptr, msgFlag::COMMAND));
  return std::move(recvMessages().front());
}
void *PvzClient::getBase() {
  return sendBuiltinsCommand(BuiltinsCommand::GETBASE).ptr;
}
void *PvzClient::getStatus() {
  return sendBuiltinsCommand(BuiltinsCommand::GETSTATUS).ptr;
}
pid_t PvzClient::getPid() {
  return sendBuiltinsCommand(BuiltinsCommand::GETPID).id;
}

void PvzClient::printDebugInfo() {
  uinoticef("PID:%d 基址:%p 状态与信息:%p\n", getPid(), getBase(), getStatus());
}
