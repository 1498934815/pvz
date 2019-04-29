/*
 * File    : server.cc
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-01-18
 * Module  :
 * License : MIT
 */
#include "common/common.h"
#include "common/communicator.h"
#include "common/options.h"
#include "server/Pvz.h"
#include "server/PvzServer.h"
#include <deque>
#include <pthread.h>
#include <signal.h>
static std::deque<pthread_t> *threads;
#define emplace_front(container, ...)                                          \
  (container->emplace_front(__VA_ARGS__), container->front())
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
void cleanup() {
  for (auto tid : *threads) {
    pthread_kill(tid, INTERRUPT_SIGNAL);
    pthread_join(tid, nullptr);
  }
  delete threads;
}
void *__daemon_wrapper(void *arg) {
  PvzDaemon *daemon = reinterpret_cast<PvzDaemon *>(arg);
  while (daemon->on) {
    if (!__isGaming())
      sleep(5);
    else
      daemon->callback(daemon->com);
  }
  pthread_exit(nullptr);
}
void runasDaemon(Communicator *com, PvzDaemon *daemon) {
  daemon->com = com;
  if (!daemon->on) {
    daemon->on = true;
    pthread_create(&daemon->tid, nullptr, __daemon_wrapper, daemon);
  }
}
void cancelDaemon(PvzDaemon *daemon) {
  if (daemon->on) {
    daemon->on = false;
    pthread_join(daemon->tid, nullptr);
  }
}
void handleCheatFunction(msgPack *pack, PvzServer *server) {
  auto *instance = Options::getInstance();
  auto *o = instance->getOption(pack->id);
  DEBUG_LOG("GOT ID:%d NAME:%s", pack->id, o->name);
  if (o->attr & GAMING && !__isGaming()) {
    server->sendMessage(makeMsgPack(0, "NOT GAMING", msgStatus::REMOTE_ERROR));
    return;
  }
  if (o->attr & PLANTS_CALLBACK) {
    eachPlant(server, o->object_callback);
  } else if (o->attr & ZOMBIES_CALLBACK) {
    eachZombie(server, o->object_callback);
  } else if (o->attr & MOWERS_CALLBACK) {
    eachMower(server, o->object_callback);
  } else if (o->attr & DAEMON_CALLBACK) {
    runasDaemon(server, &o->daemon);
  } else if (o->attr & CANCEL_DAEMON_CALLBACK) {
    cancelDaemon(instance->getDaemon(pack->id - 1));
  } else {
    o->normal_callback(server, pack);
  }
}
void handleClientCommand(msgPack *pack) {
  PvzServer *server = PvzServer::getLocalInstance();
  if (pack->status == msgStatus::COMMAND) {
    server->handleBuiltinsCommand(pack);
  } else {
    handleCheatFunction(pack, server);
  }
  server->sendEOR();
}
void *__server_process(void *pfd) {
  PvzServer server(*reinterpret_cast<int *>(pfd));
  while (msgPack *pack = server.recvMessage().getValue()) {
    if (pack->status == msgStatus::EOR)
      continue;
    handleClientCommand(pack);
  }
  DEBUG_LOG("CONNECTION CLOSED");
  server.disconnect();
  pthread_exit(nullptr);
}
void *__server_main(void *) {
  PvzServer server(SERVER_ADDR, SERVER_PORT);
  // Initialize options
  Options option;
  int csock;
  registerSignalMask();
  while ((csock = server.doAccept()) != -1) {
    DEBUG_LOG("GONNA A CLIENT");
    pthread_create(&emplace_front(threads, 0), nullptr, __server_process,
                   reinterpret_cast<void *>(&csock));
  }
  DEBUG_LOG("MAIN SERVER CLOSED");
  server.disconnect();
  pthread_exit(nullptr);
}

#define NATIVE_NAME(name) Java_ze00_PvzCheater_Helper_##name
extern "C" void NATIVE_NAME(invokePvzServer)() {
  DEBUG_LOG("INVOCATION");
  // XXX We create a deque on heaps
  // because if it was a global variable
  // it will free automaticly
  threads = new std::deque<pthread_t>();
  pthread_create(&emplace_front(threads, 0), nullptr, __server_main, nullptr);
  atexit(cleanup);
}
