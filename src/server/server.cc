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
static pthread_t tid;
void *__server_process(void *pfd) {
  Communicator server(*reinterpret_cast<int *>(pfd));
  while (msgPack *pack = server.recvMessage()) {
    DEBUG_LOG("%d %d %s", pack->type, pack->id, pack->msg);
    server.sendMessage(makeMsgPack(STRING, 0, "REPLY 0"));
    server.sendMessage(makeMsgPack(STRING, 1, "REPLY 1"));
    server.sendMessage(makeMsgPack(EOR, 0, "EOR"));
  }
  pthread_exit(nullptr);
}
void *__server_main(void *) {
  Server server(SERVER_ADDR, SERVER_PORT);
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
  Communicator::getInstance()->disconnect();
  pthread_join(tid, nullptr);
}
