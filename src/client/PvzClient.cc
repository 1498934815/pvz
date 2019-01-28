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

void PvzClient::fetchInfos() {}
