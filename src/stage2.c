#include <stage2.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <convert.h>
#include <handlers2.h>
#include <util.h>

#ifdef EXT_INT
  #ifndef EXT_FLAG
    #define EXT_FLAG
  #endif
#endif

int stage2(char* in, char* out) {

  // Open input file to read
  FILE * ptr = fopen(in, "r");
  if (ptr == NULL) {
    printf("no file found!\n");
    return 1;
  }


  // Open output file
  FILE * wptr = fopen(out, "wb");
  if (wptr == NULL) {
    printf("could not open out.bin!\n");
    return 1;
  }
  
  // Temporary read buffer and current line
  char buffer[256];
  int line = 0;

  // iterate through each line and handle an instruction
  while (next_token(ptr, buffer)) {
    int ret = 0;

    // Base features
    if (buffer[0] == '#') {
      skip_line(ptr, buffer); // ignore comments
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
    } else if (strcmp(buffer, "ext") == 0) {
      ret = handle_ext_base_outreg(ptr, wptr, 0, 0b00000, buffer, line);
    }

    // Extensions
    #ifdef EXT_INT
      else if (strcmp(buffer, "int") == 0) {
        ret = handle_interrupt(ptr, wptr, 1, buffer, line);
      }
      else if (strcmp(buffer, "rint") == 0) {
        ret = handle_interrupt(ptr, wptr, 0, buffer, line);
      }
    #endif
    #ifdef EXT_FLAG
      else if (strcmp(buffer, "ldflags") == 0) {
        ret = handle_ext_base_outreg(ptr, wptr, 0, 0b00001, buffer, line);
      } else if (strcmp(buffer, "sflag") == 0) {
        ret = handle_set_flags(ptr, wptr, 1, buffer, line);
      }
      else if (strcmp(buffer, "cflag") == 0) {
        ret = handle_set_flags(ptr, wptr, 0, buffer, line);
      }
    #endif

    // Handle invalid tokens
    else {
      printf("Error: unhandled token: \"%s\" on line: %d", buffer, line);
      ret = 1;
    }

    // Close on error
    if (ret) {
      fclose(wptr);
      fclose(ptr);
      return ret;
    }
    

    // Keep iterating
    line++;
    printf("\n");
  }

  // Close file and return
  fclose(wptr);
  fclose(ptr);
  return 0;
}
