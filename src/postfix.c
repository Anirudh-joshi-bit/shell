#include "../include/common.h"
#include "../include/defines.h"
#include "../include/stack.h"
#include "../include/helper.h"


char** postfix_conversion (char** toks){
	stack_t_ st;
	stack_init (&st);
	char** tok_iter = toks;

	stack_push(&st, strdup ("("));

// dont push the string into a string but to a 2d string
// so that no data is lost  => insert the string into toks

	char** toks_postfix = calloc (MAXNUM_COMMAND , sizeof (char*));
	// this is needed that malloc should init with NULL
	// sometimes os allocates fresh memory to malloc (kernel clears bytes for security reason)
	// but if malloced and freed and then again malloced, os will less likely set the freed
	// meory to NULL hence the memory will contain garbage value
	//  dereferancing these garbage value -> segfault (done in clean2Dstring func)
	//  soln -> either init with NULL manually or use calloc instead (always init with NULL)


	char** tok_iter_res = toks_postfix;


	// first check if the given input as a valid paranthesis or not

	while (*tok_iter) {
		if (isOper (*tok_iter)){

			if (!strcmp (*tok_iter, ")")){
				while (strcmp (stack_top (&st), "(")){
					*tok_iter_res = strdup (stack_top (&st));

					tok_iter_res++;
					stack_pop(&st);
				}
				if (st.size == 1) {
					perror ("ERROR .... bad paranthesis\n");
					clean2Dstring (toks_postfix, 0, MAXNUM_COMMAND);
					clean_stack (&st);
					return NULL;
				}

				stack_pop(&st);
			}
			// we dont want to remove anything if the current char is "(";
			// strdup is important in this case as we dont want the tok_iter to get deleted
			// when stack_pop() is called
			else if (!strcmp (*tok_iter, "(")) stack_push (&st, strdup (*tok_iter));
			else{

				while (getPrecedence(stack_top (&st)) >= getPrecedence (*tok_iter)){
					*tok_iter_res  = strdup (stack_top (&st));
					// this is important as stack_pop () is freeing the memory
					// hence store a copy instead of the actual pointer;

					tok_iter_res++;
				// pop, append;
					stack_pop(&st);
				}
				stack_push(&st, strdup (*tok_iter));
			}
		}
		else {
			*tok_iter_res = strdup (*tok_iter);
			tok_iter_res++;

		}

		tok_iter ++;
	}
	while (st.size > 1){			// the last element is ( that we dont want to add to the string;
		*tok_iter_res = strdup (stack_top (&st));
		tok_iter_res ++;
		stack_pop(&st);
	}
	stack_pop(&st);
	clean_stack (&st);
	return toks_postfix;
}
