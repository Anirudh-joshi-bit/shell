#include "../include/common.h"
#include "../include/defines.h"
#include "../include/stack.h"


void node_init (node_t* node, char* command){
	node-> command = command;
	node-> next = NULL;
	node-> prev = NULL;
}

void node_clean (node_t* node) {

	free (node->command);

}


void list_init (list_t* l){
	// the l->head has to be init (malloced)  to dereference node !!
	l->head = NULL;
	l->tail = l->head;
	l->size = 0;
}

int insert_head (list_t* l, char* command){

	if (l->head == NULL){
		l->head = calloc (1,sizeof (node_t));
		node_init (l->head, command);
		l->size++;
		l->tail = l->head;
		return 0;
	}

	node_t* node = calloc (1, sizeof (node_t));
	assert (node);
	node_init (node, command);

	node -> next = l->head;
	l->head->prev = node;

	l->head = node;
	l->size++;

	return 0;
}

int insert_tail (list_t* l, char* command){

	if (l->tail == NULL){
		l->tail = calloc (1,sizeof (node_t));
		node_init (l->tail, command);
		l->head = l->tail;
		l->size = 0;
		return 0;
	}

	node_t* node = calloc (1,sizeof (node_t));
	node_init (node, command);
	l->tail->next = node;
	node->prev = l->tail;

	l->tail = node;
	l->size ++;

	return 0;

}

int delete_tail (list_t* l){

	if (l->size == 0){
		free (l);
		return 1;
	}

	node_t* node = l->tail;
	l->tail = l->tail->prev;

	l->size --;

	free (node -> command);
	free (node);

	return 0;
}

int delete_head (list_t* l){

	if (l->size == 0){
		free (l);
		return 1;
	}


	node_t* node = l->head;
	l->head = l->head->next;
	free (node->command);
	free (node);
	l->size--;

	return 0;
}

void list_clean (list_t* l) {

	while (l->size) {
		delete_head(l);
	}
	free (l);

}

void stack_init (stack_t_* st){

	st->l = calloc (1,sizeof (list_t));
	list_init(st->l);
	st->size = 0;
}
void stack_push(stack_t_* st, char* command){
	insert_head (st->l, command);
	st->size++;
}

int stack_pop (stack_t_* st){
	if (delete_head (st->l)) return 1;
	st->size--;
	return 0;
}
char* stack_top (stack_t_* st){
	if (st->size == 0) return NULL;
	return st->l->head->command;
}
void clean_stack(stack_t_* st){

	list_clean (st->l);

}
