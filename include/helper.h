
#pragma once


#include "stack.h"
#include "circularArr.h"

int string_append (char* dest, char* target, int dest_end, int dest_size);
void clean2Dstring (char** tok, int st, int size);
int getOperNum (char a);
bool isOper (char* op);
int getPrecedence (char* oper);
char** tokenise (char* string, char* delim, int* size);
void print_stack (stack_t_* st);
