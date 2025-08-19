#include "header.h"

// oepn a file
// execute the program
// put output into the file
// read the file (filter it and remove all the /n)
// insert the content of that file (i.e a string) into the stack
// continue this process

// to open a file use "open()" syscall that returns the file descriptor of that opened file
// pipe the output os command into that file using "pipe()"

char* pipe_command (char* operand1, char* operand2, char* operator){

	return NULL;

}

//  > ->
//  this is the output redirection operator in shell
//  what it does is this sends the standard output to a file => the second operand must be a file
//
//  >> ->
//
//  < ->
//
//
//  << ->
//		here document operator
//		this is a tricky one     it does not take a single line
//		it takes the line startinf from a delimiter and read all the lines until
//		the next delimeter !! meaning we cannot just pass aa single line to the
//		command
//
//		how can we handle this case !!
//		1. while taking input, check if the command is << or not
//		if yes  then take the first word and make it as the delimeter
//		take each line as input until the next << is not encountered
//
//
//		but it can be used for something like -> cat <<EOF
//														line1
//														line2
//														EOF | grep "line1"
//		this can be handled by checking if there is << present in the current
//		input line if yes then make a while loop until ' ' or '\0' if space is
//		found then print an error message
//
//		if this is the null char then make a while loop until hext delimeter if
//		not found !
//		then insert each char into a string and pass it to the respective fun



char* execute (char* operand1,  char* operand2, char* operator){

	return NULL;
}


int process_postfix (char* postfix, int size){
	// this is a problem !!! donot loose the data and make a stirng (postfix) then parse it !!!
	char** tok = parse (postfix, size);
	char** tok_iter = tok;
	stack_t* st;

	while (*tok_iter){

		char *operand1 = NULL, *operand2 = NULL;
		if (isOper (*tok_iter)){
			operand2 = stack_top (st);
			stack_pop(st);
			if (st->size){
				operand1 = stack_top (st);
				stack_pop(st);
			}

			char* output = execute (operand1, operand2,* tok_iter);
			stack_push (st, output);
		}
		tok_iter++;
	}
	return 0;
}
