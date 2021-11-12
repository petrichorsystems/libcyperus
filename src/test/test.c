#include <stdio.h>

#include "../libcyperus.h"

int main(void) {
  libcyperus_setup("5000", "5001");
  printf("hello world\n");

  for(int i=0; i<5; i++)
   for(int i=0; i<16000; i++) {
      printf("i: %d\n", i);
      libcyperus_list_mains();
      usleep(100);
   }
}
