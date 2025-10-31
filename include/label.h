#ifndef LABEL_H
#define LABEL_H

typedef struct label {
  char name[256];
  int line;
} label_t;

/**
 * Adds a new label
 * 
 * name - Name of label
 * line - Line number in program
 * return - label size
 */
int add_label(char * name, int line);

/**
 * Resets the number of labels to 0.
 */
void reset_labels();

/**
 * Gets the name of a label at index num.
 * 
 * num - index of label
 * return - label name
 */
char * label_name(int num);

/**
 * Gets the line a label is on.
 * 
 * num - label index
 * return - line number of label
 */
int * label_line(int num);

/**
 * Gets a label
 * 
 * name - label name
 * return - label index
 */
int get_label(char * name);


#endif