#ifndef __UTIL_H
#define __UTIL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void skip_line(FILE * ptr, char * buffer);

void skip_space(FILE * ptr, char * buffer);

int next_token(FILE * ptr, char * buffer);

int write(FILE *wptr, int val);

int read_reg(FILE * ptr, char * buffer, int line, unsigned char * reg);

#endif
