#include "header.h"

int main  (){

	while (true){

		char* input ;
		size_t size = 0;
		getline (&input, &size, stdin);

		size = strlen (input);

		char** toks = parse(input, (int)size);
		if (!toks) {
			printf ("input didnot parsed !! \n");
			free (input);
			continue;
		}

		//printf ("hii");
		char** it = toks;
		while (*it){
			printf ("%s\n", *it);
			it++;
		}
	//	printf ("%d", (int)size);
		// !!!!!!!!!!!! seg fault
	postfix_conversion (toks, (int)size);
		cleanToken (toks);
		free (input);
	}
}
