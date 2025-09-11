#pragma once

#include "circularArr.h"

char** parse (char*, int, int*);
char** postfix_conversion (char**);
char* execute (char**, circularArr_t*, int*);
char* process_input(char* main_input, int main_size, circularArr_t* ca, int* status, int flag);
