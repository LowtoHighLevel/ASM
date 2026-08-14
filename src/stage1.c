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

int lda_function(FILE * ptr, char * buffer, int line) {
  char next[256];
  next_token(ptr, next);
  next_token(ptr, buffer);

  
  int label = get_label(buffer);
  if (label == -1) return -1;
  label = ((*(label_line(label)) - line)) - 4;
  printf("label: %d\n", label);
  if (label > 0x7FFF || label < -0x7FFF) {
    printf("distance to label is out of bounds! will not work for load address!");
    return -1;
  }
  snprintf(buffer, sizeof(buffer), "%d", label);
  int out = add_instruction_raw(3, "ld", next, buffer, 0);
  out += add_instruction_raw(4, "add", next, next, "r31");
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

int stage1_inner(char* in, int * line, int * line_label) {
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

    int check = (buffer[0] != '.' && buffer[1] != 'd' && (*line_label % 4) != 0);
    
    if (buffer[len-1] == ':') {
      add_label(buffer, *line_label);
    } else if (buffer[0] == '#') {
      read_line(ptr, buffer, 256, strlen(buffer));
      add_comment(*line, buffer);
    } else if (strcmp(buffer, ".include") == 0) {
      next_token(ptr, buffer);
      int val = stage1_inner(buffer, line, line_label);
      if (val < 0) return val;
    } else if (strcmp(buffer, "call") == 0) {
      ret = call_function(ptr, buffer);
    } else if (strcmp(buffer, "ret") == 0) {
      ret = return_function(ptr, buffer);
    } else if (strcmp(buffer, "lda") == 0) {
      ret = lda_function(ptr, buffer, *line_label);
    } else if (buffer[0] == '.' && buffer[1] == 'd') {
      int t = 0;
      ret = 0;
      int amt = add_define(ptr, buffer, buffer[2], &t);
      *line_label += t * amt;
      *line += amt;
      } else {
      ret = exact(ptr, buffer, *line_label);

      if (ret == -1) {
        printf("Error: unhandled token: \"%s\" on line: %d\n", buffer, *line);
      }
    }

    // Close on error
    if (ret < 0) {
      fclose(ptr);
      return ret;
    }
    // Keep iterating
    if (check) *line_label += 4 - (*line_label % 4);
    *line += ret;
    *line_label += ret * 4;
  }

  fclose(ptr);
  return 0;
}
int stage1(char* in, char* out) {
  reset_labels();
  reset_comments();

  int line = 0;
  int line_label = 0;
  int ret = stage1_inner(in, &line, &line_label);

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

  line = 0;

  // start getting ready to write the instructions...
  for (int i = 0; i < num_instructions; i++) {
    int offset = 0;

    while (comments[current_comment].line == i && current_comment < num_comments) {
      fprintf(wptr, "%s", comments[current_comment].value);
      current_comment++;
    }

    instruction_t * current_instruction = get_instruction(i);
    offset = current_instruction->offset;
    printf("offset: %d %d %d\n", current_instruction->offset, line, i*4);
    if (strcmp(current_instruction->parts[0], "j") == 0) {
      handle_jmp1(wptr, i, line);
    } else if (strcmp(current_instruction->parts[0], "jz") == 0) {
      handle_jmp1(wptr, i, line);
    } else if (strcmp(current_instruction->parts[0], "jc") == 0) {
      handle_jmp1(wptr, i, line);
    } else if (strcmp(current_instruction->parts[0], "jv") == 0) {
      handle_jmp1(wptr, i, line);
    } else if (strcmp(current_instruction->parts[0], "jn") == 0) {
      handle_jmp1(wptr, i, line);
    } else {
      handle_instruction_exact(wptr, i);
    }

    line += offset;
    
    if (i+1 < num_instructions) {
      instruction_t * next_instruction = get_instruction(i+1);
      if (next_instruction->offset == 4 && (line % 4) > 0) {
	line -= line % 4;
	line += 4;
      }
    }
    
    
  }
  fclose(wptr);
  return ret;
  
}
