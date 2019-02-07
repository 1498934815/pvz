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
#include <common/options.h>
#include <common/communicator.h>
#include <server/PvzServer.h>
static pthread_t tid;
void handleClientCommand(msgPack *pack) {
  PvzServer *server = PvzServer::getLocalInstance();
  if (pack->flags == msgFlag::COMMAND) {
    DEBUG_LOG("HANDING COMMAND");
    server->handleBuiltinsCommand(pack);
  } else {
    auto *o = Options::getInstance()->getOption(pack->id);
    DEBUG_LOG("GOT %s", o->name);
  }
  server->sendMessage(makeMsgPack(0, nullptr, msgFlag::EOR));
}
void *__server_process(void *pfd) {
  PvzServer server(*reinterpret_cast<int *>(pfd));
  while (msgPack *pack = server.recvMessage().getValue()) {
    if (pack->flags == msgFlag::EOR)
      continue;
    handleClientCommand(pack);
  }
  DEBUG_LOG("DISCONNECT");
  pthread_exit(nullptr);
}
void *__server_main(void *) {
  PvzServer server(SERVER_ADDR, SERVER_PORT);
  // Initialize options
  Options option;
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
