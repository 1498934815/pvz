/*
 * File    : src/client.c
 * Project : PVZ
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2018-04-12
 * Module  :
 * License : MIT
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <setjmp.h>
#include <signal.h>
#include <strings.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../inc/pvz.h"
#include "../inc/defs.h"
#include "../inc/utils.h"
#include "../inc/base.h"
#include "../inc/com.h"

int initConnection() {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in sin;
  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_port = htons(SERVER_PORT);
  sin.sin_addr.s_addr = inet_addr(SERVER_ADDR);
  if (connect(sockfd, (struct sockaddr *)&sin, sizeof(sin)) == -1) {
    close(sockfd);
    return 1;
  }
  info.sock = sockfd;
  return 0;
}
void initClientCore() {
  if (initConnection()) {
    printf("也许您已经退出或者并没有启动Pvz?\n");
    exit(-1);
  }
  extern void detectPVZ();
  extern void getRemoteBase();
  extern void verifyVersion();
  detectPVZ();
  verifyVersion();
  getRemoteBase();
}
int getSock() { return info.sock; }
const char *doCmd(const char *cmd) {
  static BufferType rec;
  static BufferType snd;
  memset(snd, 0, sizeof(snd));
  memset(rec, 0, sizeof(rec));
  size_t len = strlen(cmd);
  sprintf(snd, "%zu:%s", len, cmd);
  if (send(getSock(), snd, strlen(snd), 0) == -1 ||
      recv(getSock(), rec, BUFSIZE, 0) <= 0) {
    close(getSock());
    // 重试
    initClientCore();
  } else if (strcmp(rec, UN_INIT) == 0) {
    errf("您所选的选项'%s' 需要先开始游戏\n", cmd);
  }
  return rec;
}
void getRemoteBase() { parseAddr(doCmd(GETBASE), &info.base); }
void detectPVZ() { parseInt(doCmd(GETPID), &info.pid); }
void verifyVersion() {
  if (strcmp(doCmd(GETHASH), PRIVATE_HASH) != 0) {
    err("修改器与主程序版本不一致!");
    err("请于" TIEBA_POST_URL "下载最新版本的主程序与修改器");
    exit(-1);
  }
}
void *getField() { return info.base; }
void *getStatus() {
  void *v;
  parseAddr(doCmd(GETSTATUS), &v);
  return v;
}
void catchSIGINT() {
  setbuf(stdin, NULL);
  destroy((__list **)&info.task);
  longjmp(env, SETJMP_RET);
}
void registerSigHandle() { signal(SIGINT, catchSIGINT); }

void printInfo() {
  notice("Github:" GITHUB);
  notice("Tieba:" TIEBA_POST_URL);
  notice("Commit Hash:" PRIVATE_HASH);
  notice("By 百度贴吧@" AUTHOR);
  notice("部分功能的解释见 " HELP_TXT);
  notice("关于进入其他无尽 " CODE_TXT);
  notice("关于本程序的使用 " README_MD);
  noticef("PID:%d 基址:%p 状态与信息:%p\n", info.pid, info.base, getStatus());
}
void doInitClient() {
  initClientCore();
  printInfo();
}
void doDisplayUserInterface() {
  const struct pvz_option *option;
  for (unsigned idx = 0; idx < getOptionsLength(); ++idx) {
    option = &pvz_options[idx];
    printf("%u.%s\n", option->id, option->name);
  }
}
#define GETOPT(mess, opt)                                                      \
  printf(mess);                                                                \
  if (scanf("%u", &opt) != 1) {                                                \
    PANIC;                                                                     \
  }
#define GETOPT_V(mess) GETOPT(mess, info.val)
#define do_send(fmt, ...) doCmd(to_string("%d:" fmt, option->id, __VA_ARGS__))
#define sendV() do_send("%d", info.val)

void doProcessUserOption(struct pvz_option *option) {
  static BufferType buf;
  enum user_attr attr = option->user_attr;
  if (option->notice != NULL)
    puts(option->notice);
  if (attr & USER_EXIT) {
    close(getSock());
    exit(0);
  }
  if (attr & USER_INFO) {
    printInfo();
  }
  if (attr & USER_DONOTHING) {
    sendV();
  }
  if (attr & USER_GETINT) {
    GETOPT_V("?");
    sendV();
  }
  if (attr & USER_GETSTRING) {
    setbuf(stdin, NULL);
    if (fgets(buf, sizeof(buf), stdin) == NULL)
      PANIC;
    if (option->user_attr & USER_GETCOLROW) {
      // 作形式检查
      // 如果失败会引发SIGINT
      parseRowAndCol(buf, &info.task);
      destroy((__list **)&info.task);
      do_send("%s", buf);
    }
  }
}
int main(int argc, char **argv) {
  doInitClient();
  doInitOptions();
  registerSigHandle();
  unsigned id;
  if (setjmp(env) == SETJMP_RET) {
    err("无效输入");
  }
  while (1) {
    doDisplayUserInterface();
    GETOPT("请输入:", id);
    struct pvz_option *option = getOption(id);
    if (option == NULL) {
      err("输入错误...");
      continue;
    }
    doProcessUserOption(option);
  }
  return 0;
}
#undef GETOPT
#undef GETOPT_V
#undef do_send
#undef sendV
