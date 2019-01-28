/*
 * File    : src/server/PvzServer.cc
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-01-28
 * Module  :
 * License : MIT
 */
#include <server/PvzServer.h>
PvzServer::PvzServer(const char *addr, int port) : Communicator(addr, port) {
  asServer();
}
PvzServer::PvzServer(int fd) : Communicator(fd) {}
void PvzServer::fetchInfos() {}
