#include <stdio.h>

#include "../libcyperus.h"

int main(void) {
  printf("test.c::main()\n");

  char **ins, **outs;
  int num_ins, num_outs;
  int i;
  
  libcyperus_setup("5000", "5001");
  printf("hello world\n");

  libcyperus_list_mains(&ins, &num_ins, &outs, &num_outs);
  
  printf("num_ins: %d\n", num_ins);
  printf("num_outs: %d\n", num_outs);

  printf("test.c::main(), main inputs:\n");
  for(i=0; i<num_ins; i++) {
    printf("%s\n", ins[i]);
  }
  
  printf("test.c::main(), main outputs:\n");
  for(int i=0; i<num_outs; i++) {
    printf("%s\n", outs[i]);
  }

}
