
#include <util.h>
#include <convert.h>

void skip_line(FILE * ptr, char * buffer) {
  char ch = ' ';
  while (ch != '\n') {
    ch = fgetc(ptr);
    buffer[0] = ch;
  }
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
  while (ch != ' ' && ch != '\n' && ch != EOF) {
    buffer[i] = ch;
    i++;
    ch = fgetc(ptr);
  }
  buffer[i] = 0;

  return ch != EOF;
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
