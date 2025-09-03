#include "header.h"

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

int fds[2];
// oepn a file
// execute the program
// put output into the file
// read the file (filter it and remove all the /n)
// insert the content of that file (i.e a string) into the stack
// continue this process

// to open a file use "open()" syscall that returns the file descriptor of that opened file
// pipe the output os command into that file using "pipe()"

int input_redir_command (char* operand1, char* operand2){
	int size = 0;
	if (operand1 == NULL || operand2 == NULL){
		perror ("ERROR bad command and operations \n");
		return -1;
	}
	char** file_name = tokenise (operand2, " ", &size);
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
	int stdin_cp = dup(STDIN_FILENO);

	if (stdin_cp < 0){
		close (fd_file);
		perror ("error in dup STDIN_FILENO");

		return -1;
	}

	if (dup2 (fd_file, STDIN_FILENO) < 0) {
		perror ("error in dup2");
		close (fd_file);
		close (stdin_cp);
		return -1;
	}

	int f = fork ();

	if (f == 0) {				// child proces

		char** tokens = tokenise (operand1, " \n\t", &size);
		execvp (tokens[0], tokens);

		perror ("command not found !! \n");
		exit (-1);

	}
	else if (f > 0){

		int st;
		wait (&st);
		printf ("this is the default print statement");


		close (fd_file);
		if (dup2 (stdin_cp, STDIN_FILENO) < 0){
			perror ("error in parent dup2 in input redir command\n");
			return -1;
		}
		close (stdin_cp);
	}
	else {				// error in fork !!

		perror ("ERROR in fork in input redirection\n");
		if (dup2 (stdin_cp, STDIN_FILENO) < 0){
			perror ("ERROR in dup2\n");
			close (fd_file);
			return -1;
		}
		close (fd_file);
		return -1;
	}

	return 0;
}

int output_command (char* operand1, char* operand2 , int flag){


	int tok_size  = 0;
	char** file_name = tokenise (operand2, " \n\t", &tok_size);

	if (tok_size > 1){

		perror("ERROR multiple file name \n");
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

		perror ("ERROR file not found");
		return -1;
	}

	int stdout_cp = dup (STDOUT_FILENO);
	if (stdout_cp < 0){

		perror ("ERROR in creating copy of STDOOUT_FILENO inside output_redir_command function \n");
		close (stdout_cp);
		exit(1);

	}

	if (dup2 (output_file_fd, STDOUT_FILENO) < 0){
		perror ("ERROR in opening the file in place of STDOUT_FILENO\n");
		close (output_file_fd);
		close (stdout_cp);
		return -1;
	}

		int f = fork ();
		if (f < 0){

			// error

			perror ("ERROR in fork in output redir !\n");
			close (output_file_fd);
			dup2 (stdout_cp, STDOUT_FILENO);
			close (stdout_cp);
			return -1;
		}
		else if (f == 0){

			// child process;

			char** args = tokenise (operand1, " \n\t", &tok_size);
			execvp (args[0], args);

			perror ("ERROR command not found\n");
			exit (1);

		}
		else {

			// parent process

			int st = 0;
			wait (&st);
		}

		close (output_file_fd);
		dup2(stdout_cp, STDOUT_FILENO);
		close (stdout_cp);
		return 0;

	}

	int here_document_command (char* operand1, char* operand2){
		printf ("this is here_document_command\n");
		return 0;

	}

	int pipe_command (char* operand1, char* operand2){
		printf ("this is pipe_command\n");
		return 0;

	}

	int background_exe_command (char* operand1, char* operand2){
		printf ("this is background_exe_command\n");
		return 0;

	}


	char* launch_command (char* operator, char* operand1, char* operand2){

		printf ("inside the launch command \n");
		// we can use multiple pipes to a single fds[2]   no worries
		pipe2(fds, O_NONBLOCK);


		// dup will open the file present in STDOUT_FILENO in a different fd
		// that fd is stdout_cp
		int stdout_cpy = dup(STDOUT_FILENO);
		if (stdout_cpy < 0){
			perror ("ERROR in stdout_cpy in launch command \n");
			close (fds[0]);
			close (fds[1]);
			return NULL;
		}
		// set the stdout to fd 1 so that output can be read
		// child will copy the fds meaning if fds[0] is opened in fd 0 as parent child will also open fds[0] in fd 0
		if (dup2 (fds[1], STDOUT_FILENO )< 0){
			perror ("ERROR in dup2 fd[1] in launch command \n");
			close (fds[0]);
			close (fds[1]);
			close (stdout_cpy);
			return NULL;
		}

		char* output = malloc (MAXLEN_OUTPUT);
		if (!strcmp (operator, "<")) {
			if (input_redir_command(operand1, operand2)){
				perror ("ERROR in input redir\n");
				close (fds[1]);
				close (fds[0]);
				dup2 (stdout_cpy, STDOUT_FILENO);
				close (stdout_cpy);
				return NULL;
			}
		}
		else if (!strcmp (operator, ">")){

			output_command(operand1, operand2, TRUNC_OUTPUT);
		}
		else if (!strcmp (operator, ">>")) {

			output_command(operand1, operand2, APPEND_OUTPUT);
		}
		else if (!strcmp (operator, "<<")) {

			here_document_command(operand1, operand2);
		}
		else if (!strcmp (operator, "|")) {

			pipe_command(operand1, operand2);
		}
		else if (!strcmp (operator, "&")) {

			background_exe_command(operand1, operand2);
		}

	read (fds[0], output, MAXLEN_OUTPUT);

	close (fds[0]);
	close (fds[1]);

	if (dup2 (stdout_cpy, STDOUT_FILENO) < 0){			// this will reopen the default stdout
		perror ("ERROR in dup2 stdout_cpy\n");
		exit (1);
	}

	if (close (stdout_cpy) < 0){
		perror ("ERROR in close (stdout_cpy) in launch command \n");
		return	NULL;
	}

		printf ("this is after restoring the stdout \n");
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

	stack_t_ st;
	stack_init (&st);

	char** post_iter = postfix;

	while (strcmp ("", *post_iter)) {
		if (isOper (*post_iter)){
			char *operand1 = NULL, *operand2 =	NULL;

			if (st.size)
				operand2 = stack_top (&st);
			if (operand2){
				stack_pop(&st);
				if (st.size){
					operand1 = stack_top(&st);
					stack_pop(&st);
				}
				char* output = launch_command (*post_iter, operand1, operand2);
				if (output)  stack_push(&st, output);
				if (operand1) free(operand1);
				if (operand2) free(operand2);
				free(*post_iter);

			}
			else {
				perror ("ERROR      operand2 is null\n");
				if (operand1) free(operand1);
				if (operand2) free(operand2);
				free(*post_iter);
				return 1;
			}
		}
		else {
			stack_push (&st, *post_iter);
		}
		post_iter ++;

	}

	if (st.size > 1){
		perror ("ERROR.... bad command !\n");
		return -1;
	}

	if (st.size == 0) {
		return 0;
	}
	printf ("%s\n", stack_top (&st));

	// handle single command ->
	return 0;

}
