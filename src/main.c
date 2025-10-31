#include <stage2.h>
#include <stage1.h>
#include <stdio.h>
#include <string.h>

/**
 * Run method
 * 
 * check: state
 * in: input file name
 * out: output file name
 * return: state
*/
int run(int check, char* in, char* out) {
  int ret = 0;
  if (check == 0) {
    printf("Running default build\n");
    ret = stage1(in, "./.compiler_temp_1.S");
    ret = stage2("./.compiler_temp_1.S", out);
    remove("./.compiler_temp_1.S");
  } else if (check == 1) {
    printf("Running stage2 only\n");
    ret = stage2(in, out);
  } else if (check == 2) {
    printf("Running stage1 only\n");
    ret = stage1(in, out);
  }

  if (ret) {
    return ret;
  }
  
  printf("Done!\n");
  return 0;
}

/**
 * Main method
 */
int main(int argc, char * argv[]) {

  // Print output
  printf("LTHL Assembler\n");


  // print usage if needed
  if (argc == 1) {
    printf("Usage: %s [-s1, -s2, -all] <file> [out_file]\n", argv[0]);
    return 0;
  }

  // states
  int in_file = 1;
  int check = 0;
  char * out_file = "a.out";
  
  // arguments
  while(argv[in_file][0] == '-') {
    in_file++;
    if (strcmp(argv[1], "-s2") == 0) {
      check = 1;
    } else if (strcmp(argv[1], "-s1") == 0) {
      check = 2;
    }
  }

  // get output file
  if (in_file + 1 < argc) {
    out_file = argv[in_file+1];
  }

  // run the assembler
  return run(check, argv[in_file], out_file);
 }
