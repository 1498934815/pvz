/*
 * File    : server.cc
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-01-18
 * Module  :
 * License : MIT
 */
#include <signal.h>
#include <pthread.h>
#include <deque>
#include <common/common.h>
#include <common/options.h>
#include <common/communicator.h>
#include <server/PvzServer.h>
static pthread_t tid;
static std::deque<pthread_t> *threads;
#define INTERRUPT_SIGNAL SIGUSR1
#define INTERRUPT_SIGHANDLER SIGHANDLER
void SIGHANDLER(int) {}
void registerSignalMask() {
  sigset_t set;
  sigfillset(&set);
  sigdelset(&set, INTERRUPT_SIGNAL);
  pthread_sigmask(SIG_SETMASK, &set, nullptr);
  struct sigaction action = {
      .sa_handler = INTERRUPT_SIGHANDLER,
  };
  sigaction(INTERRUPT_SIGNAL, &action, nullptr);
}
void interruptAllThread() {
  for (auto tid : *threads) {
    pthread_kill(tid, INTERRUPT_SIGNAL);
    pthread_join(tid, nullptr);
  }
  delete threads;
}
void handleClientCommand(msgPack *pack) {
  PvzServer *server = PvzServer::getLocalInstance();
  if (pack->flags == msgFlag::COMMAND) {
    server->handleBuiltinsCommand(pack);
  } else {
    auto *o = Options::getInstance()->getOption(pack->id);
    DEBUG_LOG("GOT %s", o->name);
  }
  server->sendMessage(makeMsgPack(0, nullptr, msgFlag::EOR));
}
void *__server_process(void *pfd) {
  registerSignalMask();
  PvzServer server(*reinterpret_cast<int *>(pfd));
  while (msgPack *pack = server.recvMessage().getValue()) {
    if (pack->flags == msgFlag::EOR)
      continue;
    handleClientCommand(pack);
  }
  DEBUG_LOG("CONNECTION CLOSED");
  server.disconnect();
  pthread_exit(nullptr);
}
void *__server_main(void *) {
  registerSignalMask();
  PvzServer server(SERVER_ADDR, SERVER_PORT);
  // Initialize options
  Options option;
  int csock;
  while ((csock = server.doAccept()) != -1) {
    DEBUG_LOG("GONNA A CLIENT");
    pthread_create(&tid, nullptr, __server_process,
                   reinterpret_cast<void *>(&csock));
    threads->push_front(tid);
  }
  DEBUG_LOG("MAIN SERVER CLOSED");
  server.disconnect();
  pthread_exit(nullptr);
}
extern "C" void __attribute__((constructor)) __server_main_invocation() {
  DEBUG_LOG("INVOCATION");
  pthread_create(&tid, nullptr, __server_main, nullptr);
  // XXX We create a deque<pthread_t> on heaps
  // because if it was a global variable
  // it will free automaticly
  threads = new std::deque<pthread_t>();
  threads->push_front(tid);
}
extern "C" void __attribute__((destructor)) __server_cleanup() {
  DEBUG_LOG("CLEANUP");
  interruptAllThread();
}
