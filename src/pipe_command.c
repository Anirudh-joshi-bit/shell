#include "../include/common.h"
#include "../include/defines.h"
#include "../include/stack.h"
#include "../include/helper.h"

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

		pipe2 (fds_pipe_command, O_NONBLOCK);

		int f1=0, f2=0;
		f1 = fork();

		if (f1 == 0){

			assert (dup2 (fds_pipe_command[1], STDOUT_FILENO) >= 0);
			close (fds_pipe_command[1]);
			close (fds_pipe_command[0]);

			int size = 0;
			char** args = tokenise (strdup(operand1), " \n\t", &size);

			clean2Dstring (postfix, 0, MAXNUM_COMMAND);
			while (st->size) stack_pop(st);

			execvp (args[0], args);

			// out builtin functions doesnot take anything from stdin => we dont have to
			// dod the same thing inside processing of operand2


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
			char** args = tokenise (strdup(operand2), " \n\t", &size);

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


	//	waitpid(f1, NULL, 0);
		waitpid(f2, NULL, 0);


		return 0;
	}
