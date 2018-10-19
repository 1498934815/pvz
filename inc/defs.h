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

#define REPO GIT_REPO
#define BRANCH GIT_BRANCH
#define BLOB REPO "/blob/" BRANCH
#define CODE_TXT BLOB "/doc/code.txt"
#define HELP_TXT BLOB "/doc/help.txt"
#define README_MD BLOB "/README.md"

#define NOTICE "\033[33;1m[NOTICE]\033[0m "
#define ERR "\033[31;1m[ERR]\033[0m "

#define OK "ok"
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

#define __pvz_unused __attribute__((unused))
#define __pvz_weak __attribute__((weak))
#define __pvz_cheat(name) void name(void *__pvz_unused remote)
#define pvz_cheat(name) __pvz_cheat(name) __pvz_weak
#define pvz_cheat_decl(name) __pvz_cheat(name)

typedef void (*cheat_function)(void *__pvz_unused);
typedef char BufferType[BUFSIZE];
#endif //__DEFS__H
