#include "header.h"


// for history ->
// op1 doublly linked list -> delete, insert -> O(1)
//							  traverse , access -> O(n) !
//
//
//op2 queue -> delete, insert -> O(1)
//				traverse, access -> O(n)
//
//op3 circular arr -> delete, insert, access -> O(1)
//						traverse -> O(n)
//
//op3 -> choosen


int history_command (char** args, circularArr_t* ca){

	if (args[1] == NULL){

		// print the whole history
		return print_history (ca, -1);

	}
	else {

		// strtol -> converts the string into long
		// end points to the first character that is not a digit
		// it ignores all the preseeding spaces

		char* end = NULL;
		long ind = strtol (args[1], &end, 10);

		// end == input string if string is invalid
		if (!strcmp (end, args[1])) return -1;

		// O(n) !
		// instead use a char** with circle !

		return print_history (ca, ind);
	}

	return 0;

}

int builtin (char** args, circularArr_t* ca){
	if (!args[0]) return -1;

	int size = 0;

	if (!strcmp (args[0], "history")){
		if (size > 2) return -1;

		return history_command (args, ca) ;

	}
	else if (!strcmp (args[0], "cd")){



	}

	clean2Dstring (args, 0, size);

	return -1;

}
