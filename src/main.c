#include "header.h"

int main  (){

	while (true){

		char* main_input = malloc (MAXLEN_INPUT);
		size_t main_size = MAXLEN_INPUT;
		int main_iter = 0;

		char* input = malloc (MAXLEN_INPUT);
		int size = MAXLEN_INPUT;

		fgets (input, size, stdin);
		int error_found = 0;
		size = strlen (input);

		// handling <<

		for (int i=0; i<size-1; i++){
			if (main_iter >= main_size) {
				perror ("too big input \n");
				error_found = 1;
				break;

			}
			if (input[i] == '<' && input[i+1] == '<'){

				if (main_iter >= main_size-1){
					perror ("too big of a command \n");
					error_found = 1;
					break;

				}
				main_input[main_iter++] = '<';
				main_input[main_iter++] = '<';

				char* delim = malloc (MAXLEN_DELIM);
				i+=2;

				int del_iter = 0;

				while (i<size && input[i] == ' ') i++;
				if (i == size-1) {
					perror ("no delimeter found !!\n");
					free (delim);
					error_found = 1;
					break;
				}
				while (i<size){

					if (input[i] == ' ') {
						error_found =  1;
						perror ("error in delimeter in << \n");
						break;

					}
					if (del_iter >= 19){
						error_found = 1;
						perror ("error ! too big delimeter \n");
						break;
					}
					delim [del_iter++] = input[i++];
				}

				if (error_found){
					free (delim);
					break;
				}
				delim[del_iter] = '\0';
				printf ("delim = %s\n", delim);

				// get line ->
				char* temp = malloc (MAXLEN_COMMAND);
				int size_temp = MAXLEN_COMMAND;
				fgets(temp, size_temp, stdin);

				while (strcmp(delim, temp)){

					if (temp[0] == '\0') continue;
					main_iter = string_append(main_input, temp, main_iter, main_size) ;
				//	main_input[main_iter] = '\n';
					memset (temp, 0, size_temp);
					fgets (temp, size_temp, stdin);

					if (main_iter == -1) {
						perror ("too big input !!! \n");
						error_found = true;
						break;
					}
				}
				if (error_found){
					free (temp);
					free (delim);
					break;
				}
				free (temp);
				free(delim);
			}
			else {
				main_input[main_iter++] = input[i];
			}
		// this is worrking as the below line is changinng the last char into a newline
		// but not incrementing the iterator
		// => for the next iteration,that \n will get overwritten as again in the loop, we are increamenting
		// i but not this iterator
		main_input [main_iter] = '\n';
		}
		// if main iter has changed its place then only mark the next char null char
		// or we can say if the loop has ever been active then it will automatically insert a new line char
		// at the end
		//
		//
		// but if the loop has never been active then there is no new linew char in current pos (main_iter)
		// meaning skipping one place will include some garbage value !!


		//if (main_iter)
			//main_input [main_iter+1] = '\0';
		//else main_input [main_iter] = '\0';
		main_input[main_iter] = '\0';


		if (error_found) {
			printf ("error found inside the for loop !! \n");
			free (main_input);
			free (input);
			continue;
		}

		// handling << -> done

		 main_size = strlen (main_input);

		char** toks = parse(main_input, (int)main_size);
		if (!toks) {
			printf ("input didnot parsed !! \n");
			free (main_input);
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
		// print the postfix !!
		char** postfix = postfix_conversion (toks, (int)main_size);

		char** post = postfix;
		while (*post){
			printf ("%s\n", *post);
			post++;

		}

		post = postfix;
		while (*post) {
			char* temp = *post;
			free (temp);
			post++;
		}
		free (postfix);
		free (main_input);
		free (input);
	}
}
