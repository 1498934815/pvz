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
class Socket {
  int fd;
  struct sockaddr_in sin;
  public:
  Socket(const char *, int prot);
  ~Socket();
  void asServer();
  void asClient();
  void disconnect();
};
class Communicator {
  Socket socket;
  static Communicator *instance;
  public:
  enum Tag {
    Client,
    Server,
  };
  void disconnect();
  Communicator(const char *, int, Tag);
  const char *interact(const char *, ...);
  static Communicator *getInstance();
};
#endif // __COMMUNICATOR__H
