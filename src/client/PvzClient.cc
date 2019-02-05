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

void *PvzClient::getBase() {
  return nullptr;
}
void *PvzClient::getStatus() {
  return nullptr;
}
pid_t PvzClient::getPid() {
  return 999;
}

void PvzClient::printDebugInfo() {
  uinoticef("PID:%d 基址:%p 状态与信息:%p\n", getPid(), getBase(), getStatus());
}
