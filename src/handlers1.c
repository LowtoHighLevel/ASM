#include <handlers1.h>
#include <label.h>
#include <string.h>
instruction_t instructions[2000];
int num_instructions;

void reset_instructions() {
    num_instructions = 0;
}

int add_instruction(FILE* ptr, char * buffer, int args) {
    strcpy(instructions[num_instructions].parts[0], buffer);
    for (int i = 0; i < args; i++) {
        next_token(ptr, buffer);
        int len = strlen(buffer);
        strcpy(instructions[num_instructions].parts[i+1], buffer);
        // handle spaces in strings
        if (buffer[0] == '\'' && strlen(buffer) == 1) {
            next_token(ptr, buffer);
            instructions[num_instructions].parts[i+1][len] = ' ';
            instructions[num_instructions].parts[i+1][len+1] = '\'';
        }
    }
    instructions[num_instructions].num_parts = args + 1;
    num_instructions++;
    return 1;
};

int add_instruction_raw(int args, char * arg1, char * arg2, char * arg3, char * arg4) {
    char * data[4] = {arg1, arg2, arg3, arg4};
    for (int i = 0; i < args; i++) {
        strcpy(instructions[num_instructions].parts[i], data[i]);
    }
    instructions[num_instructions].num_parts = args;
    num_instructions++;
    return 1;
}

instruction_t * get_instruction(int idx) {
    if (idx >= num_instructions) {
        return 0;
    }
    return &instructions[idx];
}

int instruction_size() {
    return num_instructions;
}


comment_t comments[1000];
int num_comments = 0;

void reset_comments() {
    for (int i = 0; i < num_comments; i++) {
        comments[i].value[0] = 0;
        comments[i].line = 0;
    }
    num_comments = 0;
}

void add_comment(int line, char * value) {
    strcpy(comments[num_comments].value, value);
    comments[num_comments].line = line;
    num_comments++;
}

comment_t * get_comments() {
    return comments;
}

int comment_size() {
    return num_comments;
}


int handle_instruction_exact(FILE * wptr, int i) {
    fprintf(wptr, "%s", instructions[i].parts[0]);
    for (int j = 1; j < instructions[i].num_parts; j++) {
        fprintf(wptr, " %s", instructions[i].parts[j]);
    }
    fprintf(wptr, "\n");
    return 0;
}

int handle_jmp1(FILE * wptr, int i) {
    fprintf(wptr, "%s", instructions[i].parts[0]);
    
    char * data = instructions[i].parts[1];
    if ((data[0] == 'r' && is_number(data[1])) || is_number(data[0])) {
        fprintf(wptr, " %s\n", data);
    }
    // jump to label
    else {
        int label = get_label(data);
        if (label == -1) return -1;
        int distance = *(label_line(label)) - i;
        if (distance > 0x7FFF || distance < -0x7FFF) {
            printf("distance to label is out of bounds! will not work for relative jump!");
            return -1;
        }
        fprintf(wptr, " %d\n", distance);
    }
    return 0;
}