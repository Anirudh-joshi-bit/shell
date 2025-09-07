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

int input_redir_command (char* operand1, char* operand2, char** postfix, stack_t_* st){
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
	clean2Dstring (file_name, 0, size);

	int f = fork ();

	if (f == 0) {				// child proces

		// replace the default stdin -> file_name;
		assert (dup2 (fd_file, STDIN_FILENO) >= 0);
		close (fd_file);

		char** args = tokenise (operand1, " \n\t", &size);

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

int output_command (char* operand1, char* operand2 , int flag, char** postfix, stack_t_* st){


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

	int f = fork ();
	if (f < 0){
			// error
		perror ("ERROR in fork !!");
		exit (-1);

	}
	else if (f == 0){

			// child process;
		assert (dup2 (output_file_fd, STDOUT_FILENO));
		close (output_file_fd);

		char** args = tokenise (operand1, " \n\t", &tok_size);

		clean2Dstring (postfix, 0, MAXNUM_COMMAND);
		while (st->size) stack_pop(st);

		execvp (args[0], args);

		perror ("ERROR command not found\n");
		exit (1);

	}
	else {

			// parent process
		int st = 0;
		wait (&st);
	}
	return 0;
}

int here_document_command (char* operand1, char* operand2, char** postfix, stack_t_* st){

		int f = fork ();
		if (f == 0){

			int fd = open ("/tmp", O_TMPFILE | O_RDWR | O_EXCL);
			if (fd < 0){
				perror ("open in heredoc command");
				exit (-1);
			}
			fsync (fd);

			write (fd, operand2, strlen (operand2));
			lseek (fd, 0, SEEK_SET);

			assert (dup2 (fd , STDIN_FILENO) == 0);


			int size = 0;
			char** args = tokenise (operand1, " \n\t", &size);

			execvp (args[0], args);

			perror ("execvp heredoc");
			exit(1);

		}
		else if (f > 0){
			int st = 0;
			wait (&st);
			if (st){
				perror ("heredoc");
				return -1;
			}


		}
		else if (f < 0){
			perror ("fork in heredoc");
			exit (-1);
		}
		return 0;

	}

int pipe_command (char* operand1, char* operand2, char** postfix, stack_t_* st){

	// case one ->
	// op1 = writer
	// op2 = reader
	// operand1 is a command and writes the output to fd1 and operand2 (must be a command)
	// reads from fd0


	// case2 ->
	// operand1 is not a command but output of a different command , operand2 is a command wanting to read
	// in this case we can make a tmpfile and call the second program to read from it;

		int fds_pipe_command [2];
		// operand1 -> command1
		// operand2 -> command2

		pipe (fds_pipe_command);

		int f1=0, f2=0;
		f1 = fork();

		if (f1 == 0){

			assert (dup2 (fds_pipe_command[1], STDOUT_FILENO) >= 0);
			close (fds_pipe_command[1]);
			close (fds_pipe_command[0]);

			int size = 0;
			char** args = tokenise (operand1, " \n\t", &size);

			clean2Dstring (postfix, 0, MAXNUM_COMMAND);
			while (st->size) stack_pop(st);

			execvp (args[0], args);
			// meaning command not found !!
			perror ("ERROR ... command not found\n");
			exit (-1);

		}
		else if (f1 > 0){
			int st = 0;
			waitpid (f1, &st, 0);
			if (st){
				// meaning operand1 is not a command

				write (fds_pipe_command[1], operand1, strlen (operand1));

			}

		}

		else if (f1<0){
			perror ("ERROR ... fork failed in pipecommand\n");
			exit (1);

		}

		f2 = fork ();

		if (f2 == 0){

			assert (dup2 (fds_pipe_command[0], STDIN_FILENO) >= 0);
			close (fds_pipe_command[0]);
			close (fds_pipe_command[1]);

			int size = 0;
			char** args = tokenise (operand2, " \n\t", &size);

			clean2Dstring (postfix, 0, MAXNUM_COMMAND);
			while (st->size) stack_pop(st);

			execvp (args[0], args);

			perror ("ERROR ... command not found\n");
			exit (-1);
		}
		else if (f2 < 0){
			perror ("ERROR ... fork failed in pipecommand\n");
			exit (1);
		}

		// first close both the end as parent then weait for child else halt !
		close (fds_pipe_command[0]);
		close (fds_pipe_command[1]);


		waitpid(f1, NULL, 0);
		waitpid(f2, NULL, 0);


		return 0;
	}

	int background_exe_command (char* operand1, char* operand2, char** postfix, stack_t_* st){
		printf ("this is background_exe_command\n");
		return 0;

	}


	char* launch_command (char* operator, char* operand1, char* operand2, char** postfix, stack_t_* st){

		// we can use multiple pipes to a single fds[2]   no worries
		pipe2(fds, O_NONBLOCK);
		//pipe (fds);


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
		close (fds[1]);

		if (!strcmp (operator, "<")) {
			if (input_redir_command(operand1, operand2, postfix, st)){
				perror ("ERROR in input redir\n");
				close (fds[1]);
				close (fds[0]);
				dup2 (stdout_cpy, STDOUT_FILENO);
				close (stdout_cpy);
				return NULL;
			}
		}
		else if (!strcmp (operator, ">")){

			if (output_command(operand1, operand2, TRUNC_OUTPUT, postfix, st)){
				perror ("ERROR in ouput_redir_command\n");
				close (fds [1]);
				close (fds[0]);
				dup2 (stdout_cpy, STDOUT_FILENO);
				close (stdout_cpy);
			}
		}
		else if (!strcmp (operator, ">>")) {
			if (output_command(operand1, operand2, APPEND_OUTPUT, postfix, st)){
				perror ("ERROR in ouput_redir_command\n");
				close (fds [1]);
				close (fds[0]);
				dup2 (stdout_cpy, STDOUT_FILENO);
				close (stdout_cpy);
			}
		}
		else if (!strcmp (operator, "<<")) {

			if (here_document_command(operand1, operand2, postfix, st)){
				perror ("heredoc");
			}
		}
		else if (!strcmp (operator, "|")) {

			pipe_command(operand1, operand2, postfix, st);
		}
		else if (!strcmp (operator, "&")) {

			background_exe_command(operand1, operand2, postfix, st);
		}

	char* output = calloc (MAXLEN_OUTPUT, sizeof (char));
	read (fds[0], output, MAXLEN_OUTPUT);

	close (fds[1]);

	if (dup2 (stdout_cpy, STDOUT_FILENO) < 0){			// this will reopen the default stdout
		perror ("ERROR in dup2 stdout_cpy\n");
		exit (1);
	}

	if (close (stdout_cpy) < 0){
		perror ("ERROR in close (stdout_cpy) in launch command \n");
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
 */

int execute (char** postfix){

	stack_t_ st;
	stack_init (&st);

	char** post_iter = postfix;

	while (strcmp ("", *post_iter)) {
		if (isOper (*post_iter)){
			char *operand1 = NULL, *operand2 =	NULL;

			if (st.size){
				operand2 = strdup (stack_top (&st));
				stack_pop(&st);
			}
			if (operand2 && st.size){
				operand1 = strdup (stack_top(&st));
				stack_pop(&st);

				// pass postfix and stack to clean them in child processes
				// we dont want the child to hold these blocks
				char* output = launch_command (*post_iter, operand1, operand2, postfix, &st);
				free (operand1);
				free (operand2);

				if (output)  stack_push(&st, output);

			}
			else {
				perror ("ERROR      operand2 is null\n");
				return 1;
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
		perror ("ERROR.... bad command !\n");
		while (st.size){
			stack_pop(&st);
		}
		return -1;
	}

	if (st.size == 0) {
		return 0;
	}
	printf ("%s", stack_top (&st));
	//free (stack_top (&st));
	stack_pop(&st);

	return 0;

}
