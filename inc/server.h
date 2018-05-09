/*
 * File    : inc/server.h
 * Project : PVZ
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2018-04-12
 * Module  :
 * License : MIT
 */
#ifndef __SERVER__H
#define __SERVER__H
#include <pthread.h>
#include <unistd.h>
#include "base.h"
#include "cheat.h"
#define is_cmd(icmd) (strcmp(cmd, icmd) == 0)
#define do_send(str) send(fd, str, strlen(str), 0)

int executeCmd(int fd, size_t len, const char *cmd) {
  static BufferType arg;
  memset(arg, 0, sizeof(arg));
  int option;
  // 没有匹配到:会停止
  // 但在那之前option已经得到正确的值
  sscanf(cmd, "%d:%s", &option, arg);
  // 这些是不需要游戏被初始化的选项
  if (getStatus() == NULL && option != 2 && option != 6 && option != 12) {
    return 1;
  }
  switch (option) {
  case 1:
    sscanf(arg, "%d", &info.val);
    setSun();
    break;
  case 2:
    freePlants();
    break;
  case 3:
    forEachZombies(coverZombies);
    break;
  case 4: {
    parseRowAndCol(arg, &info.task);
    while (info.task != NULL) {
      forEachZombies(putLadder);
      usleep(WAIT_USECONDS);
    }
  } break;
  case 5: {
    parseRowAndCol(arg, &info.task);
    forEachPlants(fuck_LilyPad_Pumpkin);
    destroy((__list **)&info.task);
  } break;
  // 在processCmd中会处理
  // getStatus
#if 0
  case 6:
    break;
#endif
  case 7:
    pass();
    break;
  case 8:
    sscanf(arg, "%d", &info.val);
    setFlags();
    break;
  case 9:
    callLadder();
    break;
  case 10:
    doLimits();
    break;
  case 11:
    sscanf(arg, "%d", &info.val);
    switchMode();
    break;
  case 12:
    sscanf(arg, "%d", &info.val);
    moveSaves();
    break;
  }
  return 0;
}
void processCmd(int fd, size_t len, const char *cmd) {
  if (is_cmd("getpid")) {
    do_send(to_string("%d", getpid()));
  } else if (is_cmd("getbase")) {
    do_send(to_string("%p", getField()));
  } else if (is_cmd("getstatus")) {
    do_send(to_string("%p", getStatus()));
  } else {
    if (executeCmd(fd, len, cmd)) {
      do_send("uninitialized");
    } else {
      do_send("success");
    }
  }
}
void *doProcessClient(void *arg) {
  int csock = *(int *)arg;
  size_t rlen, clen;
  static BufferType buf;
  static BufferType cmd;
  // 非'\0'结尾
  // 可能有魔法加成
  memset(cmd, 0, sizeof(cmd));
  memset(buf, 0, sizeof(buf));
  // 失败时会返回-1或者0
  while ((rlen = recv(csock, buf, BUFSIZE, 0)) > 0) {
    sscanf(buf, "%zu:%s", &clen, cmd);
    cmd[clen] = 0;
    processCmd(csock, clen, cmd);
  }
  close(csock);
  pthread_exit(NULL);
}
#endif //__SERVER__H
