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
  COMMAND = 2,
};
struct msgPack {
  enum msgFlag flags;
  union {
    int id;
    void *ptr;
    int_least64_t _;
  };
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

template <typename Ty>
msgPack makeMsgPack(Ty val, const char *msg = nullptr,
                    msgFlag flags = msgFlag::NONE) {
  msgPack pack = {
      .flags = flags,
  };
  error<>(sizeof(val) > sizeof(msgPack::_))
      .except(true, "sizeof(val) > sizeof(msgPack::_)");
  memcpy(&pack._, &val, sizeof(val));
  if (msg != nullptr) {
    assert(strlen(msg) < sizeof(msgPack::msg) || !"Out of buffer size");
    strcpy(pack.msg, msg);
  }
  return pack;
}
#endif // __COMMUNICATOR__H
