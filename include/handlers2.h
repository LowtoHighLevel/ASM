#ifndef __HANDLER_S2_H
#define __HANDLER_S2_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int handle_ld(FILE * ptr, FILE * wptr, char * buffer, int line);

int handle_mov(FILE * ptr, FILE * wptr, char * buffer, int line);

int handle_alu(FILE * ptr, FILE * wptr, unsigned char control, const char * str, char * buffer, int line);

int handle_rotate(FILE * ptr, FILE * wptr, unsigned char control, const char * str, char * buffer, int line);

int handle_memory(FILE * ptr, FILE * wptr, char * buffer, int line, unsigned char dir);

int handle_jmp(FILE * ptr, FILE * wptr, char * buffer, int line, unsigned char cond);

int handle_ext_base_outreg(FILE * ptr, FILE * wptr, unsigned char ext, unsigned char cmd, char * buffer, int line);

int handle_set_flags(FILE * ptr, FILE * wptr, unsigned char val, char * buffer, int line);

int handle_interrupt(FILE * ptr, FILE * wptr, unsigned char val, char * buffer, int line);

#endif
