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

lo_address lo_addr_send;

int _handler_osc_message(const char *path, const char *types, lo_arg **argv,
                         int argc, void *data, void *user_data) {
  printf("libcyperus.c::_osc_handler()\n");
  response_t *response = NULL;
  int i;
  /* printf("path: <%s>\n", path); */
  /* for (i = 0; i < argc; i++) { */
  /*   printf("arg %d '%c' ", i, types[i]); */
  /*   lo_arg_pp((lo_type)types[i], argv[i]); */
  /*   printf("\n"); */
  /* } */

  unsigned long hash = request_hash_string((unsigned char*)(argv[0]));
  printf("about to interate over entries\n");
  for(i=0; i<REQUEST_REGISTRY_MAX_ENTRIES; i++) {
    if(*(global_registry->entries[i]->hash) == hash) {
      global_registry->entries[i]->response = realloc(global_registry->entries[i]->response, sizeof(response_t));
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

extern void libcyperus_list_mains(char ***ins, int *num_ins, char ***outs, int *num_outs) {
  printf("libcyperus.c::libcyperus_list_mains()\n");

  int number_ins = 0, number_outs = 0;
  request_t *request = request_register();
  printf("libcyperus.c::libcyperus_list_mains(), request->requst_id: %s\n", request->request_id);  
  lo_send(lo_addr_send, "/cyperus/list/main", "s", request->request_id);
  request_wait(request);
  parse_mains((char *)(global_registry->entries[request->id]->response->argv[2]), &ins, &number_ins, &outs, &number_outs);
  request_cleanup(request);

  *num_ins = number_ins;
  *num_outs = number_outs;
} /* libcyperus_list_mains */

void error(int num, const char *msg, const char *path)
{
  printf("liblo server error %d in path %s: %s\n", num, path, msg);
  fflush(stdout);
}

extern int libcyperus_setup(char *osc_port_in, char *osc_port_out)
{
  printf("libcyperus.c::libcyperus_setup()\n");
  lo_server_thread st = lo_server_thread_new(osc_port_in, error);  
  if(osc_port_in == NULL)
    osc_port_in = "5001";
  if(osc_port_out == NULL)
    osc_port_out = "5000";
  
  request_registry_init();
  
  lo_addr_send = lo_address_new("127.0.0.1", osc_port_out);
  lo_server_thread_add_method(st, NULL, NULL, _handler_osc_message, NULL);
  lo_server_thread_start(st);
  return 0;
} /* libcyperus_setup */
