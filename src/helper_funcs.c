#include "../include/common.h"
#include "../include/defines.h"
#include "../include/stack.h"

int string_append (char* dest, char* target, int dest_end, int dest_size){

	char* target_it = target;

	// delete headint spaces
	while (*target_it && *target_it == ' ') target_it++;


	while (dest_end < dest_size && *target_it){
		if (dest_end >= dest_size) return -1;
		dest[dest_end++] = *target_it;
		target_it ++;
	}
	return dest_end;
}

void clean2Dstring (char** tok, int st, int size){

	if (!tok) return;
	for (int i=st; i<=size; i++){
		if (tok[i])
			free (tok[i]);
	}
	if (!st)
		free(tok);
}

int getOperNum (char a){
	//if ((a == '<' && b == '<') || (a == '>' && b == '>')) return 2;
	return a=='|' ||  a== '<' ||  a=='>' ||  a=='(' ||  a==')' ||  a==';' || a=='$';
}


bool isOper (char* op){
	return !strcmp(op, "(") || !strcmp(op, ")") || !strcmp(op, "<") || !strcmp(op, "<<") || !strcmp(op, ">") || !strcmp(op, ">>") || !strcmp(op, "|") || !strcmp(op, ";") || !strcmp (op, "$");
}

int getPrecedence (char* oper){
	//always ; has the least precedence  => need of postfix conversion and precedence !!!

	if (!strcmp(oper, "(") || !strcmp(oper, ")")) return -1;
	if (!strcmp (oper, "$")) return 1;
	if (!strcmp(oper, "<") || !strcmp(oper, "<<") || !strcmp(oper, ">") || !strcmp(oper, ">>")) return 1;
	if (!strcmp(oper, "|")) return 1;
	if (!strcmp(oper, "&")) return 1;
	if (!strcmp(oper, ";")) return 0;

	return -1;
}

char** tokenise (char* string, char* delim, int* size){

	if (!string){
		*size = 0;
		return NULL;
	}


	char** toks = calloc (MAXNUM_COMMAND , sizeof (char*));
	int tok_iter = 0;
	char* tok = strtok (string, delim);

	while (tok){

		toks[tok_iter++] = strdup (tok);
		if (tok_iter >= MAXNUM_COMMAND){

			clean2Dstring (toks, 0, MAXNUM_COMMAND);
			//free (tok);
			return NULL;

		}
		//free (tok);

		tok = strtok (NULL, delim);

	}
	free (string);
	*size = tok_iter;
	toks[tok_iter] = NULL;

	return toks;

}


void print_stack (stack_t_* st){
	stack_t_ st_;
	stack_init (&st_);

	printf ("printing stack.... \n");
	while (st->size){
		stack_push (&st_, stack_top(st));
		printf ("%s\n", stack_top (&st_));
		stack_pop(st);
	}
	while (st_.size){

		stack_push (st, stack_top (&st_));
		stack_pop(&st_);
	}

}
//int exe_one_command (char* command){
//
//	int size = 0, st = 0;
//
//	int f = fork ();
//	if (f == 0){
//		char** args = tokenise (command, " \n\t", &size);
//		execvp (args[0], args);
//		perror ("command not found \n");
//		// why clean if exiting -> so that valgind donot track this !!
//		clean2Dstring (args, 0, size);
//		exit(1);
//	}
//	else if (f > 0){
//		wait (&st);
//		if (st){
//			perror ("ERROR in launching command \n");
//			return -1;
//		}
//	}
//	else {
//		perror ("ERROR in fork in exe_one_command \n");
//		exit (-1);
//	}
//	return 0;
//}
