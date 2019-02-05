/*
 * File    : server.cc
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-01-18
 * Module  :
 * License : MIT
 */
#include <pthread.h>
#include <common/common.h>
#include <common/communicator.h>
#include <server/PvzServer.h>
static pthread_t tid;
void *__server_process(void *pfd) {
  PvzServer server(*reinterpret_cast<int *>(pfd));
  while (msgPack *pack = server.recvMessage().getValue()) {
    if (pack->flags == msgFlag::EOR)
      continue;
    DEBUG_LOG("%d %d %s", pack->flags, pack->id, pack->msg);
    server.sendMessage(makeMsgPack(0, "REPLY 0"));
    server.sendMessage(makeMsgPack(1, "REPLY 1"));
    server.sendMessage(makeMsgPack(0, nullptr, msgFlag::EOR));
  }
  DEBUG_LOG("DISCONNECT");
  pthread_exit(nullptr);
}
void *__server_main(void *) {
  PvzServer server(SERVER_ADDR, SERVER_PORT);
  int csock;
  while ((csock = server.doAccept()) != -1) {
    pthread_t tid;
    DEBUG_LOG("Gonna a client!");
    pthread_create(&tid, nullptr, __server_process,
                   reinterpret_cast<void *>(&csock));
    pthread_detach(tid);
  }
  // Maybe will not reach here
  pthread_exit(nullptr);
}
extern "C" void __attribute__((constructor)) __server_main_invocation() {
  DEBUG_LOG("Invocation");
  pthread_create(&tid, nullptr, __server_main, nullptr);
}
extern "C" void __attribute__((destructor)) __server_cleanup() {
  DEBUG_LOG("CleanUp");
  PvzServer::getInstance()->disconnect();
  pthread_join(tid, nullptr);
}
