#include "../include/common.h"
#include "../include/defines.h"
#include "../include/circularArr.h"
#include "../include/helper.h"
#include "../include/builtin.h"
#include "../include/utils.h"


// removing all the clean2Dstring as i am getting segfault all the time when recursion
// is used ($)
//
// solve this -> check where is the problem
//
// vim , and other program that run in the terminal cannot be paired with other commands
// as programs like vim expect the output screen to be connected to the terminal
// ans when implementing multiple command and operators, we need the output of a commd to be in a string
// to achive this we redirect the output to a fd that is piped (basically the stdout is closed)






// global or static variables must init with compile time conatanst (numbers, addres to other global, NULL)

char* handle_single_command (char** parsed, circularArr_t* ca, int* st, int flag);


char* process_input(char* main_input, int main_size, circularArr_t* ca, int* status, int flag) {

		if (main_size == 0){
			perror ("empty input");
			*status = -1;
			return NULL;
		}
		 // for handling single command
		 int tok_size = 0;
		char** parsed = parse(main_input, (int)main_size, &tok_size);
		if (!parsed) {
			perror ("input didnot parsed !!");
			*status = -1;
			return NULL;
		}

		// probelm -> <space> <operator> <command>
		// in the above case -> parser thinks <space> as a separate command
		// this creates problem in execute fun (in the stack) as there are spaces in the place of actual command !
		// sol ->
		// remove the entries with no actual command (only spaces and one \n at the end)

	//	char** parsed = sanitise (toks, tok_size);
	//	char** t = parsed;
//		while (strcmp (*t, "")){
//			printf ("%s\n", *t);
//			t++;
//
//		}
//			//printf ("hii");

		if (!parsed[0]){

			perror ("invalid command");
			*status = -1;
			return	NULL;

		}




		if (tok_size == 1){
			int st = 0;
			char* output = handle_single_command (parsed, ca, &st, flag);
	//		clean2Dstring (parsed, 0, MAXNUM_COMMAND);
			free (parsed[0]) ;

			if (!st){
				if (NOT_MINI_SHELL == flag)
					push_crclArr(ca, main_input);
				*status = 0;
				return output;
			}
			*status = -1;
			if (output) free (output);
			return NULL;
		}

		char** postfix = postfix_conversion (parsed);

	//	char ** post = postfix;
	//	while (*post){

	//	printf ("%s\n", *post);
	//		post++;
	//	}

		clean2Dstring(parsed, 0, MAXNUM_COMMAND);
//



		char* exe = execute (postfix, ca, status);
		if (*status){
			if (exe) free (exe);
				return NULL;
		}
		else
			if (flag == NOT_MINI_SHELL)
				push_crclArr(ca, main_input);
	//	exe = NULL;
		clean2Dstring (postfix, 0,  MAXNUM_COMMAND);

	return exe;
}





char* handle_single_command (char** parsed, circularArr_t* ca, int* status, int flag) {

		int fd_single_command [2], stdout_cp = 0;
	if (flag == MINI_SHELL){
		pipe2 (fd_single_command, O_NONBLOCK);
		stdout_cp = dup (STDOUT_FILENO);
		assert (dup2(fd_single_command[1], STDOUT_FILENO) == 1);
	}

//			// no operator !
			int size_ = 0;
			char** arg = tokenise(strdup(parsed[0]), " \n", &size_);
		//	char** arg_it = arg;
//			while (*arg_it){
//				printf ("%s\n", *arg_it);
//				arg_it++;
//
//			}
			if (size_ == 2 && !strcmp ("cd", arg[0])){

				if (chdir (arg[1])){
					perror ("cd");

					clean2Dstring (arg, 0, size_);
	//				clean2Dstring (parsed, 0, MAXNUM_COMMAND);

					if (flag == MINI_SHELL){
						dup2(stdout_cp, STDOUT_FILENO);
						close (stdout_cp) ;
						close (fd_single_command[0]);
						close (fd_single_command[1]);
					}

					*status = -1;
					return NULL;
				}
				clean2Dstring (arg, 0, size_);
	//			clean2Dstring (parsed, 0, MAXNUM_COMMAND);

				if (flag == MINI_SHELL){
					dup2 (stdout_cp, STDOUT_FILENO);

					close (stdout_cp);
					close(fd_single_command[0]);
					close(fd_single_command[1]);
				}

				*status = 0;
				return NULL;
			}

			int f = fork();
			if (f== 0){

				if (!builtin (arg, size_,  ca)){
					exit (0);
				}

				execvp (arg[0], arg);
				perror ("command not found");
				exit (1);

			}
			else if (f > 0){
				int st = 0;

				waitpid (f, &st, 0);

				if (st){
					perror ("single command execution");
					clean2Dstring (arg, 0, size_);
	//				clean2Dstring (parsed, 0, MAXNUM_COMMAND);

					if (flag == MINI_SHELL){
						dup2 (stdout_cp, STDOUT_FILENO);

						close (stdout_cp);
						close(fd_single_command[0]);
						close(fd_single_command[1]);
					}
					*status = -1;
					return NULL;
				}

			}
			else
				exit (1);

			// clean shit
			clean2Dstring (arg, 0, size_);

			if (flag == MINI_SHELL){

				char* output = calloc (MAXLEN_OUTPUT , sizeof (char));
				read (fd_single_command[0], output, MAXLEN_OUTPUT);
				dup2 (stdout_cp, STDOUT_FILENO);

				close (stdout_cp);
				close(fd_single_command[0]);
				close(fd_single_command[1]);


				*status = 0;
				return output;

			}

			*status = 0;
			return NULL;
}


circularArr_t* ca;


int main  (){


circularArr_t* ca = calloc (1, sizeof (circularArr_t));
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

		char* main_input = calloc (MAXLEN_INPUT, sizeof (char));
		char input[MAXLEN_INPUT];

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




		if (error_found) {
			perror ("error found inside the for loop !!");
			continue;
		}

		// handling << -> done


		 main_size = strlen (main_input);

		 int st = 0;
		 char* output = process_input(main_input, main_size, ca, &st, NOT_MINI_SHELL);
		 if (!st && output){
			printf ("%s", output);
		 }
		 if (output) free (output);


	}
	return 0;
}
