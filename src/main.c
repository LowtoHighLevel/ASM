#include <stage2.h>
#include <stdio.h>
#include <string.h>

int run(int check, char* in, char* out) {
  int ret = 0;
  if (check == 0) {
    printf("Running default build\n");
    ret = stage2(in, out);
  } else if (check == 1) {
    printf("Running stage2 only\n");
    ret = stage2(in, out);
  }

  if (ret) {
    return ret;
  }
  
  printf("Done!\n");
  return 0;
}
int main(int argc, char * argv[]) {

  printf("LTHL Assembler\n");

  if (argc == 1) {
    printf("Usage: %s [-s2, -all] <file> [out_file]\n", argv[0]);
    return 0;
  }

  int in_file = 1;
  int check = 0;
  char * out_file = "out.bin";
  while(argv[in_file][0] == '-') {
    in_file++;
    if (strcmp(argv[1], "-s2") == 0) {
      check = 1;
    }
  }

  if (in_file + 1 < argc) {
    out_file = argv[in_file+1];
  }

  return run(check, argv[in_file], out_file);
 }
