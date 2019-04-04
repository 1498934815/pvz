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
#include <arpa/inet.h>
#include <assert.h>
#include <common/common.h>
#include <sys/socket.h>
#include <vector>
class Communicator {
  int fd;
  struct sockaddr_in sin;

public:
  Communicator(const char *, int);
  Communicator(int);
  ~Communicator();
  void asServer();
  void asClient();
  void sendMessage(msgPack &&);
  void sendEOR();
  error<int, msgPack *> recvMessage();
  error<int> doAccept();
  std::vector<msgPack> recvMessages();
  void disconnect();
};
#endif // INC_COMMON_COMMUNICATOR_H
