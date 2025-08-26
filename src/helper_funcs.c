#include "header.h"


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

void cleanToken (char** tok, int size){

	if (!tok) return;
	int i = 0;
	while (i<size) free(tok[i++]);
	free (tok);
}

int getOperNum (char a, char b){
	if ((a == '<' && b == '<') || (a == '>' && b == '>')) return 2;
	return a=='|' ||  a== '<' ||  a=='>' ||  a=='(' ||  a==')' ||  a=='&' ||  a==';';
}


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

char** tokenise (char* string, char* delim){

	char** toks = malloc (MAXLEN_COMMAND * sizeof (char*));
	int tok_iter = 0;
	char* tok = strtok (string, " ") ;
	while (tok){

		toks[tok_iter++] = strdup (tok);
		if (tok_iter >= MAXLEN_COMMAND){

			perror ("too big command ERROR \n");
			return NULL;

		}
		tok = strtok (NULL, " ");

	}
	toks[tok_iter++] = NULL;

	for (; tok_iter < MAXLEN_COMMAND; tok_iter++){

		free (toks[tok_iter]);

	}

	return toks;

}
