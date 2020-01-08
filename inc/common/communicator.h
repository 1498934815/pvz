/*
 * File    : inc/common/communicator.h
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-01-18
 * Module  :
 * License : MIT
 */
#ifndef INC_COMMON_COMMUNICATOR_H
#define INC_COMMON_COMMUNICATOR_H
#include "common/common.h"
#include <arpa/inet.h>
#include <assert.h>
#include <sys/socket.h>
#include <vector>
class Communicator {
public:
  int fd;
  struct sockaddr_in sin;
  Communicator(const char *, int);
  Communicator(int);
  ~Communicator();
  void asServer();
  void sendEOR();
  int startSocket();
  error<> sendMessage(const msgPack &);
  error<int, msgPack *> recvMessage();
  std::vector<msgPack> recvMessages();
  error<int> doAccept();
  void disconnect();
};
#endif // INC_COMMON_COMMUNICATOR_H
