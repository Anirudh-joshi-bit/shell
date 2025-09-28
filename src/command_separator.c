#include "../include/common.h"
#include "../include/defines.h"
#include "../include/stack.h"
#include "../include/helper.h"

	int	command_separator(char* operand1, char* operand2, char** postfix, stack_t_* st){

		//case 1 -> operand1 = NULL , operand2 = command | o/p of another command;
		//case 2 -> operand1 = command, operand2 = command | o/p
		//case 3 -> operand1 = o/p , operand2 = command
		// there is no othe cases !





		int f1=0, f2=0;
		if (operand1) {
			// as operand1 can be null and if it is null, we dont want to exec for operand2

				f1 = fork ();

				if (f1 == 0){

					signal (SIGINT, SIG_DFL);


				int size_1 = 0;
				char** args_1 = tokenise (operand1, " \n\t", &size_1);
					execvp (args_1[0], args_1);


					PERROR ("command not found");
					exit (1);

				}
				else if (f1 > 0){
					int st = 0;
					waitpid (f1, &st, 0);
					if (st){
						// operand1 is a output of another command
						write (STDOUT_FILENO, operand1, strlen(operand1));

					}

				}
				else {
					PERROR ("FORK in ; ");
					exit (1);

				}
				// clean args_1 once operand1 is processed
			}



		f2 = fork ();

		if (f2 == 0){

			signal (SIGINT, SIG_DFL);

			int size_2 = 0;
			char** args_2 = tokenise (operand2, " \n\t", &size_2);

			execvp (args_2[0], args_2);

			exit (1);

		}
		else if (f2 > 0){

			int st = 0;
			waitpid (f2, &st, 0);
			// if operand1 is null only in that case -> operand2 is alowed to not be a command
			if (st){
				write (STDOUT_FILENO, operand2, strlen (operand2));

			}
			// clean args_2 after operand2 is processsed

		}
		else {
			PERROR ("FORK in ;");
			exit (1);
		}

		return 0;

	}
