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

request_t* _request_init(char *request_id) {
  request_t *request = malloc(sizeof(request_t));;

  request->request_id = (char *)malloc(sizeof(char) * (strlen(request_id)+1));
  request->mutex = malloc(sizeof(pthread_mutex_t));
  request->cond = malloc(sizeof(pthread_cond_t));
  
  snprintf(request->request_id, strlen(request_id)+1, "%s", request_id);
  printf("strlen(rquest_id): %d\n", (int)strlen(request_id));
  
  printf("libcyerus.c::_requestt_init(), building hash\n");

  request->hash = malloc(sizeof(unsigned long int));
  *request->hash = hash_string((unsigned char*)request->request_id);

  printf("libcyperus.c::_request_init(), %s\n", request_id);
  printf("libcyperus.c::_request_init(), %lu\n", *request->hash);
  
  if (pthread_mutex_init(request->mutex, NULL) != 0) {
    printf("libcyperus.c::_thread_osc_message_receive(), mutex init failed\n");
    exit(1);
  }
  if (pthread_cond_init(request->cond, NULL) != 0) {
    printf("libcyperus.c::_thread_osc_message_receive(), condition var init failed\n");
    exit(1);
  }

  printf("returning\n");
  
  return request;
} /* _request_init */

void _request_registry_init() {
  global_registry = malloc(sizeof(request_registry_t));
  global_registry->count = 0;
  global_registry->entries = malloc(sizeof(request_registry_t) * REQUEST_REGISTRY_MAX_ENTRIES);
} /* _request_registry_init */

void _request_registry_register(request_t *request) {
  printf("libcyperus.c::_request_registry_register()\n");  
  unsigned int id;
  pthread_mutex_lock(request->mutex);
  id = global_registry->count;
  if(global_registry->count == REQUEST_REGISTRY_MAX_ENTRIES)
    global_registry->count = 0;
  else
    global_registry->count++;
  printf("about to.. '%d' \n", id);
  global_registry->entries[id] = *request;
  pthread_mutex_unlock(request->mutex);
} /* _request_registry_get_id */

int _handler_osc_message(const char *path, const char *types, lo_arg **argv,
                         int argc, void *data, void *user_data) {
  printf("libcyperus.c::_osc_handler()\n");
  response_t *response = NULL;
  int i;
  printf("path: <%s>\n", path);
  for (i = 0; i < argc; i++) {
    printf("arg %d '%c' ", i, types[i]);
    lo_arg_pp((lo_type)types[i], argv[i]);
    printf("\n");
  }
  /* communicate osc message response and unblock corresponding calling thread */
  unsigned long hash = hash_string((unsigned char*)(argv[0]));

  printf("about to interate over entries\n");
  for(i=0; i<REQUEST_REGISTRY_MAX_ENTRIES; i++) {
    printf("i: %d\n", i);
    printf("argv[0]: %s\n", (char *)argv[0]);
    printf("global_registry->entries[i]->request_id: %s\n", global_registry->entries[i].request_id);
    printf("global_registry->entries[i]->hash: %lu\n", *global_registry->entries[i].hash);
    printf("hash: %lu\n", hash);
    if(*(global_registry->entries[i].hash) == hash) {
      printf("matched\n");
      global_registry->entries[i].response = malloc(sizeof(response_t));
      global_registry->entries[i].response->path = malloc(sizeof(char) * (strlen(path) + 4));
      printf("strlen(path): %d\n", (int)strlen(path));
      snprintf(global_registry->entries[i].response->path,
               strlen(path)+1,
               "%s",
               path);
      printf("OH YEAH: %s\n", global_registry->entries[i].response->path);
      global_registry->entries[i].response->argv = argv;
      global_registry->entries[i].response->argc = argc;
      global_registry->entries[i].response->data = data;
      global_registry->entries[i].response->user_data = user_data;
      response = global_registry->entries[i].response;
      pthread_cond_signal(global_registry->entries[i].cond);
      break;
    }
  }
  printf("done iterating\n");
  
  if(response == NULL) {
    printf("libcyperus.c::_request_registry_register(), incoming message did not match request\n");
    return 1;
  }

  printf("_handler_osc_message returning\n");
  
  return 0;
} /* _handler_osc_message */

request_t* _request_register() {
  printf("libcyperus.c::_request_register()\n");
  char *request_id = build_uuid();
  request_t *request = _request_init(request_id);
  _request_registry_register(request);
  return request;
} /* register_request */

void _request_wait(request_t *request) {
  printf("libcyperus.c::_request_wait()\n");  
  pthread_mutex_lock(request->mutex);
  pthread_cond_wait(request->cond, request->mutex);
  pthread_mutex_unlock(request->mutex);
} /* _request_wait */

void _request_cleanup(request_t *request) {
  printf("libcyperus.c::_request_cleanup()\n");  
  pthread_mutex_lock(request->mutex);  
  free(global_registry->entries[request->id].response);
  pthread_mutex_unlock(request->mutex);
} /* _request_cleanup */

void _parse_cyperus_mains(char *incoming_message) {
  /* char **main_ins; */
  /* char **main_outs; */
  printf("incoming_message: %s\n", incoming_message);
} /* _parse_cyperus_mains */

extern void libcyperus_list_mains() {
  request_t *request = _request_register();
  printf("libcyperus.c::libcyperus_list_mains(), request->requst_id: %s\n", request->request_id);
  lo_send(lo_addr_send, "/cyperus/list/main", "s", request->request_id);

  _request_wait(request);

  printf("hi, %d\n", request->id);
  
  printf("libcyperus.c::libcyperus_list_mains(), request->response->path: %s\n", global_registry->entries[request->id].response->path);
  
  _parse_cyperus_mains((char*)&(request->response->argv[0]));
  _request_cleanup(request);
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
  
  _request_registry_init();
  
  lo_addr_send = lo_address_new("127.0.0.1", osc_port_out);
  lo_server_thread_add_method(st, NULL, NULL, _handler_osc_message, NULL);
  lo_server_thread_start(st);
  return 0;
} /* libcyperus_setup */
