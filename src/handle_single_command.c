#include "../include/common.h"
#include "../include/utils.h"
#include "../include/defines.h"
#include "../include/helper.h"
#include "../include/builtin.h"


char* handle_single_command (char** parsed, circularArr_t* ca, int* status, int flag) {

		int fd_single_command [2], stdout_cp = 0;
	if (flag == MINI_SHELL){
		pipe2 (fd_single_command, O_NONBLOCK);
		stdout_cp = dup (STDOUT_FILENO);
		assert (dup2(fd_single_command[1], STDOUT_FILENO) == 1);
	}

//			// no operator !
			int size_ = 0;
			char** arg = tokenise(strdup(parsed[0]), " \n", &size_);
		//	char** arg_it = arg;
//			while (*arg_it){
//				printf ("%s\n", *arg_it);
//				arg_it++;
//
//			}
			if (size_ == 2 && !strcmp ("cd", arg[0])){

				if (chdir (arg[1])){
					PERROR ("cd");

					clean2Dstring (arg, 0, size_);
	//				clean2Dstring (parsed, 0, MAXNUM_COMMAND);

					if (flag == MINI_SHELL){
						dup2(stdout_cp, STDOUT_FILENO);
						close (stdout_cp) ;
						close (fd_single_command[0]);
						close (fd_single_command[1]);
					}

					*status = -1;
					return NULL;
				}
				clean2Dstring (arg, 0, size_);
	//			clean2Dstring (parsed, 0, MAXNUM_COMMAND);

				if (flag == MINI_SHELL){
					dup2 (stdout_cp, STDOUT_FILENO);

					close (stdout_cp);
					close(fd_single_command[0]);
					close(fd_single_command[1]);
				}

				*status = 0;
				return NULL;
			}

			int f = fork();
			if (f== 0){

				signal (SIGINT, SIG_DFL);


				if (!builtin (arg, size_,  ca)){
					exit (0);
				}

				execvp (arg[0], arg);
				exit (1);

			}
			else if (f > 0){
				int st = 0;

				waitpid (f, &st, 0);

				if (st){
					PERROR ("single command execution");
					clean2Dstring (arg, 0, size_);
	//				clean2Dstring (parsed, 0, MAXNUM_COMMAND);

					if (flag == MINI_SHELL){
						dup2 (stdout_cp, STDOUT_FILENO);

						close (stdout_cp);
						close(fd_single_command[0]);
						close(fd_single_command[1]);
					}
					*status = -1;
					return NULL;
				}

			}
			else
				exit (1);

			// clean shit
			clean2Dstring (arg, 0, size_);

			if (flag == MINI_SHELL){

				char* output = calloc (MAXLEN_OUTPUT , sizeof (char));
				read (fd_single_command[0], output, MAXLEN_OUTPUT);
				dup2 (stdout_cp, STDOUT_FILENO);

				close (stdout_cp);
				close(fd_single_command[0]);
				close(fd_single_command[1]);


				*status = 0;
				return output;

			}

			*status = 0;
			return NULL;
}
