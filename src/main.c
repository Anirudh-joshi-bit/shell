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

const char* commands[] = {
	"ls",			"cd",			"grep",			 "clear",			"history",			"ps",
	"vim",			"find",			"fzf",			 "exit",			"htop",				"btop",
	"man",			"touch",		"mkdir",		 "rm",				"rmdir",			"make",
	"make all",		"valgrind",	"gdb",			 "sudo",			"./build/SHELL_D",	"nat",
	NULL
};



// global or static variables must init with compile time conatanst (numbers, addres to other global, NULL)

char* command_generate (const char* text, int state){

	// this function will be called repeatedly by the readline internals
	// readline expoects a string with prefix same as text each time this function is called
	// or NULL if there is no matching string
	// returning NULL will notify readline to stop calling this function;


	static int index, len;
	if (state == 0){
		// this is the first call => set the value of variables
		index = 0;
		len = strlen (text);
	}

	// find a matching string ->
	while (commands[index]){
		const char* cmd = commands[index];
		index++;
		if (!strncmp (cmd, text, len)){

			return strdup(cmd);

		}
	}
	return NULL;
}



char** command_completion (const char* text, int start, int end){

	if (start == 0) {
		return rl_completion_matches (text, command_generate);
	}

	// if text is not the first word -> do the default (autocomplete by file/dir name);
	return NULL;
}


char* get_input (){
	rl_attempted_completion_function = command_completion;
	char* input = readline ("");

	return input;
}



char* process_input(char* main_input, int main_size, circularArr_t* ca, int* status, int flag) {

		if (main_size == 0){
			PERROR ("empty input");
			*status = -1;
			return NULL;
		}
		 // for handling single command
		 int tok_size = 0;
		char** parsed = parse(main_input, (int)main_size, &tok_size);
		if (!parsed) {
			PERROR ("input didnot parsed !!");
			*status = -1;
			return NULL;
		}

		// probelm -> <space> <operator> <command>
		// in the above case -> parser thinks <space> as a separate command
		// this creates problem in execute fun (in the stack) as there are spaces in the place of actual command !
		// sol ->
		// remove the entries with no actual command (only spaces and one \n at the end)
		// this is handled by the parse function internally (no need to use a different function)

//		char** t = parsed;
//		while (strcmp (*t, "")){
//			printf ("%s\n", *t);
//			t++;
//
//		}

		if (!parsed[0]){
			free (parsed);
			PERROR ("invalid command");
			*status = -1;
			return	NULL;

		}




		if (tok_size == 1){
			int st = 0;
			char* output = handle_single_command (parsed, ca, &st, flag);
			clean2Dstring (parsed, 0, tok_size);

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

		clean2Dstring(parsed, 0, tok_size);
//

		if (!postfix){
			return NULL;

		}


		char* exe = execute (postfix, ca, status);
		if (*status){
			if (exe) free (exe);
			clean2Dstring (postfix, 0,  MAXNUM_COMMAND);

			return NULL;
		}
		else
			if (flag == NOT_MINI_SHELL)
				push_crclArr(ca, main_input);
	//	exe = NULL;
		clean2Dstring (postfix, 0,  MAXNUM_COMMAND);

	return exe;
}

circularArr_t* ca;


int main  (){


	// in parent process (shell) ignore the sigint signal and set default action in child
	// process


	signal (SIGINT, SIG_IGN);

circularArr_t* ca = calloc (1, sizeof (circularArr_t));
ca_init (ca, MAXNUM_HISTORY);

const char *art =
"███████╗██╗  ██╗███████╗██╗     ██╗     \n"
"██╔════╝██║  ██║██╔════╝██║     ██║     \n"
"███████╗███████║█████╗  ██║     ██║     \n"
"╚════██║██╔══██║██╔══╝  ██║     ██║     \n"
"███████║██║  ██║███████╗███████╗███████╗\n"
"╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n";
printf("\n" MAGENTA "%s" RESET "\n", art);
printf (GREEN"welcome to SHELL"RESET"\n");



	while (true){

		char pwd[MAXLEN_PWD];
		getcwd (pwd, MAXLEN_PWD);

		 printf (CYAN "__%s__ $ \n" RESET, pwd);

		char* main_input = calloc (MAXLEN_INPUT, sizeof (char));
		char* input;

		int main_size = MAXLEN_INPUT, main_iter = 0, size = MAXLEN_INPUT, error_found = 0;



		//fgets (input, size, stdin);


		// tab completion start

		input = get_input ();

	    if (*input) add_history(input);


		// fgets reads \n so the whole design was made keeping this point in mind
		// but readline does not read \n
		//				i could change the parser and postfix conv fun
		//				or just append a /n char into the output of readline function

		input = realloc (input, strlen (input) + 2);
		strcat (input, "\n");

		// tab completion done


		if (!strncmp (input, "exit", strlen ("exit"))){
			free (main_input);
			free(input);
			break;
		}

		size = strlen (input);

		// handling <<

		for (int i=0; i<size-1; i++){
			if (main_iter >= main_size) {
				PERROR ("too big input");
				error_found = 1;
				break;

			}
			if (input[i] == '<' && input[i+1] == '<'){

				if (main_iter >= main_size-1){
					PERROR ("too big of a command");
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
					PERROR ("no delimeter found !!");
					error_found = 1;
					break;
				}
				while (i<size){

					if (input[i] == ' ') {
						error_found =  1;
						PERROR ("error in delimeter in << ");
						break;

					}
					if (del_iter >= 19){
						error_found = 1;
						PERROR ("error ! too big delimeter ");
						break;
					}
					delim [del_iter++] = input[i++];
				}

				if (error_found){
					break;
				}
				delim[del_iter] = '\0';
				printf (CYAN "delim = %s" RESET "\n", delim);

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
						PERROR ("too big input !!!");
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

		free (input);



		if (error_found) {
			PERROR ("error found inside the for loop !!");
			free (main_input);
			continue;
		}

		// handling << -> done

		 main_size = strlen (main_input);

		 int st = 0;
		 char* output = process_input(main_input, main_size, ca, &st, NOT_MINI_SHELL);
		 if (!st && output){
			printf ( "%s\n", output);
		 }
		 if (output) free (output);

		free (main_input);
	//	free (input);

	}

	rl_clear_history();
	cleanCA(ca);
	return 0;
}
