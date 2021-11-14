#include <stdio.h>

#include "../libcyperus.h"

void test_libcyperus_list_bus() {
  printf("test.c::test_libcyperus_list_bus()\n");

  char *bus_id, **bus_ids, **bus_names;
  int *num_ins, *num_outs, num_busses, idx;
  
  libcyperus_add_bus("/", "main0", "in", "out", &bus_id);
  printf("test.c::test_libcyperus_add_bus(), bus_id: %s\n", bus_id);

  libcyperus_list_bus("/", &bus_ids, &bus_names, &num_ins, &num_outs, &num_busses);

  printf("num_busses: %d\n", num_busses);
  for(idx=0; idx<num_busses; idx++) {
    printf("idx: %d\n", idx);
    printf("bus_ids: %s\n", bus_ids[idx]);
    printf("bus_names: %s\n", bus_names[idx]);
    printf("num_ins: %d\n", num_ins[idx]);
    printf("num_outs: %d\n", num_outs[idx]);
  }
} /* test_libcyperus_list_bus */

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
  test_libcyperus_list_bus();
}
