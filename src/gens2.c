#include <gens2.h>
#include <convert.h>

unsigned int gen_load(unsigned char reg, unsigned int val) {
  unsigned int out = 0b001 << 29;
  out |= (((unsigned int)reg) << 24);
  out |= (val & 0xFFFFFF);
  return out;
}
unsigned int gen_reg(unsigned char control, unsigned char r1, unsigned char r2, unsigned char r3) {
  unsigned int out = 0;
  out |= (((unsigned int)control) << 24);
  out |= ((unsigned int)r1) << 16;
  out |= ((unsigned int)r2) << 8;
  out |= ((unsigned int)r3);
  return out;
}

unsigned int gen_read_mem(unsigned char r1, unsigned char r3) {
  unsigned int out = 0b010 << 29;
  out |= ((unsigned int)r1) << 15;
  out |= ((unsigned int)r3);
  return out;
}

unsigned int gen_write_mem(unsigned char r1, unsigned char r2) {
  unsigned int out = 0b011 << 29;
  out |= ((unsigned int)r1) << 16;
  out |= ((unsigned int)r2) << 8;
  return out;
}

unsigned int gen_jmprel(unsigned char condition, short rel) {
  unsigned int out = 0b100 << 29;
  out |= (((unsigned int)condition) << 24);
  union int_to_vals conv;
  conv.ival = out;
  conv.isval[0] = rel;
  return conv.ival;
}

unsigned int gen_jmpreg(unsigned char condition, unsigned char reg) {
  unsigned int out = 0b101 << 29;
  out |= (((unsigned int)condition) << 24);
  out |= ((unsigned int)reg) << 16;
  return out;
}
unsigned int gen_ext(unsigned char ext_num, unsigned char cmd, unsigned char b1, unsigned char b2, unsigned char b3) {
  unsigned int out = (0b110 + ((unsigned int)ext_num)) << 29;
  out |= ((((unsigned int)cmd) & 0b11111) << 24);
  out |= (((unsigned int)b1) << 16);
  out |= (((unsigned int)b2) << 8);
  out |= (((unsigned int)b3) << 0);
  return out;
}

