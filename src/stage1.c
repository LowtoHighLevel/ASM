#include <stage2.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <convert.h>
#include <handlers1.h>
#include <util.h>
#include <label.h>

int exact_val(FILE * rptr, char * buffer, int args, const char * name) {
  if (strcmp(buffer, name) == 0 ) {
    return add_instruction(rptr, buffer, args);
  }
  return -1;
}

const char * exact_names[] = {"ld", "mov", "add", "sub", "and", "or", "xor", "ror", "rol", "write", "read", "ext", "int", "rint", "ldflags", "sflag", "cflag", "j", "jz", "jc", "jv", "jn"};
int exact_args[] = {2, 2, 3, 3, 3, 3, 3, 2, 2, 2, 2, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1};
 
int exact(FILE * rptr, char * buffer, int line) {
  int val = -1;
  int amt = 0;
  while (val == -1 && amt < 22) {
    val = exact_val(rptr, buffer, exact_args[amt], exact_names[amt]);
    amt++;
  }
  return val;
}

int move_stack(char * type, char * offset_reg) {
      return add_instruction_raw(4, type, "r30", "r30", offset_reg);
}

int call_function(FILE * ptr, char * buffer) {
      int out = add_instruction_raw(3, "write", "r30", "r28", 0);
      out += add_instruction_raw(3, "ld", "r28", "4", 0);
      out += move_stack("sub", "r28");
      out += add_instruction_raw(3, "write", "r30", "r31", 0);
      out += move_stack("sub", "r28");
      next_token(ptr, buffer);
      out += add_instruction_raw(2, "j", buffer, "", "");
      return out;
}

int return_function(FILE * ptr, char * buffer) {
    next_token(ptr, buffer);
    if (strcmp(buffer, "r28") == 0) {
      strcpy(buffer, "r27");
    }
    int out = move_stack("add", "r28");
    out += add_instruction_raw(3, "read", "r30", buffer, 0);
    for (int i = 0; i < 3; i++) {
      out += add_instruction_raw(4, "add", buffer, buffer, "r28");
    }
    out += move_stack("add", "r28");
    out += add_instruction_raw(3, "read", "r30", "r28", 0);
    out += add_instruction_raw(2, "j", buffer, 0, 0);
    return out;
}

int stage1_inner(char* in, int * line) {
  // Open input file to read
  FILE * ptr = fopen(in, "r");
  if (ptr == NULL) {
    printf("no file found!\n");
    return -1;
  }

  // Temporary read buffer and current line
  char buffer[256];

  // iterate through each line and parse instructions and labels
  while (next_token(ptr, buffer)) {
    int ret = 0;
    int len = strlen(buffer);
    if (buffer[len-1] == ':') {
      add_label(buffer, *line);
    } else if (buffer[0] == '#') {
      read_line(ptr, buffer, 256, strlen(buffer));
      add_comment(*line, buffer);
    } else if (strcmp(buffer, ".include") == 0) {
      next_token(ptr, buffer);
      int val = stage1_inner(buffer, line);
      if (val < 0) return val;
    } else if (strcmp(buffer, "call") == 0) {
      ret = call_function(ptr, buffer);
    } else if (strcmp(buffer, "ret") == 0) {
      ret = return_function(ptr, buffer);
      printf("ret: %d\n", ret);
    } else {
      ret = exact(ptr, buffer, *line);

      if (ret == -1) {
        printf("Error: unhandled token: \"%s\" on line: %d", buffer, *line);
      }
    }

    // Close on error
    if (ret < 0) {
      fclose(ptr);
      return ret;
    }
    // Keep iterating
    *line += ret;
  }

  fclose(ptr);
  return 0;
}
int stage1(char* in, char* out) {
  reset_labels();
  reset_comments();

  int line = 0;
  int ret = stage1_inner(in ,&line);

  if (ret < 0) {
    return ret;
  }

    
  int num_comments = comment_size();
  int current_comment = 0;
  int num_instructions = instruction_size();

  comment_t * comments = get_comments();

    // Open output file
  FILE * wptr = fopen(out, "wb");
  if (wptr == NULL) {
    printf("could not open output file!\n");
    return 1;
  }

  // start getting ready to write the instructions...
  for (int i = 0; i < num_instructions; i++) {
    while (comments[current_comment].line == i && current_comment < num_comments) {
      fprintf(wptr, "%s", comments[current_comment].value);
      current_comment++;
    }

    instruction_t * current_instruction = get_instruction(i);
    
    if (strcmp(current_instruction->parts[0], "j") == 0) {
      handle_jmp1(wptr, i);
    } else if (strcmp(current_instruction->parts[0], "jz") == 0) {
      handle_jmp1(wptr, i);
    } else if (strcmp(current_instruction->parts[0], "jc") == 0) {
      handle_jmp1(wptr, i);
    } else if (strcmp(current_instruction->parts[0], "jv") == 0) {
      handle_jmp1(wptr, i);
    } else if (strcmp(current_instruction->parts[0], "jn") == 0) {
      handle_jmp1(wptr, i);
    } else {
      handle_instruction_exact(wptr, i);
    }
    
  }
  fclose(wptr);
  return ret;
  
}
