 /*
  * File    : server.cc
  * Project :
  * Author  : ze00
  * Email   : zerozakiGeek@gmail.com
  * Date    : 2019-01-18
  * Module  : 
  * License : MIT
  */
#include <unistd.h>
#include <pthread.h>
#include <common/common.h>
#include <common/communicator.h>
static pthread_t tid;
void *__server_main(void *) {
  Communicator communicator(SERVER_ADDR, SERVER_PORT, Communicator::Server);
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
