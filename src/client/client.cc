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
  Client client(SERVER_ADDR, SERVER_PORT);
  for (int i = 0; i < 10; ++i) {
    client.sendMessage(makeMsgPack(STRING, i, "Hello World"));
    for (auto &&reply : client.recvMessages()) {
      DEBUG_LOG("%d %s", reply.id, reply.msg);
    }
  }
  client.sendMessage(makeMsgPack(EOR, 0, "EOR"));
  return 0;
}
