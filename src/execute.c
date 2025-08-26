#include "header.h"

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



	char* output = NULL;
	if (!strcmp (operator, "<")) {

	}
	if (!strcmp (operator, ">")){

	}
	if (!strcmp (operator, ">>")) {

	}
	if (!strcmp (operator, "<<")) {

	}
	if (!strcmp (operator, "|")) {

	}
	if (!strcmp (operator, "&")) {

	}
	return output ;
}

/*
 *	<
 *		input redirection
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


char* execute (char* operand1,  char* operand2, char* operator){

	return NULL;
}
