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
#include <assert.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>
enum class msgFlag : unsigned int {
  NONE,
  EOR,
  COMMAND,
};
struct msgPack {
  enum msgFlag flags;
  unsigned id;
  union {
    int val;
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
  void sendEOR();
  error<int, msgPack *> recvMessage();
  error<int> doAccept();
  std::vector<msgPack> recvMessages();
  void disconnect();
};

template <typename Ty>
msgPack makeMsgPack(unsigned id, Ty val, const char *msg = nullptr,
                    msgFlag flags = msgFlag::NONE) {
  msgPack pack = {
      .id = id,
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
template <typename Ty>
msgPack makeMsgPack(Ty val, const char *msg = nullptr,
                    msgFlag flags = msgFlag::NONE) {
  return makeMsgPack(0, val, msg, flags);
}
#endif // __COMMUNICATOR__H
