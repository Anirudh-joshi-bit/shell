#include "../include/common.h"
#include "../include/defines.h"
#include "../include/commands.h"
#include "../include/helper.h"


// why we need some thing to be written in fd[1] to be able to read from fd[0]
/*

	if a process wants to read form fd[0] but nothing is written to fds[1],
	in this case, the process (reader) will block until
		1. something is written to fd[1]
		2. all writer close fd[1]

	to solve this probelm, use pipe2 ( fds, O_NOBLOCK );
	to avoid any blocking

	to do so , we ned to define _GNU_SOURCE   (if _GNU_SOURCE is not already declared)
*/



// postfix is not NULL terminating instead "" terminating

#ifndef FDS_EXECUTE
#define FDS_EXECUTE

int fds[2];

#endif

// oepn a file
// execute the program
// put output into the file
// read the file (filter it and remove all the /n)
// insert the content of that file (i.e a string) into the stack
// continue this process

// to open a file use "open()" syscall that returns the file descriptor of that opened file
// pipe the output os command into that file using "pipe()"




	char* launch_command (char* operator, char* operand1, char* operand2, char** postfix, stack_t_* st, circularArr_t* ca){

		// we can use multiple pipes to a single fds[2]   no worries
		pipe2(fds, O_NONBLOCK);
		//pipe (fds);


		// dup will open the file present in STDOUT_FILENO in a different fd
		// that fd is stdout_cp
		int stdout_cpy = dup(STDOUT_FILENO);
		if (stdout_cpy < 0){
			PERROR ("ERROR in stdout_cpy in launch command \n");
			close (fds[0]);
			close (fds[1]);
			return NULL;
		}
		// set the stdout to fd 1 so that output can be read
		// child will copy the fds meaning if fds[0] is opened in fd 0 as parent child will also open fds[0] in fd 0
		if (dup2 (fds[1], STDOUT_FILENO )< 0){
			PERROR ("ERROR in dup2 fd[1] in launch command \n");
			close (fds[0]);
			close (fds[1]);
			close (stdout_cpy);
			return NULL;
		}
		close (fds[1]);

		if (!strcmp (operator, "<")) {
			if (input_redir_command(operand1, operand2, postfix, st)){
				PERROR ("ERROR in input redir\n");
				close (fds[1]);
				close (fds[0]);
				dup2 (stdout_cpy, STDOUT_FILENO);
				close (stdout_cpy);
				return NULL;
			}
		}
		else if (!strcmp (operator, ">")){

			if (output_command(operand1, operand2, TRUNC_OUTPUT, postfix, st, ca)){
				PERROR ("ERROR in ouput_redir_command\n");
				close (fds [1]);
				close (fds[0]);
				dup2 (stdout_cpy, STDOUT_FILENO);
				close (stdout_cpy);
				return NULL;
			}
		}
		else if (!strcmp (operator, ">>")) {
			if (output_command(operand1, operand2, APPEND_OUTPUT, postfix, st, ca)){
				PERROR ("ERROR in ouput_redir_command\n");
				close (fds [1]);
				close (fds[0]);
				dup2 (stdout_cpy, STDOUT_FILENO);
				close (stdout_cpy);
				return NULL;
			}
		}
		else if (!strcmp (operator, "<<")) {

			if (here_document_command(operand1, operand2, postfix, st)){
				PERROR ("heredoc");
				close (fds [1]);
				close (fds[0]);
				dup2 (stdout_cpy, STDOUT_FILENO);
				close (stdout_cpy);
				return NULL;
			}
		}
		else if (!strcmp (operator, "|")) {

			if (pipe_command(operand1, operand2, postfix, st)){
				PERROR ("pipe");
				close (fds [1]);
				close (fds[0]);
				dup2 (stdout_cpy, STDOUT_FILENO);
				close (stdout_cpy);
				return NULL;
			}
		}
		else if (!strcmp (operator, ";")) {

			if (command_separator (operand1, operand2, postfix, st)){
				PERROR ("command_separator");
				close (fds [1]);
				close (fds[0]);
				dup2 (stdout_cpy, STDOUT_FILENO);
				close (stdout_cpy);
				return NULL;
			}
		}

	char* output = calloc (MAXLEN_OUTPUT, sizeof (char));
	read (fds[0], output, MAXLEN_OUTPUT);

	close (fds[1]);

	if (dup2 (stdout_cpy, STDOUT_FILENO) < 0){			// this will reopen the default stdout
		PERROR("ERROR in dup2 stdout_cpy\n");
		exit (1);
	}

	if (close (stdout_cpy) < 0){
		PERROR ("ERROR in close (stdout_cpy) in launch command \n");
		free (output);
		return	NULL;
	}

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
 *	;
 *		command separator
 *		separates commands
 */

char* execute (char** postfix, circularArr_t* ca, int* status){

	stack_t_ st;
	stack_init (&st);
	//stack_push (&st, "(");

	char** post_iter = postfix;

	while (*post_iter) {
		if (isOper (*post_iter)){
			char *operand1 = NULL, *operand2 =	NULL;

			if (st.size){
				operand2 = strdup (stack_top (&st));
				stack_pop(&st);
			}
			if (operand2 ){
				if (st.size){
					operand1 = strdup (stack_top(&st));
					stack_pop(&st);
				}

				// pass postfix and stack to clean them in child processes
				// we dont want the child to hold these blocks
				char* output = launch_command (*post_iter, operand1, operand2, postfix, &st, ca);
				if (operand1) free (operand1);
				free (operand2);

				if (output)  stack_push(&st, output);

			}
			else {
				PERROR ("ERROR operand2 is null\n");
				*status = -1;
				clean_stack(&st);
				return	NULL;
			}
		}
		else {
			stack_push (&st, strdup (*post_iter));			// push a copy of the string
																// as we want to clean the postfix and
																// stack separately inside the child process
																// for simplicity
		}
		post_iter ++;

	}


	if (st.size > 1){
		PERROR ("ERROR.... bad command !\n");
		while (st.size){
			stack_pop(&st);
		}
		*status = -1;
		clean_stack(&st);
		return NULL;
	}

	if (st.size == 0) {
		*status = 0;
		clean_stack(&st);
		return NULL;
	}
	char* ans = strdup (stack_top (&st));
	//free (stack_top (&st));
	stack_pop(&st);
	clean_stack(&st);
	*status = 0;
	return ans;

}
