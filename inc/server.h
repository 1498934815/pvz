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
int executeCmd(int fd, size_t len, const char *cmd) {
  static BufferType arg;
  int option;
  // 没有匹配到:会停止
  // 但在那之前option已经得到正确的值
  sscanf(cmd, "%d:%s", &option, arg);
  // 这些是不需要游戏被初始化的选项
  if (getStatus() == NULL && option != 3 && option != 4 && option != 13) {
    return 1;
  }
  switch (option) {
  case 1:
    sscanf(arg, "%d", &baseInfo.val);
    setSun();
    break;
  case 2:
    forEachZombies(letZombiesFragile);
    break;
  case 3:
    increaseCabbagePult();
    break;
  case 4:
    freePlants();
    break;
  case 5:
    forEachZombies(coverZombies);
    break;
  case 6:
    forEachZombies(increaseZombies);
    break;
  case 7:
    forEachPlants(increasePlants);
    break;
  case 8:
    forEachPlants(increasePlantsAttack);
    break;
  case 9: {
    parseRowAndCol(arg, &baseInfo.task);
    while (baseInfo.task != NULL) {
      forEachZombies(putLadder);
      usleep(WAIT_USECONDS);
    }
  } break;
  case 10: {
    parseRowAndCol(arg, &baseInfo.task);
    forEachPlants(fuck_LilyPad_Pumpkin);
    destroy((__list **)&baseInfo.task, NULL);
  } break;
  case 11:
    forEachPlants(plants_freeze);
    break;
  case 12:
    forEachPlants(plants_attack);
    break;
    // 在processCmd中会处理
    // getStatus
#if 0
  case 13:
    break;
#endif
  case 14:
    pass();
    break;
  case 15:
    sscanf(arg, "%d", &baseInfo.val);
    setFlags();
    break;
  case 16:
    callLadder();
    break;
  case 17:
    doLimits();
    break;
  }
  return 0;
}
void processCmd(int fd, size_t len, const char *cmd) {
#define is_cmd(icmd) (strcmp(cmd, icmd) == 0)
#define do_send(str) send(fd, str, strlen(str), 0)
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
  // 失败时会返回-1或者0
  while ((rlen = recv(csock, buf, BUFSIZE, 0)) > 0) {
    sscanf(buf, "%zu:%s", &clen, cmd);
    // 非'\0'结尾
    // 可能有魔法加成
    cmd[clen] = 0;
    processCmd(csock, clen, cmd);
  }
  shutdown(csock, SHUT_RDWR);
  pthread_exit(NULL);
}
#endif //__SERVER__H
