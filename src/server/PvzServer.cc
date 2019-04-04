/*
 * File    : src/server/PvzServer.cc
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-01-28
 * Module  :
 * License : MIT
 */
#include <server/Pvz.h>
#include <server/PvzServer.h>
#include <unistd.h>
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
  return __getBase();
}
void *PvzServer::getStatus() {
  return __getStatus();
}
void *PvzServer::getSaves() {
  return __getSaves();
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
      [BuiltinsCommand::GETSAVES] = (intptr_t)getSaves(),
  };
  sendMessage(makeMsgPack(commandsMap[pack->val]));
}
