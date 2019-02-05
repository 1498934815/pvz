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
#include <client/PvzClient.h>
void printAuthorInfo() {
  uinotice("Github " GIT_REPO);
  uinotice("Tieba " TIEBA_POST_URL " @" AUTHOR);
  uinotice("CommitHash " GIT_HASH);
  uinotice("关于本程序的用法 " README_MD);
}
void getUserInputSafety(const char *prompt, const char *fmt, ...) {
  uiprintf("%s", prompt);
  va_list va;
  va_start(va, fmt);
  if (uivscanf(fmt, va) <= 0) {
    uierror("您的输入有误");
    setbuf(stdin, nullptr);
  }
  va_end(va);
}
void displayUserInterface() {
  for (auto &&o : Options::getInstance()->getOptions()) {
    uiprintf("%d.%s\n", o.id, o.name);
  }
}
void handleUserInput(int inputId) {
  auto *o = Options::getInstance()->getOption(inputId);
  if (o == nullptr) {
    uierror("您的输入有误");
    return;
  }
  PvzClient *client = PvzClient::getInstance();
  uinoticef("SELECT %d %s %s\n", o->id, o->name, o->description);
  client->sendMessage(makeMsgPack(o->id));
  for (auto &&v : client->recvMessages())
    uiprintf("%s\n", v.msg);
}
int main() {
  printAuthorInfo();
  PvzClient client(SERVER_ADDR, SERVER_PORT);
  Options options;
  client.printDebugInfo();
  int id = 0;
  while (true) {
    displayUserInterface();
    getUserInputSafety("?", "%d", &id);
    handleUserInput(id);
  }
  return 0;
}
