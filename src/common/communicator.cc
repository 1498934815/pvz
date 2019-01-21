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
#include <common/communicator.h>
Socket::Socket(const char *addr, int port) {
  fd = socket(AF_INET, SOCK_STREAM, 0);
  sin = {
      .sin_family = AF_INET,
      .sin_port = htons(port),
      .sin_addr.s_addr = inet_addr(addr),
  };
}
Socket::~Socket() {
  disconnect();
}
void Socket::disconnect() {
  close(fd);
}
void Socket::asServer() {
  int reuseaddr = 1;
  setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));
  if (bind(fd, (struct sockaddr *)&sin, sizeof(sin)) == -1) {
    disconnect();
    assert(!"Can't bind socket");
  }
  listen(fd, 1024);
}
void Socket::asClient() {
  if (connect(fd, (struct sockaddr *)&sin, sizeof(sin)) == -1) {
    disconnect();
    assert(!"Can't connect to server");
  }
}
Communicator::Communicator(const char *addr, int port, Communicator::Tag tag):socket(addr, port) {
  switch (tag) {
    case Tag::Server:
      socket.asServer();
      break;
    case Tag::Client:
      socket.asClient();
      break;
  }
  instance = this;
}
void Communicator::disconnect() {
  socket.disconnect();
}
Communicator *Communicator::instance = nullptr;
Communicator *Communicator::getInstance() {
  return instance;
}
