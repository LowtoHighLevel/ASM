
#include <util.h>
#include <convert.h>

void skip_line(FILE * ptr, char * buffer) {
  char ch = ' ';
  while (ch != '\n') {
    ch = fgetc(ptr);
    buffer[0] = ch;
  }
}

void read_line(FILE * ptr, char * buffer, int max, int idx) {
  char ch = ' ';
  int i = idx;
  while (ch != '\n' && i < max) {
    ch = fgetc(ptr);
    buffer[i] = ch;
    i++;
  }
  buffer[i] = 0;
}

void skip_space(FILE * ptr, char * buffer) {
  char ch = ' ';
  while (ch == ' ' || ch == '\n' || ch == '\t') {
    ch = fgetc(ptr);
    buffer[0] = ch;
  }
}

int next_token(FILE * ptr, char * buffer) {
  skip_space(ptr, buffer);
  int i = 0;
  char ch = buffer[0];
  while (ch != ' ' && ch != '\n' && ch != '\t' && ch != EOF) {
    buffer[i] = ch;
    i++;
    ch = fgetc(ptr);
  }
  buffer[i] = 0;

  return ch != EOF;
}

int is_number(char c) {
  return c >= '0' && c <= '9';
}


int write(FILE *wptr, int val) {

  union int_to_vals conv;
  conv.ival = val;
  
  return fwrite(conv.cval, sizeof(conv.cval), 1, wptr);
}

int read_reg(FILE * ptr, char * buffer, int line, unsigned char * reg) {

  next_token(ptr, buffer);
  
  if (buffer[0] != 'r') {
    printf("Error: not a register! on line: %d\n", line);
    return 1;
  }
  
  union int_to_vals conv;
  conv.ival = atoi(buffer+1);
  *reg = conv.cval[0];
  return 0;
}

int read_int8(FILE * ptr, char * buffer, int line, unsigned char * val) {

  next_token(ptr, buffer);
  
  union int_to_vals conv;
  conv.ival = atoi(buffer);
  *val = conv.cval[0];
  return 0;
}
