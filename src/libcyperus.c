/* libcyperus.c
This file is a part of 'libcyperus'
This program is free software: you can redistribute it and/or modify
hit under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

'libcyperus' is a network client library of cyperus.
https://github.com/petrichorsystems/cyperus

Copyright 2021 murray foster */

#include "libcyperus.h"

request_registry_t *global_registry;
lo_address lo_addr_send;

int _osc_message_handler(const char *path,
                         const char *types,
                         lo_arg **argv,
                         int argc, void *data,
                         void *user_data) {
  printf("libcyperus.c::_osc_handler()\n");
  response_t *response = NULL;
  int i;
  printf("path: <%s>\n", path);
  for (i = 0; i < argc; i++) {
    printf("arg %d '%c' ", i, types[i]);
    lo_arg_pp((lo_type)types[i], argv[i]);
    printf("\n");
  }
  unsigned long hash = request_hash_string((unsigned char*)(argv[0]));
  printf("about to iterate over entries\n");
  for(i=0; i<REQUEST_REGISTRY_MAX_ENTRIES; i++) {
    if(*(global_registry->entries[i]->hash) == hash) {
      global_registry->entries[i]->response = malloc(sizeof(response_t));
      global_registry->entries[i]->response->path = malloc(sizeof(char) * (strlen(path) + 1));
      global_registry->entries[i]->response->path = (char *)path;
      global_registry->entries[i]->response->argv = argv;
      global_registry->entries[i]->response->argc = argc;
      global_registry->entries[i]->response->data = data;
      global_registry->entries[i]->response->user_data = user_data;
      *(global_registry->entries[i]->hash) = 0;
      response = global_registry->entries[i]->response;      
      pthread_cond_signal(global_registry->entries[i]->cond);
      pthread_mutex_unlock(global_registry->entries[i]->mutex); 
      break;
    }
  }
  if(response == NULL) {
    printf("libcyperus.c::_request_registry_register(), incoming message did not match request\n");
    return 1;
  } 
  return 0;
} /* _handler_osc_message */

extern int libcyperus_list_mains(char ***ins,
                                  int *num_ins,
                                  char ***outs,
                                  int *num_outs) {
  printf("libcyperus.c::libcyperus_list_mains()\n");
  int number_ins = 0, number_outs = 0;
  request_t *request = request_register();
  printf("libcyperus.c::libcyperus_list_mains(), request->requst_id: %s\n", request->request_id);  
  lo_send(lo_addr_send, "/cyperus/list/main", "s", request->request_id);
  request_wait(request);
  parse_mains((char *)(global_registry->entries[request->id]->response->argv[2]), &ins, &number_ins, &outs, &number_outs);
  *num_ins = number_ins;
  *num_outs = number_outs;
  request_cleanup(request);
  return 0;
} /* libcyperus_list_mains */
  
void error(int num,
           const char *msg,
           const char *path)
{
  printf("liblo server error %d in path %s: %s\n", num, path, msg);
  fflush(stdout);
}

extern int libcyperus_add_bus(char *path,
                               char *name,
                               char *ins,
                               char *outs,
                               char **bus_id) {
  printf("libcyperus.c::libcyperus_add_bus()\n");
  int bus_id_length = 0;
  request_t *request = request_register();
  printf("libcyperus.c::libcyperus_add_bus(), request->requst_id: %s\n", request->request_id);
  lo_send(lo_addr_send, "/cyperus/add/bus", "sssss", request->request_id, path, name, ins, outs);
  request_wait(request);
  bus_id_length = (int)strlen((char *)global_registry->entries[request->id]->response->argv[6]);
  *bus_id = malloc(sizeof(char)*(bus_id_length + 1));
  *bus_id = (char *)global_registry->entries[request->id]->response->argv[6];
  request_cleanup(request);
  return 0;
} /* libcyperus_add_bus */

