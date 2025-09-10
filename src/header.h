// make packaging well and do not include a singe header in every file


#pragma once

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif


#include <termios.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>

#define RED "\x1b[31;40m"
#define	BLUE "\x1b[34;40m"
#define GREEN "\x1b[32;40m"

#define APPEND_OUTPUT 1
#define TRUNC_OUTPUT 0
#define MAXLEN_PWD 200
#define MAXLEN_COMMAND 100
#define MAXNUM_COMMAND 100
#define MAXLEN_INPUT 2048
#define MAXLEN_DELIM 30
#define MAXLEN_OUTPUT 1048576
#define MAXNUM_HISTORY 100


// structs ->
typedef struct __node_t {

	char*				command;
	struct __node_t*	next;
	struct __node_t*	prev;
}node_t;
typedef struct __list_t {

	node_t*		head;
	node_t*		tail;
	int			size;
}list_t;
typedef struct __stack_t {

	list_t*		l;
	int			size;
}stack_t_;

typedef struct __circularArr_t{

	char**		arr;
	int			s;
	int			e;
	int			size;
	int			maxsize;

}circularArr_t;



// circularArr_t* ca   -> this is not declaration instead is global definition
// => can cause problem if seen multiple times
// extern -> tells the compiler that the defination is somewhere else
// and donot define it here !
// defined in the main function



#ifndef CRCL_ARR
#define CRCL_ARR

extern circularArr_t* ca;

#endif






// functions ->
int tab_completion(char* input, int size);
void stack_init (stack_t_*);
int stack_pop(stack_t_*);
char* stack_top(stack_t_*);
void stack_push(stack_t_*, char*);
int execute (char**);
int getOperNum (char, char);
bool isOper (char*);
char** parse (char*, int, int*);
int getOperNum (char, char);
void clean2Dstring(char**, int, int);
int getPrecedence (char*);
char** postfix_conversion (char**, int);
char** filter(char**);
int string_append (char*, char*, int, int);
char** tokenise (char*, char*, int*);
void print_stack (stack_t_*);
int exe_one_command(char*);
void list_init (list_t* l);
int insert_head (list_t* l, char* command);
int delete_head (list_t* l);
int print_history (circularArr_t* ca, long ind);
int builtin (char** args, circularArr_t* ca);
void ca_init (circularArr_t* ca, int ind);
 void push_crclArr (circularArr_t* ca, char* str);
