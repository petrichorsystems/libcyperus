#include <stdio.h>

#include "../libcyperus.h"

void test_libcyperus_add_bus() {
  printf("test.c::test_libcyperus_add_bus()\n");

  char *bus_id;
  libcyperus_add_bus("/", "main0", "in", "out", &bus_id);

  printf("test.c::test_libcyperus_add_bus(), bus_id: %s\n", bus_id);
  
} /* test_libcyperus_list_mains */

void test_libcyperus_list_mains() {
  printf("test.c::test_libcyperus_list_mains()\n");

  char **ins, **outs;
  int num_ins, num_outs;
  int i;
  libcyperus_list_mains(&ins, &num_ins, &outs, &num_outs);
  
  printf("num_ins: %d\n", num_ins);
  printf("num_outs: %d\n", num_outs);
} /* test_libcyperus_list_mains */

int main(void) {
  printf("test.c::main()\n");
  libcyperus_setup("5000", "5001");
  printf("hello world\n");

  test_libcyperus_list_mains();
  test_libcyperus_add_bus();
}
