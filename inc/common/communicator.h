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
enum msgType {
  STRING = 1,
  ID = 2,
  EOR = 4,
};
struct msgPack {
  msgType type;
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

msgPack makeMsgPack(msgType, int, const char *);
#endif // __COMMUNICATOR__H
