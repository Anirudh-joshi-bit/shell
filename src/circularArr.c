#include "../include/common.h"
#include "../include/circularArr.h"

void ca_init (circularArr_t* ca, int size){

		ca->arr = calloc (size, sizeof (char*));
		ca->s = -1;
		ca->e = -1;
		ca->size = 0;
		ca->maxsize = size;

}
int pop_crclArr (circularArr_t* ca){

	if (ca->size == 0){
		perror ("circularArr_t pop");
		return -1;

	}
	free (ca->arr [ca->e]);
	// wraping around using %
	ca -> e = (ca->e+1) % ca -> maxsize;
	ca->size --;

	return 0;
}

void push_crclArr (circularArr_t* ca, char* str){

	if (!str) return;

	if (ca->size == 0){
		// reset the ends
		ca->s = 0;
		ca->e = 0;
		ca->size = 1;
		ca->arr[ca->s] = strdup (str) ;
		return;

	}
	else if (ca -> size == ca -> maxsize){
		// pop and push the string
		if (pop_crclArr(ca)){
			perror ("msize == 0 !");
			exit (1);
		}
		ca->s = (ca->s+1) % ca->maxsize;
		ca->arr[ca -> s] = strdup (str);
		ca->size ++;
	}
	else {

		ca->s = (ca->s +1) % ca->maxsize;
		ca->size++;
		ca->arr[ca->s] = strdup (str);
	}
}

char* access_crclArr (circularArr_t* ca, int ind){

	if (ind >= ca->size) return NULL;

	if (ca->s > ca->e){

		// not wrapped around;
		return ca->arr [ca->s - ind];
	}

	// wrapped around
	if (ca->s >= ind)
		return ca->arr[ ca->s - ind ];
	else {
		int back_ind = ind - ca->s;
		return ca->arr [ ca->maxsize - back_ind ];
	}
}

int print_history (circularArr_t* ca, long ind){

	if (ind == -1) {

		int it = ca->e;
		int index = ca->size;
		while (it != ca->s) {
			printf ("%d - %s\n", index, ca->arr[it]);
			it = (it +1) % ca->maxsize;
			index--;
		}
		printf ("%d - %s\n", index, ca->arr[it]);
	}
	else{
		char* string = access_crclArr(ca, ind);
		if (string)
			printf ("%s\n", string);
		else
			return -1;
	}
	return 0;

}
