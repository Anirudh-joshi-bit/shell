#pragma once


typedef struct __node_t {

	char*				command;
	struct __node_t*	next;
	struct __node_t*	prev;
}node_t;
typedef struct __list_t {

	node_t*		head;
	node_t*		tail;
	int			size;
}list_t;
typedef struct __stack_t {

	list_t*		l;
	int			size;
}stack_t_;


void stack_init (stack_t_*);
int stack_pop(stack_t_*);
char* stack_top(stack_t_*);
void stack_push(stack_t_*, char*);
void list_init (list_t* l);
int insert_head (list_t* l, char* command);
int delete_head (list_t* l);
