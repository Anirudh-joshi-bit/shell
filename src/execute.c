#include "header.h"




int fds[2];
// oepn a file
// execute the program
// put output into the file
// read the file (filter it and remove all the /n)
// insert the content of that file (i.e a string) into the stack
// continue this process

// to open a file use "open()" syscall that returns the file descriptor of that opened file
// pipe the output os command into that file using "pipe()"

void input_redir_command (char* operand1, char* operand2){

}

void output_redir_command (char* operand1, char* operand2){

}

void append_output_command (char* operand1, char* oprand2){

}

void here_document_command (char* operand1, char* operand2){

}

void pipe_command (char* operand1, char* operand2){

}

void background_exe_command (char* operand1, char* operand2){

}


char* launch_command (char* operator, char* operand1, char* operand2){

	// we can use multiple pipes to a single fds[2]   no worries
	pipe(fds);


	// dup will open the file present in STDOUT_FILENO in a different fd
	// that fd is stdout_cp
	int stdout_cpy = dup(STDOUT_FILENO);
	// set the stdout to fd 1 so that output can be read
	// child will copy the fds meaning if fds[0] is opened in fd 0 as parent child will also open fds[0] in fd 0
	dup2 (fds[1], STDOUT_FILENO);



	char* output = NULL;
	if (!strcmp (operator, "<")) {
		input_redir_command(operand1, operand2);
	}
	if (!strcmp (operator, ">")){

		output_redir_command(operand1, operand2);
	}
	if (!strcmp (operator, ">>")) {

		append_output_command(operand1, operand2);
	}
	if (!strcmp (operator, "<<")) {

		here_document_command(operand1, operand2);
	}
	if (!strcmp (operator, "|")) {

		pipe_command(operand1, operand2);
	}
	if (!strcmp (operator, "&")) {

		background_exe_command(operand1, operand2);
	}

	read (fds[0], output, MAXLEN_OUTPUT);
	close (fds[0]);
	close (fds[1]);

	dup2 (stdout_cpy, 1);				// this will reopen the default stdout
	close (stdout_cpy);
	return output ;
}

/*
 *	<
 *		input redirection3
 *		redirects file into a program stdinput
 *
 *	>
 *		output rediretction
 *		reditect the output of a program to a file
 *
 *	>>
 *		append output
 *		append the output to a file
 *
 *	<<
 *		here document
 *		everyt thing until the delim is passed as stdin to the command
 *
 *	|
 *		pipe
 *		passes the output of a command as input of another command
 *
 *	&
 *		background exe
 *		exe in the background
 *
 */

int execute (char** postfix){

	stack_t st;
	stack_init (&st);

	char** post_iter = postfix;

	while (post_iter) {

		if (isOper (*post_iter)){
			char *operand1 = NULL, *operand2 =	NULL;

			operand2 = stack_top (&st);
			if (operand2){
				operand1 = stack_top(&st);
				char* output = launch_command (*post_iter, operand1, operand2);

				if (output)  stack_push(&st, output);
			}
			else return 1;
		}
		else {
			stack_push (&st, *post_iter);
		}
		post_iter ++;

	}

	return 0;

}
