/*
 * File    : inc/common/communicator.h
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-01-18
 * Module  :
 * License : MIT
 */
#ifndef __COMMUNICATOR__H
#define __COMMUNICATOR__H
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>
enum class msgFlag : unsigned int {
  NONE,
  EOR = 1,
};
struct msgPack {
  enum msgFlag flags;
  int id;
  char msg[128];
};
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
  error<int, msgPack *> recvMessage();
  std::vector<msgPack> recvMessages();
  int doAccept();
  void disconnect();
};

msgPack makeMsgPack(int, const char * = nullptr, enum msgFlag = msgFlag::NONE);
#endif // __COMMUNICATOR__H
