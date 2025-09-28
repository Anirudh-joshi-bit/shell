#include "../include/common.h"
#include "../include/defines.h"
#include "../include/circularArr.h"
#include "../include/stack.h"
#include "../include/helper.h"
#include "../include/builtin.h"

#ifndef FDS_EXECUTE
#define FDS_EXECUTE
	extern int fds[2];
#endif



int output_command (char* operand1, char* operand2 , int flag, char** postfix, stack_t_* st, circularArr_t* ca){

	if (operand1 == NULL) return 1;
	// operand1 can be output of a command
	// operand2 will always have to be a file else it wil create one
	// operand2 cannot be NULL nor an output of another command !

	int tok_size  = 0;
	char** file_name = tokenise (strdup(operand2), " \n\t", &tok_size);

	if (tok_size > 1){

		PERROR ("ERROR multiple file name \n");
		clean2Dstring (file_name, 0, tok_size);
		return -1;
	}
	int output_file_fd;

	if (flag == TRUNC_OUTPUT)
		output_file_fd = open (file_name[0],	O_TRUNC | O_CREAT | O_WRONLY , 0777);
	else
		output_file_fd = open (file_name[0], O_CREAT | O_WRONLY | O_APPEND , 0777);

	clean2Dstring (file_name, 0, tok_size);

	if (output_file_fd < 0){

		PERROR ("ERROR file not found");
		return -1;
	}

	int f = fork ();
	if (f < 0){
			// error
		PERROR ("ERROR in fork !!");
		exit (-1);

	}
	else if (f == 0){


		signal (SIGINT, SIG_DFL);


			// child process;
		assert (dup2 (output_file_fd, STDOUT_FILENO));
		close (output_file_fd);

		close (fds[0]);
		close (fds[1]);


		char** args = tokenise (strdup(operand1), " \n\t", &tok_size);


		clean2Dstring (postfix, 0, MAXNUM_COMMAND);
		while (st->size) stack_pop(st);

		execvp (args[0], args);

		if (!builtin (args, tok_size,  ca)){
			// right  !

			exit (0);


		}
		exit (1);

	}
	else {

			// parent process
		int st = 0;
		waitpid (f, &st, 0);
		if (st) {
			write (output_file_fd, operand1, strlen (operand1));

		}

	}
	return 0;
}
