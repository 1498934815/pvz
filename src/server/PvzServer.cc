/*
 * File    : src/server/PvzServer.cc
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-01-28
 * Module  :
 * License : MIT
 */
#include <unistd.h>
#include <server/Pvz.h>
#include <server/PvzServer.h>
thread_local PvzServer *PvzServer::localInstance = nullptr;
PvzServer::PvzServer(const char *addr, int port) : Communicator(addr, port) {
  localInstance = this;
  asServer();
}
PvzServer::PvzServer(int fd) : Communicator(fd) {
  localInstance = this;
}
PvzServer *PvzServer::getLocalInstance() {
  return localInstance;
}
void *PvzServer::getBase() {
  return reinterpret_cast<void *>(0x1);
}
void *PvzServer::getStatus() {
  return reinterpret_cast<void *>(0x2);
}
pid_t PvzServer::getPid() {
  return getpid();
}
int PvzServer::getVersion() {
  return LOCAL_VERSION;
}
void PvzServer::handleBuiltinsCommand(msgPack *pack) {
  intptr_t commandsMap[] = {
      [BuiltinsCommand::GETVERSION] = (intptr_t)getVersion(),
      [BuiltinsCommand::GETPID] = (intptr_t)getPid(),
      [BuiltinsCommand::GETBASE] = (intptr_t)getBase(),
      [BuiltinsCommand::GETSTATUS] = (intptr_t)getStatus(),
  };
  sendMessage(makeMsgPack(commandsMap[pack->id]));
}
