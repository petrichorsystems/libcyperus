#include <stdio.h>

#include "../libcyperus.h"

int main(void) {
  libcyperus_setup("5000", "5001");
  printf("hello world\n");

  libcyperus_list_mains();
  printf("returning?\n");
}
