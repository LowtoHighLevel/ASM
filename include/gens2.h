#ifndef __GENS2_H
#define __GENS2_H

unsigned int gen_load(unsigned char reg, unsigned int val);

unsigned int gen_reg(unsigned char control, unsigned char r1, unsigned char r2, unsigned char r3);

unsigned int gen_read_mem(unsigned char r1, unsigned char r3);

unsigned int gen_write_mem(unsigned char r1, unsigned char r2);

unsigned int gen_jmprel(unsigned char condition, short rel);

unsigned int gen_jmpreg(unsigned char condition, unsigned char reg);

unsigned int gen_ext(unsigned char ext_num, unsigned char cmd, unsigned char b1, unsigned char b2, unsigned char b3);


#endif

