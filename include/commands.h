
#pragma once

#include "common.h"
#include "stack.h"
#include "circularArr.h"

int command_separator (char* operand1, char* operand2, char** postfix, stack_t_* st);
int here_document_command (char* operand1, char* operand2, char** postfix, stack_t_* st);
int input_redir_command (char* operand1, char* operand2, char** postfix, stack_t_* st);
int output_command (char* operand1, char* operand2, int flag, char** postfix, stack_t_* st, circularArr_t* ca);
int pipe_command(char* operand1, char* operand2, char** postfix, stack_t_* st);
