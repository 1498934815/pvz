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
#include <common/common.h>
#include <common/communicator.h>
Communicator::Communicator(int sfd) : fd(sfd) {}
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
  error<>(send(fd, &msg, sizeof(msg), 0))
      .when(0, 0)
      .except(-1, "Can't send message");
}
error<int, msgPack *> Communicator::recvMessage() {
  static msgPack msg;
  return error<int, msgPack *>(recv(fd, &msg, sizeof(msg), 0), &msg)
      .when(0, nullptr)
      .except(-1, "Can't recv message");
}
std::vector<msgPack> Communicator::recvMessages() {
  std::vector<msgPack> result;
  while (msgPack *pack = recvMessage().getValue()) {
    if (pack->flags == msgFlag::EOR)
      break;
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
  error<>(bind(fd, (struct sockaddr *)&sin, sizeof(sin)))
      .except(-1, "Can't bind to server");
  listen(fd, 1024);
}
void Communicator::asClient() {
  error<>(connect(fd, (struct sockaddr *)&sin, sizeof(sin)))
      .except(-1, "Can't connect to server");
}

msgPack makeMsgPack(int id, const char *msg, msgFlag flags) {
  msgPack pack = {
      .flags = flags,
      .id = id,
  };
  if (msg != nullptr) {
    assert(strlen(msg) < sizeof(msgPack::msg) || !"Out of buffer size");
    strcpy(pack.msg, msg);
  }
  return pack;
}
