/*
 * File    : defs.h
 * Project : PVZ
 * Author  : ze00
 * Email   : zerozakiGeek@gmail.com
 * Date    : 2017-08-17
 * Module  :
 * License : MIT
 */
#ifndef __DEFS__H
#define __DEFS__H
#include <limits.h>
#define SERVER_ADDR "127.0.0.1"
// 'ze' -> 0x7a65
#define SERVER_PORT 0x7a65
#define BUFSIZE 4096
#define IN_RANGE(obj, min, max) (obj >= min && obj <= max)
#define SETJMP_RET 0xff
#define WAIT_USECONDS (500000)

#define GITHUB "https://github.com/ze00/pvz"
#define BRANCH "client"
#define CODE_TXT GITHUB "/blob/" BRANCH "/doc/code.txt"
#define HELP_TXT GITHUB "/blob/" BRANCH "/doc/help.txt"
#define README_MD GITHUB "/blob/" BRANCH "/README.md"

#define TIEBA_POST_URL "https://tieba.baidu.com/p/5645499737"
#define AUTHOR "AS魇梦蚀"

#define NOTICE "\033[33;1m[NOTICE]\033[0m "
#define ERR "\033[31;1m[ERR]\033[0m "

#define UN_INIT "uninitialized"
#define GETPID "getpid"
#define GETHASH "gethash"
#define GETBASE "getbase"
#define GETSTATUS "getstatus"
#define PAGE_START(x) ((x)&PAGE_MASK)
#define PAGE_END(x) PAGE_START((x) + (PAGE_SIZE - 1))

#define ARRAY_SIZE(ar) (sizeof(ar) / sizeof(ar[0]))

#define notice(mess) puts(NOTICE mess)
#define noticef(...) printf(NOTICE __VA_ARGS__)
#define err(mess) puts(ERR mess)
#define errf(...) printf(ERR __VA_ARGS__)

typedef char BufferType[BUFSIZE];
#endif //__DEFS__H
