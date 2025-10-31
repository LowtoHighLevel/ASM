#include <label.h>
#include <string.h>

#define NUM_LABELS 256
label_t labels[NUM_LABELS];
int label_size = 0;
/**
 * Adds a new label
 * 
 * name - Name of label
 * line - Line number in program
 * return - label size
 */
int add_label(char * name, int line) {
  int len = strlen(name);
  for (int i = 0; i < len-1; i++) {
    labels[label_size].name[i] = name[i];
  }
  labels[label_size].name[len-1] = 0;
  labels[label_size].line = line;
  label_size++;
}

/**
 * Resets the number of labels to 0.
 */
void reset_labels() {
  for (int i = 0; i < NUM_LABELS; i++) {
    labels[i].line = 0;
    labels[i].name[0] = 0;
  }
  label_size = 0;
}

/**
 * Gets the name of a label at index num.
 * 
 * num - index of label
 * return - label name
 */
char * label_name(int num) {
  if (num >= label_size) {
    return 0;
  }
  return labels[num].name;
}

/**
 * Gets the line a label is on.
 * 
 * num - label index
 * return - line number of label
 */
int * label_line(int num) {
  return &(labels[num].line);
}

/**
 * Gets a label
 * 
 * name - label name
 * return - label index
 */
int get_label(char * name) {
  for (int i = 0; i < label_size; i++) {
    if (strcmp(labels[i].name, name) == 0) {
      return i;
    }
  }
  return -1;
}