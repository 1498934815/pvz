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
#include <common/options.h>
#include <common/communicator.h>
#include <client/PvzClient.h>
void printAuthorInfo() {
  uinotice("Github " GIT_REPO);
  uinotice("Tieba " TIEBA_POST_URL " @" AUTHOR);
  uinotice("CommitHash " GIT_HASH);
  uinotice("关于本程序的用法 " README_MD);
}
void printDebugInfo() {
  PvzClient *pvz = PvzClient::getInstance();
  pvz->fetchInfos();
  uinoticef("PID:%d 基址:%p 状态与信息:%p\n", pvz->pid, pvz->base, pvz->status);
}
void displayUserInterface() {
  for (auto &&o : Options::getInstance()->getOptions()) {
    uiprintf("%d.%s\n", o.id, o.name);
  }
  getchar();
}
int main() {
  printAuthorInfo();
  PvzClient client(SERVER_ADDR, SERVER_PORT);
  printDebugInfo();
  Options options;
  while (true) {
    displayUserInterface();
  }
  return 0;
}
