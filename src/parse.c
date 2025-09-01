#include "header.h"

char** parse (char* input, int size){
	// preblem (was) ->  malloc (MAXNUM_COMMAND) -> wrong
	char** tokens = malloc (MAXNUM_COMMAND * sizeof (char*));

	for (int i=0; i<MAXNUM_COMMAND; i++){
		tokens[i] = malloc (MAXNUM_COMMAND);
	}

	int row = 0, col = 0;
	for (int i=0; i<size-1; i++){
		int val = getOperNum (input[i], input[i+1]);
		if (col >= MAXLEN_COMMAND-1) {
			printf ("too big command !! 1\n");
			clean2Dstring (tokens, 0, MAXNUM_COMMAND);
			return NULL;
		}
		if (!val) tokens[row][col++] = input[i];
		else {
			// problem -> when there are commands like -> <<< , >>>, ||, ;; etc -> setting toke[rox][0] = '\0' -> problem !!
			// sol ->
			if (col){
				tokens[row][col] = '\0';
				row++;
			}
			col = 0;
			if (row >= MAXNUM_COMMAND-1) {
				printf ("too big command !! 2 \n");
				clean2Dstring (tokens, 0, MAXNUM_COMMAND);
				return NULL;
			}
			if (val == 1){
				tokens [row][col] = input[i];
				tokens[row][col+1] = '\0';
			}
			else {
				tokens[row][col] = input[i];
				tokens[row][col+1] = input[i+1];
				tokens[row][col+2] =  '\0';
				i++;
			}
			row++;
		}
	}

	if (getOperNum (input[size-1], '\0')) {
		tokens[row][col] = '\0';
		tokens[++row][0] = input[size-1];
		tokens[row][1] = '\0';
	}

	else{
		tokens[row][col] = input[size-1];

		tokens[row][col+1] = '\0';
	}
	//tokens[1+row][0] = '\0';
	tokens [1+row] = strdup ("");
	return tokens;
}
