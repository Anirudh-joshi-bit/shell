#include "../include/common.h"
#include "../include/defines.h"
#include "../include/helper.h"

int input_redir_command (char* operand1, char* operand2, char** postfix, stack_t_* st){

	// here the operand2 must be a file, no other cases
	// operand2 cannot be NULL or output of another command !



	int size = 0;
	if (operand1 == NULL || operand2 == NULL){
		perror ("ERROR bad command and operations \n");
		return -1;
	}
	char** file_name = tokenise (strdup(operand2), " ", &size);
	if (size > 1){
		perror ("ERROR too many file names !! \n");
		clean2Dstring(file_name, 0, size);
		return -1;
	}

	int fd_file = open ( file_name[0], O_RDONLY);

	if (fd_file < 0){
		perror ("file does not exits \n");
		clean2Dstring(file_name, 0, size);
													 	// MAXLEN_COMMAND = size of
		return -1;										// 2d string returned by tokenise
	}
	clean2Dstring (file_name, 0, size);

	int f = fork ();

	if (f == 0) {				// child proces

		signal (SIGINT, SIG_DFL);


		// replace the default stdin -> file_name;
		assert (dup2 (fd_file, STDIN_FILENO) >= 0);
		close (fd_file);

		char** args = tokenise (strdup(operand1), " \n\t", &size);


		clean2Dstring (postfix, 0, MAXNUM_COMMAND);
		while (st->size) stack_pop(st);

		execvp (args[0], args);

		perror ("command not found !! \n");
		clean2Dstring (args, 0, size);
		exit (-1);
	}
	else if (f > 0){

		int st;
		wait (&st);

	}
	else {				// error in fork !!

		perror ("ERROR in fork in input redirection\n");
		exit (-1);
	}
	return 0;
}
