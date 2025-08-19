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



char** postfix_conversion (char** toks, int input_size){
	stack_t* st = malloc(sizeof (stack_t));
	stack_init (st);
	char** tok_iter = toks;

	stack_push(st, "(");
	// to do
	//
	//
	//
	//
	//
// dont push the string into a string but to a 2d string
// so that no datra is lost  => insert the string into toks
	char** toks_postfix = malloc (MAXNUM_COMMAND);

	char** tok_iter_res = toks_postfix;


	// first check if the given input as a valid paranthesis or not

	while (*tok_iter) {
		if (isOper (*tok_iter)){

			if (!strcmp (*tok_iter, ")")){
				while (strcmp (stack_top (st), "(")){
					*tok_iter_res = strdup (stack_top (st));
					tok_iter_res++;
					assert(!stack_pop(st));
				}
				assert (!stack_pop(st));
			}
			// we dont want to remove anything if the current char is "(";
			else if (!strcmp (*tok_iter, "(")) stack_push (st, *tok_iter);
			else{

				while (getPrecedence(stack_top (st)) >= getPrecedence (*tok_iter)){
					*tok_iter_res  = strdup (stack_top (st));					// this is important as stack_pop () is freeing the memory
																					// hence store a copy instead of the actual pointer;
					tok_iter_res++;
				// pop, append;
					assert (!stack_pop (st));
				}
				stack_push(st, *tok_iter);
			}
		}
		else {
			*tok_iter_res = strdup (*tok_iter);
			tok_iter_res++;

		}

		tok_iter ++;
	}
	while (st->size > 1){			// the last element is ( that we dont want to add to the string;
		*tok_iter_res = strdup (stack_top (st));
		tok_iter_res ++;
		stack_pop(st);
	}

	free (st);
	free (toks);


	return toks_postfix;
}
