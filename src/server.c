/*
 * File    : src/server.c
 * Project :
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2018-04-12
 * Module  :
 * License : MIT
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../inc/pvz.h"
#include "../inc/defs.h"
#include "../inc/server.h"
void *initServer(void *unused) {
  (void)unused;
  int sockfd = socket(AF_INET, SOCK_STREAM, 0), csock;
  struct sockaddr_in sin, cl;
  memset(&sin, 0, sizeof(sin));
  memset(&cl, 0, sizeof(cl));
  sin.sin_family = AF_INET;
  sin.sin_port = htons(SERVER_PORT);
  sin.sin_addr.s_addr = inet_addr(SERVER_ADDR);
  bind(sockfd, (struct sockaddr *)&sin, sizeof(sin));
  listen(sockfd, 1024);
  socklen_t socklen = sizeof(cl);
#define doAccept() (accept(sockfd, (struct sockaddr *)&cl, &socklen))
  doInit();
  while ((csock = doAccept())) {
    pthread_t tid;
    // 多个client时
    // &csock应该换成在csock在堆的拷贝
    pthread_create(&tid, NULL, doProcessClient, &csock);
    pthread_detach(tid);
  }
  pthread_exit(NULL);
}
void __attribute__((constructor)) doInitServer() {
  pthread_t tid;
  pthread_create(&tid, NULL, initServer, NULL);
}
