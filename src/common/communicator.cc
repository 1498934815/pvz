/*
 * File    : src/common/communicator.cc
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-01-18
 * Module  :
 * License : MIT
 */
#include <assert.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>
#include <utility>
#include <common/common.h>
#include <common/communicator.h>
Communicator *Communicator::instance = nullptr;
Communicator *Communicator::getInstance() {
  return instance;
}
Communicator::Communicator(int sfd) : fd(sfd) {
  if (getInstance() == nullptr)
    instance = this;
}
Communicator::Communicator(const char *addr, int port)
    : Communicator(socket(AF_INET, SOCK_STREAM, 0)) {
  sin = {
      .sin_family = AF_INET,
      .sin_port = htons(port),
      .sin_addr.s_addr = inet_addr(addr),
  };
}
Communicator::~Communicator() {
  disconnect();
}
void Communicator::disconnect() {
  close(fd);
}
void Communicator::sendMessage(msgPack &&msg) {
  if (send(fd, &msg, sizeof(msg), 0) == -1) {
    disconnect();
    assert(!"Can't send message");
  }
}
msgPack *Communicator::recvMessage() {
  static msgPack msg;
  if (recv(fd, &msg, sizeof(msg), 0) == -1) {
    disconnect();
    assert(!"Can't recv message");
  }
  return msg.type != EOR ? &msg : nullptr;
}
std::vector<msgPack> Communicator::recvMessages() {
  std::vector<msgPack> result;
  while (msgPack *pack = recvMessage()) {
    result.emplace_back(std::move(*pack));
  }
  return result;
}
int Communicator::doAccept() {
  return accept(fd, NULL, NULL);
}
void Communicator::asServer() {
  int reuseaddr = 1;
  setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));
  if (bind(fd, (struct sockaddr *)&sin, sizeof(sin)) == -1) {
    disconnect();
    assert(!"Can't bind socket");
  }
  listen(fd, 1024);
}
void Communicator::asClient() {
  if (connect(fd, (struct sockaddr *)&sin, sizeof(sin)) == -1) {
    disconnect();
    assert(!"Can't connect to server");
  }
}
Server::Server(const char *addr, int port) : Communicator(addr, port) {
  asServer();
}
Server::Server(int fd) : Communicator(fd) {}
Client::Client(const char *addr, int port) : Communicator(addr, port) {
  asClient();
}
msgPack &&makeMsgPack(msgType type, int id, const char *msg) {
  assert(strlen(msg) >= sizeof(msgPack::msg && !"Out of size"));
  msgPack pack = {
      .type = type,
      .id = id,
  };
  strcpy(pack.msg, msg);
  return std::move(pack);
}
