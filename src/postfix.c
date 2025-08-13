#include "header.h"

bool isOper (char* op){
	return !strcmp(op, "(") || !strcmp(op, ")") || !strcmp(op, "<") || !strcmp(op, "<<") || !strcmp(op, ">") || !strcmp(op, ">>") || !strcmp(op, "|") || !strcmp(op, "&") || !strcmp(op, ";");
}

int getPrecedence (char* oper){
	if (!strcmp(oper, "(") || !strcmp(oper, ")")) return -1;
	if (!strcmp(oper, "<") || !strcmp(oper, "<<") || !strcmp(oper, ">") || !strcmp(oper, ">>")) return 3;
	if (!strcmp(oper, "|")) return 2;
	if (!strcmp(oper, "&")) return 1;
	if (!strcmp(oper, ";")) return 0;

	return -1;
}


int string_append (char* dest, char* target, int dest_end){

	char* target_it = target;

	// delete headint spaces
	while (*target_it && *target_it == ' ') target_it++;


	while (*target_it){
		dest[dest_end++] = *target_it;
		target_it ++;
	}
	return dest_end;
}


char* postfix_conversion (char** toks, int input_size){
	stack_t* st = malloc(sizeof (stack_t));
	stack_init (st);
	char** tok_iter = toks;

	stack_push(st, "(");

	char* postfix = malloc (input_size* sizeof (char));
	int postfix_iter = 0;

	while (*tok_iter) {
		if (isOper (*tok_iter)){

			if (!strcmp (*tok_iter, ")")){
				while (strcmp (stack_top (st), "(")){
					postfix_iter = string_append (postfix, stack_top (st), postfix_iter);
					assert(!stack_pop(st));
				}
				assert (!stack_pop(st));
			}
			// we dont want to remove anything if the current char is "(";
			else if (!strcmp (*tok_iter, "(")) stack_push (st, *tok_iter);
			else{

				while (getPrecedence(stack_top (st)) >= getPrecedence (*tok_iter)){
					// pop, append;
					postfix_iter = string_append (postfix, stack_top(st), postfix_iter);
					assert (!stack_pop (st));
				}
				stack_push(st, *tok_iter);
			}
		}
		else {
			postfix_iter = string_append (postfix, *tok_iter, postfix_iter);
		}
		tok_iter ++;
	}
	while (st->size > 1){			// the last element is ( that we dont want to add to the string;
		postfix_iter = string_append (postfix, stack_top (st), postfix_iter);
		stack_pop(st);
	}

	free (st);
	postfix_iter = '\0';

	printf ("%s\n", postfix);


	return postfix;
}
