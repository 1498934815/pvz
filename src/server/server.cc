/*
 * File    : server.cc
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-01-18
 * Module  :
 * License : MIT
 */
#include <algorithm>
#include <common/common.h>
#include <common/communicator.h>
#include <common/options.h>
#include <deque>
#include <pthread.h>
#include <server/Pvz.h>
#include <server/PvzServer.h>
#include <signal.h>
static std::deque<pthread_t> *threads;
static std::deque<PvzDaemon> *daemons;
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
  delete daemons;
}
PvzDaemon *findDaemon(unsigned id) {
  auto &&it = std::find(daemons->begin(), daemons->end(), id);
  if (it != daemons->end())
    return &*it;
  return nullptr;
}
void *__daemon_wrapper(void *arg) {
  PvzDaemon *daemon = reinterpret_cast<PvzDaemon *>(arg);
  while (daemon->on) {
    if (!__isGaming())
      usleep(500000);
    else
      daemon->callback(daemon->com);
  }
  pthread_exit(nullptr);
}
void runasDaemon(Communicator *com, const option *o) {
  PvzDaemon *daemon;
  if (!(daemon = findDaemon(o->id)))
    daemon = &emplace_front(
        daemons, PvzDaemon{o->daemon_callback, o->id, 0, com, false});
  if (!daemon->on) {
    daemon->on = true;
    pthread_create(&daemon->tid, nullptr, __daemon_wrapper, daemon);
  }
}
void cancelDaemon(const option *o) {
  PvzDaemon *daemon = findDaemon(o->id - 1);
  if (daemon && daemon->on) {
    daemon->on = false;
    pthread_join(daemon->tid, nullptr);
  }
}
void handleCheatFunction(msgPack *pack, PvzServer *server) {
  auto *o = Options::getInstance()->getOption(pack->id);
  DEBUG_LOG("GOT ID:%d NAME:%s", pack->id, o->name);
  if (o->attr & GAMING && !__isGaming()) {
    server->sendMessage(
        makeMsgPack(0, "UNINITIALIZED", msgStatus::REMOTE_ERROR));
    return;
  }
  if (o->attr & PLANTS_CALLBACK) {
    eachPlant(server, o->object_callback);
  } else if (o->attr & ZOMBIES_CALLBACK) {
    eachZombie(server, o->object_callback);
  } else if (o->attr & MOWERS_CALLBACK) {
    eachMower(server, o->object_callback);
  } else if (o->attr & DAEMON_CALLBACK) {
    runasDaemon(server, o);
  } else if (o->attr & CANCEL_DAEMON_CALLBACK) {
    cancelDaemon(o);
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
extern "C" void __attribute__((constructor)) __server_main_invocation() {
  DEBUG_LOG("INVOCATION");
  // XXX We create a deque on heaps
  // because if it was a global variable
  // it will free automaticly
  threads = new std::deque<pthread_t>();
  daemons = new std::deque<PvzDaemon>();
  pthread_create(&emplace_front(threads, 0), nullptr, __server_main, nullptr);
}
extern "C" void __attribute__((destructor)) __server_cleanup() {
  DEBUG_LOG("CLEANUP");
  cleanup();
}
