/*
 * File    : src/server.c
 * Project : PVZ
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2018-04-12
 * Module  :
 * License : MIT
 */
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../inc/pvz.h"
#include "../inc/com.h"
#include "../inc/defs.h"
#include "../inc/cheat.h"

int sockfd;
void *initServer(void *unused) {
  (void)unused;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  int csock;
  struct sockaddr_in sin;
  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_port = htons(SERVER_PORT);
  sin.sin_addr.s_addr = inet_addr(SERVER_ADDR);
  int reuseaddr = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));
  bind(sockfd, (struct sockaddr *)&sin, sizeof(sin));
  listen(sockfd, 1024);
#define doAccept() (accept(sockfd, NULL, NULL))
  doInit();
  doInitOptions();
  while ((csock = doAccept())) {
    pthread_t tid;
    // 多个client时
    // &csock应该换成在csock在堆的拷贝
    extern void *doHandleClient(void *);
    pthread_create(&tid, NULL, doHandleClient, &csock);
    pthread_detach(tid);
  }
  pthread_exit(NULL);
}
#define is_cmd(icmd) (strcmp(cmd, icmd) == 0)
#define do_send(str) send(fd, str, strlen(str), 0)

int executeCmd(int fd, size_t len, const char *cmd) {
  static BufferType arg;
  memset(arg, 0, sizeof(arg));
  unsigned id;
  // 没有匹配到:会停止
  // 但在那之前option已经得到正确的值
  sscanf(cmd, "%u:%s", &id, arg);
  struct pvz_option *option = getOption(id);
  enum server_attr attr = option->server_attr;
  cheat_function function = option->callback;
  if ((attr & SERVER_NOT_INGAME) == 0 && getStatus() == NULL)
    return 1;
#define getV() sscanf(arg, "%d", &info.val)
  if (attr & SERVER_GETINT) {
    getV();
  }
  if (attr & SERVER_GETCOLROW) {
    parseRowAndCol(arg, &info.task);
  }
  if (attr & SERVER_NEED_ZOMBIES) {
    forEachZombies(function);
  } else if (attr & SERVER_NEED_PLANTS) {
    forEachPlants(function);
  } else {
    function(arg, NULL);
  }
  destroy((__list **)&info.task);
  return 0;
}
void handleCmd(int fd, size_t len, const char *cmd) {
  if (is_cmd(GETPID)) {
    do_send(to_string("%d", getpid()));
  } else if (is_cmd(GETBASE)) {
    do_send(to_string("%p", getField()));
  } else if (is_cmd(GETSTATUS)) {
    do_send(to_string("%p", getStatus()));
  } else if (is_cmd(GETHASH)) {
    do_send(GIT_HASH);
  } else {
    if (executeCmd(fd, len, cmd)) {
      do_send(UN_INIT);
    } else {
      do_send("success");
    }
  }
}
void *doHandleClient(void *arg) {
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
    handleCmd(csock, clen, cmd);
  }
  close(csock);
  pthread_exit(NULL);
}
#undef is_cmd
#undef do_send
#undef getV

void __attribute__((constructor)) doInitServer() {
  pthread_t tid;
  pthread_create(&tid, NULL, initServer, NULL);
  pthread_detach(tid);
}
void __attribute__((destructor)) doDestroyServer() { close(sockfd); }
