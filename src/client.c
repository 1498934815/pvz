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
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../inc/pvz.h"
#include "../inc/com.h"
#include "../inc/defs.h"
#include "../inc/utils.h"
#include "../inc/client.h"

bool initConnection(void) {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in sin;
  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_port = htons(SERVER_PORT);
  sin.sin_addr.s_addr = inet_addr(SERVER_ADDR);
  if (connect(sockfd, (struct sockaddr *)&sin, sizeof(sin)) == -1) {
    close(sockfd);
    return true;
  }
  info.sock = sockfd;
  return false;
}
int getSock(void) {
  return info.sock;
}
void initClientCore(void) {
  if (initConnection()) {
    if (getSock() == 0) {
      err("也许您没有启动PvZ?");
      err("也可能...您启动的是不正确的PvZ");
      err("请于 " TIEBA_POST_URL " 中下载对应的" SPECIFIC_PACKAGE
          "_V{修改器版本}.apk");
    } else {
      err("您已退出PvZ, 请重新进入PvZ后再开启本程序");
    }
    exit(-1);
  }
  extern void detectPVZ(void);
  extern void getRemoteBase(void);
  extern void verifyVersion(void);
  detectPVZ();
  verifyVersion();
  getRemoteBase();
}
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
    // 重新连接到新的server后重新执行
    doCmd(cmd);
  } else if (strcmp(rec, UN_INIT) == 0) {
    errf("您所选的选项'%s' 需要先开始游戏\n", cmd);
  }
  return rec;
}
void getRemoteBase(void) {
  parseAddr(doCmd(GETBASE), &info.base);
}
void detectPVZ(void) {
  parseInt(doCmd(GETPID), &info.pid);
}
void verifyVersion(void) {
  const char *hash = doCmd(GETHASH);
  if (strcmp(hash, GIT_HASH) != 0) {
    errf("修改器(%s)与主程序(%s)版本不一致!\n", GIT_HASH, hash);
    err("请于" TIEBA_POST_URL "下载最新版本的主程序与修改器");
    exit(-1);
  }
}
void *getField(void) {
  return info.base;
}
void *getStatus(void) {
  void *v;
  parseAddr(doCmd(GETSTATUS), &v);
  return v;
}
void catchSIGINT(int sig) {
  (void)sig;
  setbuf(stdin, NULL);
  destroy(&info.task);
  longjmp(env, SETJMP_RET);
}
void registerSigHandle(void) {
  signal(SIGINT, catchSIGINT);
}

void printInfo(void) {
  notice("Github " REPO);
  notice("Tieba " TIEBA_POST_URL);
  notice("CommitHash " GIT_HASH);
  notice("By 百度贴吧@" AUTHOR);
  notice("部分功能的解释见 " HELP_TXT);
  notice("关于进入其他无尽 " CODE_TXT);
  notice("关于本程序的使用 " README_MD);
}
void printDebugInfo(void) {
  // XXX 游戏重新打开时
  // getStatus() -> doCmd会重新得到PID/BASE
  // 如果最后一个参数是getStatus() 那么打印的PID/BASE可能是之前client的PID/BASE
  void *status = getStatus();
  noticef("PID:%d 基址:%p 状态与信息:%p\n", info.pid, info.base, status);
}
void doInitClient(void) {
  printInfo();
  initClientCore();
  doInitOptions();
  registerSigHandle();
  printDebugInfo();
}
void doDisplayUserInterface(void) {
  for_each_option(option) {
    printf("%u.%s\n", option->id, option->name);
  }
}
#define GETOPT(mess, opt)                                                      \
  printf(mess);                                                                \
  if (scanf("%u", &opt) != 1) {                                                \
    PANIC;                                                                     \
  }
#define GETOPT_V(mess) GETOPT(mess, info.val)

void doHandleUserOption(struct pvz_option *option) {
  static BufferType buf;
  enum user_attr attr = option->user_attr;
  if (option->notice != NULL) {
    printf("%s", option->notice);
  }
  if (attr & USER_EXIT) {
    close(getSock());
    exit(0);
  }
  if (attr & USER_DEBUGINFO) {
    printDebugInfo();
  }
  if (attr & USER_DONOTHING) {
    do_cmd();
  }
  if (attr & USER_GETINT) {
    GETOPT_V("?");
    sendI(info.val);
  }
  if (attr & USER_GETSTRING) {
    setbuf(stdin, NULL);
    if (fgets(buf, sizeof(buf), stdin) == NULL)
      PANIC;
    strip(buf);
    if (attr & USER_GETCOLROW) {
      // 作形式检查
      // 如果失败会引发SIGINT
      parseRowAndCol(buf, &info.task);
      destroy(&info.task);
    }
    sendS(buf);
  }
}
int main(int argc, char **argv) {
  doInitClient();
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
    doHandleUserOption(option);
  }
  return 0;
}
#undef GETOPT
#undef GETOPT_V
