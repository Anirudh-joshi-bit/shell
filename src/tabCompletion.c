#include "header.h"

int tab_completion (char* input, int size){

	struct termios oldt, newt;
	int st = 0;
	st = tcgetattr (STDIN_FILENO, &oldt);

	if (st){
		perror ("ERROR in implementing tab_completion -> tcgetattr failed\n");
		exit (-1);
	}

	newt = oldt;

	// close the canon mode that the terminal usually open in
	// also close echo to get full control of what is printed to
	newt.c_lflag &= ~(ICANON | ECHO);

	tcsetattr (STDIN_FILENO, TCSANOW, &newt);
	char c='\0';

	int it = 0;

	c = getchar ();
	while (c != '\n'){

		if (c == '\t'){
			printf (" tab ");
		}
		else {
			input[it++] = c;
			printf ("%c" , c);
		}
		c = getchar();

	}
	input [it++] = '\n';
	input[it+1] = '\0';

	tcsetattr (STDIN_FILENO, TCSANOW, &oldt);

	return 0;

}
