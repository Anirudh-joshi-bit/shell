
#pragma once

typedef struct __circularArr_t{

	char**      arr;
	 int         s;
	 int         e;
	 int         size;
	 int         maxsize;

}circularArr_t;


#ifndef CRCL_ARR
#define CRCL_ARR

extern circularArr_t* ca;

#endif

void ca_init (circularArr_t* ca, int ind);
void push_crclArr (circularArr_t* ca, char* str);
int print_history (circularArr_t* ca, long ind);
