 /*
  * File    : client.cc
  * Project :
  * Author  : ze00
  * Email   : zerozakiGeek@gmail.com
  * Date    : 2019-01-18
  * Module  : 
  * License : MIT
  */
#include <common/common.h>
#include <common/communicator.h>
int main() {
  Communicator communicator(SERVER_ADDR, SERVER_PORT, Communicator::Client);
  return 0;
}
