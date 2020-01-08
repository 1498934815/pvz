/*
 * File    : src/common/communicator.cc
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-01-18
 * Module  :
 * License : MIT
 */
#include "common/communicator.h"
#include "common/common.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
Communicator::Communicator(int sfd) : fd(sfd) {}
Communicator::Communicator(const char *addr, int port)
    : Communicator(startSocket()) {
  sin = {
      .sin_family = AF_INET,
      .sin_port = htons(port),
      .sin_addr.s_addr = inet_addr(addr),
  };
}
Communicator::~Communicator() {
  disconnect();
}
int Communicator::startSocket() {
  return fd = socket(AF_INET, SOCK_STREAM, 0);
}
void Communicator::disconnect() {
  if (fd != 0) {
    close(fd);
    fd = 0;
  } else {
    error<>(true).except(true, "socket was already closed");
  }
}
error<> Communicator::sendMessage(const msgPack &msg) {
  return error<>(send(fd, &msg, sizeof(msg), 0))
      .except(-1, "Can't send message");
}
void Communicator::sendEOR() {
  sendMessage(makeMsgPack(0, nullptr, msgStatus::EOR));
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
    if (pack->status == msgStatus::EOR)
      break;
    result.emplace_back(std::move(*pack));
  }
  return result;
}
error<int> Communicator::doAccept() {
  return error<int>(accept(fd, NULL, NULL)).except(-1, "Can't accept");
}
void Communicator::asServer() {
  int reuseaddr = 1;
  setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));
  error<>(bind(fd, (struct sockaddr *)&sin, sizeof(sin)))
      .except(-1, "Can't bind to server");
  listen(fd, 1024);
}
