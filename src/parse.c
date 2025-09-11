#include "../include/common.h"
#include "../include/defines.h"
#include "../include/utils.h"
#include "../include/helper.h"

char* mini_shell (char* main_input, int size, circularArr_t* ca, int* status, int flag){

	// start a recursive solutione
	char* output = process_input (main_input, size, ca, status, flag);
	if (*status)
		return NULL;
	return output;

}



// add bound checks



char** parse (char* input, int size, int* tok_size){
	// preblem (was) ->  malloc (MAXNUM_COMMAND) -> wrong

	char** ans = calloc (MAXNUM_COMMAND, sizeof (char*));
	char** ans_it = ans;
	int ans_size = 0;

	for (int i=0; i<size; i++){

		char* temp = calloc (MAXLEN_COMMAND, sizeof (char));
		char* temp_it = temp;
		bool first_char_found = false;


		while (i < size && temp_it - temp < MAXLEN_COMMAND  && !getOperNum(input[i])){
			if (!first_char_found && (input[i] == ' ' || input[i] == '\n')){
				i++;
				continue;
			}
			first_char_found = true;
			*(temp_it++)= input[i++];
		}
		if (temp_it - temp >= MAXLEN_COMMAND){
			perror ("too big command");
			clean2Dstring (ans, 0, MAXNUM_COMMAND);
			free (temp);
			return NULL;

		}
		if (!temp[0]){
			free (temp);
			continue;;
		}

		*temp_it = '\0';

		if (*ans_it)
			strcat (*ans_it, temp);
		else
			*ans_it = strdup(temp);

		ans_size++;
		if (ans_size >= MAXNUM_COMMAND){
			clean2Dstring (ans, 0, MAXNUM_COMMAND);
			perror ("too big command");
			return NULL;
		}
		ans_it ++;
		free (temp);
		if (i>= size) {
			*tok_size = ans_size;
			return ans;

		}
		if (i<size-1 && input[i] == '<' && input[i+1] == '<'){

			if (*ans_it)
				strcat (*ans_it, "<<");
			else
				*ans_it = strdup ("<<");

			ans_size++;
		if (ans_size >= MAXNUM_COMMAND){
			clean2Dstring (ans, 0, MAXNUM_COMMAND);
			perror ("too big command");
			return NULL;
		}
			ans_it ++;

		}
		else if (i<size-1 && input[i] == '>' && input[i+1] == '>'){
			if (*ans_it)
			strcat (*ans_it, ">>");
			else
				*ans_it = strdup (">>");

			ans_size++;
		if (ans_size >= MAXNUM_COMMAND){
			clean2Dstring (ans, 0, MAXNUM_COMMAND);
			perror ("too big command");
			return NULL;
		}
			ans_it ++;

		}
		else if (input[i] == '$'){
			if (i+1 >= size || input[i+1] != '('){
				clean2Dstring (ans, 0, MAXNUM_COMMAND);
				printf ("1");
				return NULL;
			}
			int open_braces = 1;
			i+=2;
			char* temp_ = calloc(MAXLEN_COMMAND, sizeof (char));
			char* temp_it = temp_;
			bool first_char_found = false;


			while (temp_it - temp_ < MAXLEN_COMMAND && open_braces && i < size) {
				if (!first_char_found && (input[i] == ' ' || input[i] == '\n')){
						i++;
						continue;
				}
				first_char_found = true;

				if (input[i] == '(')  open_braces ++;
				else if (input[i] == ')') open_braces --;

				if (open_braces){
					*(temp_it++) = input[i]	;
				}
				i++;
			}
			// i-- -> to not skip the right next char to (    as curently i points to the next char to )
			// but after i++ in the for loop, the char pointed by i will be skipped
			i--;
			if (temp_it - temp_ >= MAXLEN_COMMAND){

				perror ("too big command inside $()");
				clean2Dstring(ans, 0, MAXNUM_COMMAND);
				free (temp_);
				return NULL;

			}
			if (!temp_[0]) {

				free (temp_);
				perror ("empty command in mini_shell");
				return NULL;
			}

			*temp_it = '\0';
			if (open_braces){
				clean2Dstring (ans, 0, ans_size);
				return NULL;

			}
			if (ans_it == ans) {
				return NULL;
			}
			int status = 0;
			char* mini_out = mini_shell (temp_, strlen(temp_), ca, &status ,MINI_SHELL);
			if (status){
				free (mini_out);
				return NULL;
			}
			if (mini_out) {

				// problem -> even if temp is always MAXLEN_COMMAND long but i am using strdup to add temp to
				// ans (strdup copied only the string) => strlen (*ans_it) = strlen (temp)
				// => strcat can write beyound allocated space ( > strlen (temp))
				// which may ovewrite the metadata => heap corruption



				char* realloc_temp =  realloc (*(ans_it-1), MAXLEN_COMMAND);
				if (realloc_temp)
					*(ans_it-1) = realloc_temp;

				else{
					clean2Dstring (ans, 0, ans_size);
					return NULL;
				}

				if (strlen (*(ans_it-1)) + strlen(mini_out) >= MAXLEN_COMMAND){
					perror ("too big output of mini_shell");
					free (mini_out);

					return NULL;
				}

				strcat (*(--ans_it), mini_out);
	//			ans_size --;
				free (mini_out);
			}
		}
		else {
			char oper [2];
			oper[0] = input[i];
			oper[1] = '\0';

			if (*ans_it)
				strcat (*ans_it, oper);
			else
				*ans_it = strdup (oper);

			ans_size++;
		if (ans_size >= MAXNUM_COMMAND){
			clean2Dstring (ans, 0, MAXNUM_COMMAND);
			perror ("too big command");
			return NULL;
		}
			ans_it ++;
		}

	}
	*tok_size = ans_size;
	return ans;
}
