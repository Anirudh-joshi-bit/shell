#pragma once

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define MAXLEN_COMMAND 100
#define MAXNUM_COMMAND 100
#define MAXLEN_INPUT 2048
#define MAXLEN_DELIM 30
#define MAXLEN_OUTPUT 1048576

// structs ->
typedef struct __node_t {
	char* command;
	struct __node_t* next;
}node_t;
typedef struct __list_t {
	node_t* head;
	int size;
}list_t;
typedef struct __stack_t {
	list_t* l;
	int size;
}stack_t;


// functions ->
void stack_init (stack_t*);
int stack_pop(stack_t*);
char* stack_top(stack_t*);
void stack_push(stack_t*, char*);
int getOperNum (char, char);
bool isOper (char*);
char** parse (char*, int);
int getOperNum (char, char);
void cleanToken (char**, int);
int getPrecedence (char*);
char** postfix_conversion (char**, int);
char** filter(char**);
int string_append (char*, char*, int, int);
char** tokenise (char*, char*);
