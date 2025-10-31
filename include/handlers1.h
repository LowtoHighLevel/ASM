#ifndef __HANDLER_S1_H
#define __HANDLER_S1_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <util.h>

typedef struct instruction {
    char parts[8][256];
    int num_parts;
} instruction_t;

typedef struct comment {
    char value[256];
    int line;
} comment_t;


void reset_instructions();

int add_instruction(FILE* ptr, char * buffer, int args);

int add_instruction_raw(int args, char * arg1, char * arg2, char * arg3, char * arg4);

instruction_t * get_instruction(int idx);

int instruction_size();

void reset_comments();

void add_comment(int line, char * value);

comment_t * get_comments();

int comment_size();

int handle_instruction_exact(FILE * wptr, int i);

int handle_jmp1(FILE * wptr, int i);
#endif
