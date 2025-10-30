#include <handlers2.h>
#include <gens2.h>
#include <util.h>
#include <convert.h>
#include <string.h>

int handle_ld(FILE * ptr, FILE * wptr, char * buffer, int line) {
  // Load register
  unsigned char reg;
  if (read_reg(ptr, buffer, line, &reg)) {
    fclose(wptr);
    fclose(ptr);
    return 1;
  }

  // Parse value
  next_token(ptr, buffer);

  // Handle characters
  if (buffer[0] == '\'') {
    // Handle spaces
    if (strlen(buffer) == 1) {
      write(wptr, gen_load(reg, ' '));
      next_token(ptr, buffer);
      printf("ld r%hi, ' '", reg);
      return 0;
    }
    // Handle escape characters
    else if (buffer[1] == '\\') {
      if (buffer[2] == 'n') write(wptr, gen_load(reg, '\n'));
      else if (buffer[2] == 't') write(wptr, gen_load(reg, '\t'));
      else if (buffer[2] == '\\') write(wptr, gen_load(reg, '\\'));
      else write(wptr, gen_load(reg, 0));
    }
    // Handle regular characters
    else {
      write(wptr, gen_load(reg, buffer[1]));
    }
    printf("ld r%hu, %s", reg, buffer);
 }
 // otherwise load an integer
 else {
    unsigned int imm = atoi(buffer);
    write(wptr, gen_load(reg, imm));
    printf("ld r%hu %u", reg, imm);
  }	    
  return 0;
}


int handle_mov(FILE * ptr, FILE * wptr, char * buffer, int line) {

  unsigned char r1, r2;

  // Load r1
  if (read_reg(ptr, buffer, line, &r1)) {
    fclose(wptr);
    fclose(ptr);
    return 1;
  }

  // Load r2
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


  // Read ALU registers
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


  // Generate and write output
  write(wptr, gen_reg(control, r2, r3, r1));
  printf("%s r%u r%u r%u", str, r1, r2, r3);
  return 0;
}

int handle_rotate(FILE * ptr, FILE * wptr, unsigned char control, const char * str, char * buffer, int line) {
  unsigned char r1, r2;

  // Read rotate registers
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

  // Get registers
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

  // If we are writing
  if (dir) {
    write(wptr, gen_write_mem(r1, r2));
    printf("write r%u r%u", r1, r2);
  }
  // Reading
  else {
    write(wptr, gen_read_mem(r1, r2));
    printf("read r%u r%u", r1, r2);
  }

  return 0;
}


int handle_jmp(FILE * ptr, FILE * wptr, char * buffer, int line, unsigned char cond) {
  
  // Gather the next token for a jump instruction
  next_token(ptr, buffer);
  printf("buffer: %s\n", buffer);
  
  union int_to_vals conv;
  
  char type[16];
  // It's a register
  if (buffer[0] == 'r') {
    strcpy(type, "absolute");
    unsigned int data = atoi(buffer+1);
    conv.ival = data;
    write(wptr, gen_jmpreg(cond, conv.cval[0]));
  }
  // Not a register, relative jump.
  else {
    strcpy(type, "relative");
    int data = atoi(buffer);
    conv.iival = data;
    printf("data: %d\n", data);
    printf("test\n");
    
    write(wptr, gen_jmprel(cond, conv.isval[0]));
  }
  printf("jmp(cond=%hu, type=%s) %s", cond, type, buffer);
  return 0;
}


// Anything past this is for extensions
int handle_ext_base_outreg(FILE * ptr, FILE * wptr, unsigned char ext, unsigned char cmd, char * buffer, int line) {
  
  // Read a register
  unsigned char r1;
  if (read_reg(ptr, buffer, line, &r1)) {
    fclose(wptr);
    fclose(ptr);
    return 1;
  }

  // Output command
  write(wptr, gen_ext(ext, cmd, 0, 0, r1));
  return 0;
}

int handle_set_flags(FILE * ptr, FILE * wptr, unsigned char val, char * buffer, int line) {
      
  // Read flag number and whether we are setting or resetting
  unsigned char flag;
  if (read_int8(ptr, buffer, line, &flag)) {
    fclose(wptr);
    fclose(ptr);
    return 1;
  }
  write(wptr, gen_ext(0, 0b00010, flag & 0b11111, 0, val & 0b1));
  return 0;
}

int handle_interrupt(FILE * ptr, FILE * wptr, unsigned char val, char * buffer, int line) {
  // just yeet it.
  write(wptr, gen_ext(0, 0b00011, 0, 0, val & 0b1));
  return 0;
}