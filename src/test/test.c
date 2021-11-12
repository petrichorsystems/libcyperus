#include <stdio.h>

#include "../libcyperus.h"

int main(void) {
  libcyperus_setup("5000", "5001");
  printf("hello world\n");

  int c = 0;

   for(int i=0; i<5000; i++) {
      printf("i: %d\n", i);
      libcyperus_list_mains();
      c++;
      usleep(250);
   }
}
