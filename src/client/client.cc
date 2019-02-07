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
  uinoticef("Github %s\n", GIT_REPO);
  uinoticef("Tieba %s @%s\n", TIEBA_POST_URL, AUTHOR);
  uinoticef("Version v%d(%s)\n", LOCAL_VERSION, GIT_HASH);
  uinoticef("关于本程序的用法 %s\n", README_MD);
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
  if (o->attr & EXIT) {
    client->disconnect();
    exit(0);
  } else if (o->attr & DEBUG_INFO) {
    client->printDebugInfo();
  }
  client->sendMessage(makeMsgPack(o->id));
  for (auto &&v : client->recvMessages())
    uiprintf("%s\n", v.msg);
}
void checkVersion() {
  int version = PvzClient::getInstance()->getVersion();
  if (version != LOCAL_VERSION) {
    uierrorf("修改器(v%d)与主程序(v%d)版本不一致!\n", LOCAL_VERSION, version);
    uierror("请于" TIEBA_POST_URL "下载最新版本的主程序与修改器");
    exit(-1);
  }
}
int main() {
  printAuthorInfo();
  PvzClient client(SERVER_ADDR, SERVER_PORT);
  Options options;
  checkVersion();
  client.printDebugInfo();
  int id = 0;
  while (true) {
    displayUserInterface();
    getUserInputSafety("?", "%d", &id);
    handleUserInput(id);
  }
  return 0;
}
