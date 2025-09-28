
#pragma once

#define RESET   "\001\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define CYAN    "\001\033[1;36m"
#define MAGENTA "\033[1;35m"



#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define PERROR(msg) perror (RED msg RESET)

#define MINI_SHELL 1
#define NOT_MINI_SHELL 0
#define APPEND_OUTPUT 1
#define TRUNC_OUTPUT 0
#define MAXLEN_PWD 200
#define MAXLEN_COMMAND 1000
#define MAXNUM_COMMAND 100
#define MAXLEN_INPUT 2048
#define MAXLEN_DELIM 30
#define MAXLEN_OUTPUT 1048576
#define MAXNUM_HISTORY 1024
