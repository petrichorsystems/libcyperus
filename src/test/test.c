#include <stdio.h>

#include "../libcyperus.h"

void test_libcyperus_add_connection() {
  printf("test.c::test_libcyperus_add_connection()\n");
  
} /* test_libcyperus_add_connection */

void test_libcyperus_list_bus_port() {
  printf("test.c::test_libcyperus_list_bus_port()\n");

  char **bus_ids, **bus_names, **bus_port_id_ins, **bus_port_name_ins,
    **bus_port_id_outs, **bus_port_name_outs, *path;
  int *num_ins, *num_outs, num_port_ins, num_port_outs, num_busses;

  int idx = 0;
  
  libcyperus_list_bus("/", &bus_ids, &bus_names, &num_ins, &num_outs, &num_busses);

  /* grab bus id, make path */

  path = malloc(sizeof(char) * (strlen(bus_ids[0]) + 2));
  snprintf(path, strlen(bus_ids[0])+2, "/%s", bus_ids[0]);

  libcyperus_list_bus_port(path,
                           &bus_port_id_ins,
                           &bus_port_name_ins,
                           &num_port_ins,
                           &bus_port_id_outs,
                           &bus_port_name_outs,
                           &num_port_outs);

  /* for(idx=0; idx<num_port_ins; idx++) { */
  /*   printf("num_port_in: %d\n", idx); */
  /*   printf("bus_port_id_in: %s\n", bus_port_id_ins[idx]); */
  /*   printf("bus_port_name_in: %s\n", bus_port_name_ins[idx]); */
  /* } */
  /* for(idx=0; idx<num_port_outs; idx++) { */
  /*   printf("num_port_out: %d\n", idx); */
  /*   printf("bus_port_id_out: %s\n", bus_port_id_outs[idx]); */
  /*   printf("bus_port_name_out: %s\n", bus_port_name_outs[idx]); */
  /* } */
} /* test_libcyperus_list_bus_port */

void test_libcyperus_list_bus() {
  printf("test.c::test_libcyperus_list_bus()\n");

  char **bus_ids, **bus_names;
  int *num_ins, *num_outs, num_busses;

  libcyperus_list_bus("/", &bus_ids, &bus_names, &num_ins, &num_outs, &num_busses);

  /* printf("num_busses: %d\n", num_busses); */
  /* for(idx=0; idx<num_busses; idx++) { */
  /*   printf("idx: %d\n", idx); */
  /*   printf("bus_ids: %s\n", bus_ids[idx]); */
  /*   printf("bus_names: %s\n", bus_names[idx]); */
  /*   printf("num_ins: %d\n", num_ins[idx]); */
  /*   printf("num_outs: %d\n", num_outs[idx]); */
  /* } */
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
  libcyperus_setup("5000", "127.0.0.1", "5001");
  printf("hello world\n");

  test_libcyperus_list_mains();
  test_libcyperus_add_bus();
  test_libcyperus_list_bus();
  test_libcyperus_list_bus_port();
  test_libcyperus_add_connection();
}
