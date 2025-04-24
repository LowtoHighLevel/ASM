#include <stage2.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <convert.h>
#include <handlers2.h>
#include <util.h>


int stage2(char* in, char* out) {

  FILE * ptr = fopen(in, "r");

  
  if (ptr == NULL) {
    printf("no file found!\n");
    return 1;
  }

  FILE * wptr = fopen(out, "wb");

  if (wptr == NULL) {
    printf("could not open out.bin!\n");
    return 1;
  }
  
  char buffer[256];

  int line = 0;
  while (next_token(ptr, buffer)) {
    int ret = 0;
    if (buffer[0] == '#') {
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
