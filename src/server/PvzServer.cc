/*
 * File    : src/server/PvzServer.cc
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-01-28
 * Module  :
 * License : MIT
 */
#include "server/PvzServer.h"
#include "server/Pvz.h"
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
void PvzServer::handleBuiltinsCommand(msgPack *pack) {
  intptr_t val;
  switch (pack->val) {
  case BuiltinsCommand::GETVERSION:
    val = (intptr_t)LOCAL_VERSION;
    break;
  case BuiltinsCommand::GETPID:
    val = (intptr_t)getpid();
    break;
  case BuiltinsCommand::GETBASE:
    val = (intptr_t)__getBase();
    break;
  case BuiltinsCommand::GETSTATUS:
    val = (intptr_t)__getStatus();
    break;
  case BuiltinsCommand::GETSAVES:
    val = (intptr_t)__getSaves();
    break;
  case BuiltinsCommand::GETCORELIB:
    val = (intptr_t)__getCoreLib();
    break;
  case BuiltinsCommand::GETWAVE:
    val = (intptr_t)getI32(incrStatus(OFF_CURRENT_WAVE));
    break;
  case BuiltinsCommand::GET_TOTAL_HITPOINT:
    val = (intptr_t)getI32(incrStatus(OFF_WAVE_HITPOINT_TOTAL));
    break;
  case BuiltinsCommand::GET_HITPOINT_BOUNDARY:
    val = (intptr_t)getI32(incrStatus(OFF_WAVE_HITPOINT_BOUNDARY));
    break;
  case BuiltinsCommand::GET_TOTAL_FRESH_COUNTDOWN:
    val = (intptr_t)getI32(incrStatus(OFF_FRESH_COUNTDOWN_TOTAL));
    break;
  case BuiltinsCommand::GET_FRESH_COUNTDOWN:
    val = (intptr_t)getI32(incrStatus(OFF_FRESH_COUNTDOWN_REMAIN));
    break;
  }
  sendMessage(makeMsgPack(val));
}
