#include <stdio.h>
#include <string.h>
#include <stdlib.h>


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


union int_to_vals {
  unsigned int ival;
  int iival;
  unsigned short sval[2];
  short isval[2];
  unsigned char cval[4];
};


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
  printf("in gen_jmprel\n");
  unsigned int out = 0x80000000;
  printf("test");
  out |= (((unsigned int)condition) << 24);
  union int_to_vals conv;
  conv.ival = out;
  conv.isval[0] = rel;
  return conv.ival;
}

unsigned int gen_jmpreg(unsigned char condition, unsigned char reg) {
  unsigned int out = 0x90000000;
  out |= (((unsigned int)condition) << 24);
  out |= ((unsigned int)reg) << 16;
  return out;
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

int handle_ld(FILE * ptr, FILE * wptr, char * buffer, int line) {
  unsigned char reg;
  if (read_reg(ptr, buffer, line, &reg)) {
    fclose(wptr);
    fclose(ptr);
    return 1;
  }

  next_token(ptr, buffer);

  if (buffer[0] == '\'') {
    if (strlen(buffer) == 1) {
      write(wptr, gen_load(reg, ' '));
      next_token(ptr, buffer);
      printf("ld r%hi, ' '", reg);
      return 0;
    }
    else if (buffer[1] == '\\') {
      if (buffer[2] == 'n') write(wptr, gen_load(reg, '\n'));
      else if (buffer[2] == 't') write(wptr, gen_load(reg, '\t'));
      else if (buffer[2] == '\\') write(wptr, gen_load(reg, '\\'));
      else write(wptr, gen_load(reg, 0));
    } else {
      write(wptr, gen_load(reg, buffer[1]));
    }
    printf("ld r%hu, %s", reg, buffer);
 } else {
    unsigned int imm = atoi(buffer);
    write(wptr, gen_load(reg, imm));
    printf("ld r%hu %u", reg, imm);
  }	    
  return 0;
}


int handle_mov(FILE * ptr, FILE * wptr, char * buffer, int line) {

  unsigned char r1;
  if (read_reg(ptr, buffer, line, &r1)) {
    fclose(wptr);
    fclose(ptr);
    return 1;
  }

  unsigned char r2;
  if (read_reg(ptr, buffer, line, &r2)) {
    fclose(wptr);
    fclose(ptr);
    return 1;
  }

  write(wptr, gen_reg(0b00000, r2, 0, r1));
  printf("mov r%u r%u", r1, r2);
  return 0;

}

int handle_alu(FILE * ptr, FILE * wptr, unsigned char control, const char * str, char * buffer, int line) {
  unsigned char r1, r2, r3;

  if (read_reg(ptr, buffer, line, &r1)) {
    fclose(wptr);
    fclose(ptr);
    return 1;
  }

  if (read_reg(ptr, buffer, line, &r2)) {
    fclose(wptr);
    fclose(ptr);
    return 1;
  }

  if (read_reg(ptr, buffer, line, &r3)) {
    fclose(wptr);
    fclose(ptr);
    return 1;
  }

  write(wptr, gen_reg(control, r2, r3, r1));
  printf("%s r%u r%u r%u", str, r1, r2, r3);
  return 0;
}

int handle_rotate(FILE * ptr, FILE * wptr, unsigned char control, const char * str, char * buffer, int line) {
  unsigned char r1, r2;

  if (read_reg(ptr, buffer, line, &r1)) {
    fclose(wptr);
    fclose(ptr);
    return 1;
  }

  if (read_reg(ptr, buffer, line, &r2)) {
    fclose(wptr);
    fclose(ptr);
    return 1;
  }

  write(wptr, gen_reg(control, r2, 0, r1));
  printf("%s r%u r%u", str, r1, r2);
  return 0;
}

int handle_memory(FILE * ptr, FILE * wptr, char * buffer, int line, unsigned char dir) {

  unsigned char r1, r2;

  if (read_reg(ptr, buffer, line, &r1)) {
    fclose(wptr);
    fclose(ptr);
    return 1;
  }

  if (read_reg(ptr, buffer, line, &r2)) {
    fclose(wptr);
    fclose(ptr);
    return 1;
  }

  if (dir) {
    write(wptr, gen_write_mem(r1, r2));
    printf("write r%u r%u", r1, r2);
  } else {
    write(wptr, gen_read_mem(r1, r2));
    printf("read r%u r%u", r1, r2);
  }

  return 0;
}


int handle_jmp(FILE * ptr, FILE * wptr, char * buffer, int line, unsigned char cond) {
  

  next_token(ptr, buffer);

  printf("buffer: %s\n", buffer);
  
  union int_to_vals conv;
  
  if (buffer[0] == 'r') {
    unsigned int data = atoi(buffer+1);
    conv.ival = data;
    write(wptr, gen_jmpreg(cond, conv.cval[0]));
  } else {
    printf("not a reg\n");
    int data = atoi(buffer);
    conv.iival = data;
    printf("data: %d\n", data);
    printf("test\n");
    
    write(wptr, gen_jmprel(cond, conv.isval[0]));
  }
  printf("jmp(cond=%hu) %s", cond, buffer);
  return 0;
}

int main(int argc, char * argv[]) {

  printf("LTHL Assembler\n");


  FILE * ptr = fopen(argv[1], "r");

  
  if (ptr == NULL) {
    printf("no file found!\n");
    return 1;
  }

  FILE * wptr = fopen("out.bin", "wb");

  if (wptr == NULL) {
    printf("could not open out.bin!\n");
    return 1;
  }
  
  char buffer[256];

  int line = 0;
  while (next_token(ptr, buffer)) {
    int ret = 0;
    if (buffer[strlen(buffer)-1] == ':') {
      printf("label");
      ret = 0;
    }else if (buffer[0] == '#') {
      skip_line(ptr, buffer);
    } else if (strcmp(buffer, "ld") == 0) {
      ret = handle_ld(ptr, wptr, buffer, line);
    } else if (strcmp(buffer, "mov") == 0) {
      ret = handle_mov(ptr, wptr, buffer, line);
    } else if (strcmp(buffer, "add") == 0) {
      ret = handle_alu(ptr, wptr, 0b00000, "add", buffer, line);
    } else if (strcmp(buffer, "sub") == 0) {
      ret = handle_alu(ptr, wptr, 0b11000, "sub", buffer, line);
    } else if (strcmp(buffer, "and") == 0) {
      ret = handle_alu(ptr, wptr, 0b00001, "and", buffer, line);
    } else if (strcmp(buffer, "or") == 0) {
      ret = handle_alu(ptr, wptr, 0b00010, "or", buffer, line);
    } else if (strcmp(buffer, "xor") == 0) {
      ret = handle_alu(ptr, wptr, 0b00011, "xor", buffer, line);
    } else if (strcmp(buffer, "ror") == 0) {
      ret = handle_rotate(ptr, wptr, 0b00100, "ror", buffer, line);
    } else if (strcmp(buffer, "rol") == 0) {
      ret = handle_rotate(ptr, wptr, 0b00101, "rol", buffer, line);
    } else if (strcmp(buffer, "write") == 0) {
      ret = handle_memory(ptr, wptr, buffer, line, 1);
    } else if (strcmp(buffer, "read") == 0) {
      ret = handle_memory(ptr, wptr, buffer, line, 0);
    } else if (strcmp(buffer, "j") == 0) {
      ret = handle_jmp(ptr, wptr, buffer, line, 0);
    } else if (strcmp(buffer, "jz") == 0) {
      ret = handle_jmp(ptr, wptr, buffer, line, 1);
    } else if (strcmp(buffer, "jc") == 0) {
      ret = handle_jmp(ptr, wptr, buffer, line, 2);
    } else if (strcmp(buffer, "jv") == 0) {
      ret = handle_jmp(ptr, wptr, buffer, line, 3);
    } else if (strcmp(buffer, "jn") == 0) {
      ret = handle_jmp(ptr, wptr, buffer, line, 4);
    } else {
      printf("Error: unhandled token: \"%s\" on line: %d", buffer, line);
      ret = 1;
    }

    if (ret) {
      fclose(wptr);
      fclose(ptr);
      return ret;
    }
    
    line++;
    printf("\n");
  }

  fclose(wptr);
  fclose(ptr);

  return 0;
}