extern int libcyperus_list_bus(char *path,
                                char ***bus_ids,
                                char ***bus_names,
                                int **num_ins,
                                int **num_outs,
                                int *num_busses) {
  printf("libcyperus.c::libcyperus_list_bus()\n");
  request_t *request = request_register();
  printf("libcyperus.c::libcyperus_list_bus(), request->requst_id: %s\n", request->request_id);
  lo_send(lo_addr_send, "/cyperus/list/bus", "ssi", request->request_id, path, 1); /* 1 is for list type 'list all bus peers' */
  request_wait(request);
  parse_list_bus((char *)(global_registry->entries[request->id]->response->argv[5]),
                 &bus_ids,
                 &bus_names,
                 &num_ins,
                 &num_outs,
                 &num_busses);
  request_cleanup(request);
  return 0;
} /* libcyperus_list_bus */

extern int libcyperus_list_bus_port(char *path,
                                    char ***bus_port_id_ins,
                                    char ***bus_port_name_ins,
                                    int *num_ins,
                                    char ***bus_port_id_outs,
                                    char ***bus_port_name_outs,
                                    int *num_outs) {
  printf("libcyperus.c::libcyperus_list_bus_port()\n");
  request_t *request = request_register();
  printf("libcyperus.c::libcyperus_list_bus_port(), request->request_id: %s\n", request->request_id);
  lo_send(lo_addr_send, "/cyperus/list/bus_port", "ss", request->request_id, path);
  request_wait(request);
  parse_list_object_port((char *)(global_registry->entries[request->id]->response->argv[3]),
                      &bus_port_id_ins,
                      &bus_port_name_ins,
                      &num_ins,
                      &bus_port_id_outs,
                      &bus_port_name_outs,
                      &num_outs);
  request_cleanup(request);
  return 0;
} /* libcyperus_list_bus_ports */

extern int libcyperus_list_module_port(char *path,
                                       char ***module_port_id_ins,
                                       char ***module_port_name_ins,
                                       int *num_ins,
                                       char ***module_port_id_outs,
                                       char ***module_port_name_outs,
                                       int *num_outs) {
  printf("libcyperus.c::libcyperus_list_module_port()\n");
  request_t *request = request_register();
  printf("libcyperus.c::libcyperus_list_module_port(), request->request_id: %s\n", request->request_id);
    printf("libcyperus.c::libcyperus_list_module_port(), path: %s\n", path);
  lo_send(lo_addr_send, "/cyperus/list/module_port", "ss", request->request_id, path);
  request_wait(request);
  parse_list_object_port((char *)(global_registry->entries[request->id]->response->argv[3]),
                      &module_port_id_ins,
                      &module_port_name_ins,
                      &num_ins,
                      &module_port_id_outs,
                      &module_port_name_outs,
                      &num_outs);
  request_cleanup(request);
  return 0;
} /* libcyperus_list_module_ports */

extern int libcyperus_add_connection(char *path_out,
                                     char *path_in) {
  printf("libcyperus.c::libcyperus_add_connection()\n");
  request_t *request = request_register();
  printf("libcyperus.c::libcyperus_add_connection(), request->request_id: %s\n", request->request_id);
  lo_send(lo_addr_send, "/cyperus/add/connection", "sss", request->request_id, path_in, path_out);
  request_wait(request);
  /* we gotta do some error code parsing here */  
  request_cleanup(request);
  return 0;
} /* libcyperus_add_connection */


extern int libcyperus_setup(char *osc_port_in, char *osc_host_out, char *osc_port_out) {
  printf("libcyperus.c::libcyperus_setup()\n");
  lo_server_thread st = lo_server_thread_new(osc_port_in, error);  
  if(osc_port_in == NULL)
    osc_port_in = "5001";
  if(osc_host_out == NULL)
    osc_host_out = "127.0.0.1";
  if(osc_port_out == NULL)
    osc_port_out = "5000";
  
  request_registry_init();
  
  lo_addr_send = lo_address_new(osc_host_out, osc_port_out);
  lo_server_thread_add_method(st, NULL, NULL, _osc_message_handler, NULL);
  lo_server_thread_start(st);
  return 0;
} /* libcyperus_setup */
