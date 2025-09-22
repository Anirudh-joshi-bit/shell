#include "../include/common.h"
#include "../include/defines.h"
#include "../include/stack.h"
#include "../include/helper.h"



int here_document_command (char* operand1, char* operand2, char** postfix, stack_t_* st){

		int f = fork ();
		if (f == 0){

			signal (SIGINT, SIG_DFL);

			int fd = open ("/tmp", O_TMPFILE | O_RDWR | O_EXCL);
			if (fd < 0){
				perror ("open in heredoc command");
				exit (-1);
			}
			fsync (fd);

			write (fd, operand2, strlen (operand2));
			lseek (fd, 0, SEEK_SET);

			assert (dup2 (fd , STDIN_FILENO) == 0);

			// we could just write operand2 into STDIN_FILENO instead of creating a tmp file


			int size = 0;
			char** args = tokenise (strdup(operand1), " \n\t", &size);

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
