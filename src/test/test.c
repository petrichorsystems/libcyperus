#include <stdio.h>

#include "../libcyperus.h"

void test_libcyperus_list_mains() {
  printf("test.c::test_libcyperus_list_mains()\n");

  char **ins, **outs;
  int num_ins, num_outs;
  int i;
  libcyperus_list_mains(&ins, &num_ins, &outs, &num_outs);
  
  printf("num_ins: %d\n", num_ins);
  printf("num_outs: %d\n", num_outs);

  for(i=0; i<num_ins; i++) {
    printf("%s\n", ins[i]);
  }  
  for(int i=0; i<num_outs; i++) {
    printf("%s\n", outs[i]);
  }  
} /* test_libcyperus_list_mains */

int main(void) {
  printf("test.c::main()\n");
  libcyperus_setup("5000", "5001");
  printf("hello world\n");

  test_libcyperus_list_mains();
  

}
