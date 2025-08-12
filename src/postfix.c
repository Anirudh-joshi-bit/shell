#include "header.h"

bool isOper (char* op){
	return !strcmp(op, "(") || !strcmp(op, ")") || !strcmp(op, "<") || !strcmp(op, "<<") || !strcmp(op, ">") || !strcmp(op, ">>") || !strcmp(op, "|") || !strcmp(op, "&") || !strcmp(op, ";");
}

int getPrecedence (char* oper){
	if (!strcmp(oper, "(") || !strcmp(oper, ")")) return 4;
	if (!strcmp(oper, "<") || !strcmp(oper, "<<") || !strcmp(oper, ">") || !strcmp(oper, ">>")) return 3;
	if (!strcmp(oper, "|")) return 2;
	if (!strcmp(oper, "&")) return 1;
	if (!strcmp(oper, ";")) return 0;

	return -1;
}


//////////////////// todo

stack_t* postfix_conversion (char** toks){
	stack_t* st = malloc(sizeof (stack_t));
	stack_init (st);
	char** it = toks;

	while (*it) {





	}
	return st;
}
