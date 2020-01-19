/*
 * File    : client.cc
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2019-01-18
 * Module  :
 * License : MIT
 */
#include "client/PvzClient.h"
#include "common/PvzUtils.h"
#include "common/common.h"
#include "common/options.h"
#include <setjmp.h>
#include <stdarg.h>
void printAuthorInfo() {
  uinoticef("Github %s\n", GIT_REPO);
  uinoticef("Tieba 植物大战僵尸吧@%s\n", AUTHOR);
  uinoticef("Version v%d(%s)\n", LOCAL_VERSION, GIT_HASH);
  uinoticef("本程序的使用手册 %s\n", README_MD);
  uinoticef("修改时用到的代码 %s\n", CODE_TXT);
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
int getUserIntInputSafety() {
  static int val;
  getUserInputSafety("?", "%d", &val);
  return val;
}
const char *getUserStringInputSafety(char *buf) {
  getUserInputSafety("?", "%s", buf);
  return buf;
}
void displayUserInterface() {
  Options::getInstance()->uiPrint();
}
void printCodes(struct option *o, std::vector<int> &&codes) {
  const char **codesMap = nullptr;
  if (o->attr & ZOMBIES_CODE)
    codesMap = zombiesCodesMap;
  else if (o->attr & CARDS_CODE)
    codesMap = cardsCodesMap;
  uiprintf(HINT);
  for (int &c : codes)
    uiprintf("%s ", codesMap[c]);
  uiprint("");
}
void printCode(struct option *o, int code) {
  const char **codesMap = nullptr;
  if (o->attr & GAMES_CODE)
    codesMap = gamesCodesMap;
  else if (o->attr & FIELDS_CODE)
    codesMap = fieldsCodesMap;
  uihintf("%s\n", codesMap[code]);
}
void handleUserInput(int inputId) {
  auto *o = Options::getInstance()->getOption(inputId);
  if (o == nullptr) {
    uierror("您的输入有误");
    return;
  }
  if (o->description)
    uihintf("%s\n", o->description);
  PvzClient *client = PvzClient::getInstance();
  msgPack msg = {
      .id = o->id,
  };
  if (o->attr & EXIT) {
    client->disconnect();
    exit(0);
  } else if (o->attr & DEBUG_INFO) {
    client->printDebugInfo();
    return;
  }
  if (o->attr & GETINT) {
    msg.val = getUserIntInputSafety();
    printCode(o, msg.val);
  } else if (o->attr & GETINTS) {
    getUserStringInputSafety(msg.msg);
    printCodes(o, parseInts(msg.msg));
  } else if (o->attr & GETPOINTS) {
    getUserStringInputSafety(msg.msg);
    parsePoints(msg.msg);
  }
  client->sendMessage(msg);
  for (auto &&m : client->recvMessages())
    if (m.status == msgStatus::REMOTE_ERROR)
      uierrorf("%s\n", m.msg);
    else
      uihintf("%s\n", m.msg);
}
void checkVersion() {
  int version = PvzClient::getInstance()->getVersion();
  if (version != LOCAL_VERSION) {
    uierrorf("修改器(v%d)与主程序(v%d)版本不一致!\n", LOCAL_VERSION, version);
    uierror("请看" README_MD ",下载最新版本的主程序与修改器");
    exit(-1);
  }
}
static jmp_buf env;
#define JMP_RET 0xff
void recoverEnv(int) {
  longjmp(env, JMP_RET);
}
int main() {
  printAuthorInfo();
  PvzClient client(SERVER_ADDR, SERVER_PORT);
  Options options;
  checkVersion();
  client.printDebugInfo();
  signal(SIGINT, recoverEnv);
  setjmp(env);
  while (true) {
    displayUserInterface();
    handleUserInput(getUserIntInputSafety());
  }
  return 0;
}
