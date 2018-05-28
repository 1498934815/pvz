/*
 * File    : inc/client.h
 * Project : PVZ
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2018-04-12
 * Module  :
 * License : MIT
 */
#ifndef __CHEATER__H
#define __CHEATER__H
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "base.h"
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
  detectPVZ();
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
  } else if (strcmp(rec, "uninitialized") == 0) {
    printf("您所选的选项'%s' 需要先开始游戏\n", cmd);
  }
  return rec;
}
void parseAddr(const char *rec, void **out) { sscanf(rec, "%p", out); }
void parseInt(const char *rec, int *out) { sscanf(rec, "%d", out); }
void getRemoteBase() { parseAddr(doCmd("getbase"), &info.base); }
void detectPVZ() { parseInt(doCmd("getpid"), &info.pid); }
void *getField() { return info.base; }
void *getStatus() {
  void *v;
  parseAddr(doCmd("getstatus"), &v);
  return v;
}
void catchSIGINT() {
  fflush(stdout);
  setbuf(stdin, NULL);
  destroy((__list **)&info.task);
  longjmp(env, SETJMP_RET);
}
void registeSigHandle() { signal(SIGINT, catchSIGINT); }
#endif //__CHEATER__H
