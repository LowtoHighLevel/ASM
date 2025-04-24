#include <handlers2.h>
#include <gens2.h>
#include <util.h>
#include <convert.h>

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
