#include "header.h"


void node_init (node_t* node, char* command){
	node-> command = command;
	node-> next = NULL;
}

void list_init (list_t* l){
	// the l->head has to be init (malloced)  to dereference node !!
	l->head = malloc (sizeof (node_t));
	node_init (l->head, "-1");
	l->size = 0;
}

int insert_head (list_t* l, char* command){

	node_t* node = malloc (sizeof (node_t));
	assert (node);
	node_init (node, command);
	node -> next = l->head;
	l->head = node;
	l->size++;

	return 0;
}
int delete_head (list_t* l){

	if (l->size == 0){
		printf ("cannot delete from an empty list !! \n");
		return 1;
	}
	node_t* node = l->head;
	l->head = l->head->next;
	free (node);
	l->size--;

	return 0;
}

void stack_init (stack_t_* st){

	st->l = malloc (sizeof (list_t));
	list_init(st->l);
	st->size = 0;
}
void stack_push(stack_t_* st, char* command){
	insert_head (st->l, command);
	st->size++;
}

int stack_pop (stack_t_* st){
	if (delete_head (st->l)) return 1;
	st->size--;;
	return 0;
}
char* stack_top (stack_t_* st){
	if (st->size == 0) return NULL;
	return st->l->head->command;
}
