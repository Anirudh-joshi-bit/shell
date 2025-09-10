#include "header.h"


// global or static variables must init with compile time conatanst (numbers, addres to other global, NULL)
circularArr_t* ca;

char** sanitise (char** toks, int size){
	// calloc -> init with NULL
	char** parsed = calloc (MAXNUM_COMMAND, sizeof (char*));
	char** parsed_it = parsed;
	char** tok_it = toks;

	while (strcmp (*tok_it, "")){

		char* str_it = *tok_it;
		bool trash = true;

		while (*str_it){

			if (*str_it != ' ' && *str_it != '\n') {
				trash = false;
				break;
			}
			str_it++;
		}
		if (!trash){
			*parsed_it = strdup (*tok_it);
			parsed_it ++;
		}
		tok_it ++;

	}

	*parsed_it = strdup ("");

	clean2Dstring (toks, 0, size);
	return parsed;
}


int main  (){

circularArr_t* ca = malloc (sizeof (circularArr_t));
ca_init (ca, MAXNUM_HISTORY);

const char *art =
"███████╗██╗  ██╗███████╗██╗     ██╗     \n"
"██╔════╝██║  ██║██╔════╝██║     ██║     \n"
"███████╗███████║█████╗  ██║     ██║     \n"
"╚════██║██╔══██║██╔══╝  ██║     ██║     \n"
"███████║██║  ██║███████╗███████╗███████╗\n"
"╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n";
printf("%s", art);


	while (true){

		char pwd[MAXLEN_PWD];
		getcwd (pwd, MAXLEN_PWD);
		printf ("__%s__ $ ", pwd);

		char main_input[MAXLEN_INPUT], input[MAXLEN_INPUT];

		int main_size = MAXLEN_INPUT, main_iter = 0, size = MAXLEN_INPUT, error_found = 0;



		// tab completion implementation

		//int st = tab_completion (main_input, main_size);

		fgets (input, size, stdin);
		if (!strcmp (input, "exit\n"))
			break;


		size = strlen (input);

		// handling <<

		for (int i=0; i<size-1; i++){
			if (main_iter >= main_size) {
				perror ("too big input");
				error_found = 1;
				break;

			}
			if (input[i] == '<' && input[i+1] == '<'){

				if (main_iter >= main_size-1){
					perror ("too big of a command");
					error_found = 1;
					break;

				}
				main_input[main_iter++] = '<';
				main_input[main_iter++] = '<';

				char delim [MAXLEN_DELIM];
				i+=2;

				int del_iter = 0;

				while (i<size && input[i] == ' ') i++;
				if (i == size-1) {
					perror ("no delimeter found !!");
					error_found = 1;
					break;
				}
				while (i<size){

					if (input[i] == ' ') {
						error_found =  1;
						perror ("error in delimeter in << ");
						break;

					}
					if (del_iter >= 19){
						error_found = 1;
						perror ("error ! too big delimeter ");
						break;
					}
					delim [del_iter++] = input[i++];
				}

				if (error_found){
					break;
				}
				delim[del_iter] = '\0';
				printf ("delim = %s\n", delim);

				// get line ->
				char temp[MAXLEN_COMMAND];
				int size_temp = MAXLEN_COMMAND;
				fgets(temp, size_temp, stdin);

				while (strcmp(delim, temp)){

					if (temp[0] == '\0') continue;
					main_iter = string_append(main_input, temp, main_iter, main_size) ;
				//	main_input[main_iter] = '\n';
					memset (temp, 0, size_temp);
					fgets (temp, size_temp, stdin);

					if (main_iter == -1) {
						perror ("too big input !!!");
						error_found = true;
						break;
					}
				}
				if (error_found){
					break;
				}
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


		main_input[main_iter] = '\0';



		if (main_input[0] != '\0'){
			push_crclArr(ca, main_input);
		}




		if (error_found) {
			perror ("error found inside the for loop !!");
			continue;
		}

		// handling << -> done

		 main_size = strlen (main_input);

		 // for handling single command
		 int tok_size = 0;
		char** toks = parse(main_input, (int)main_size, &tok_size);
		if (!toks) {
			perror ("input didnot parsed !!");
			continue;
		}
		// probelm -> <space> <operator> <command>
		// in the above case -> parser thinks <space> as a separate command
		// this creates problem in execute fun (in the stack) as there are spaces in the place of actual command !
		// sol ->
		// remove the entries with no actual command (only spaces and one \n at the end)

		char** parsed = sanitise (toks, MAXNUM_COMMAND);
//		char** t = parsed;
//		while (strcmp (*t, "")){
//			printf ("%s\n", *t);
//			t++;
//
//		}
//
//
//
//			//printf ("hii");
		if (tok_size == 1){
//			// no operator !
			int size_ = 0;
			char** arg = tokenise(parsed[0], " \n\t", &size_);
		//	char** arg_it = arg;
//			while (*arg_it){
//				printf ("%s\n", *arg_it);
//				arg_it++;
//
//			}

			int f = fork();
			if (f== 0){

				execvp (arg[0], arg);
				exit (1);

			}
			else if (f > 0){
				int st = 0;
				waitpid (f, &st, 0);
				if (st && builtin (arg, ca)){
					perror ("single command exe ERROR ");
				}
			}
			else
				exit (1);

			// clean shit

			continue;
		}

		char** postfix = postfix_conversion (parsed, (int)main_size);

//		char ** post = postfix;
//		while (strcmp (*post, "")){
//
//			printf ("%s\n", *post);
//			post++;
//		}

		clean2Dstring(parsed, 0, MAXNUM_COMMAND);
//
		execute (postfix, ca);

		clean2Dstring (postfix, 0,  MAXNUM_COMMAND);
	}
	return 0;
}
