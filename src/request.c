/* request.c
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

#include "request.h"

char* _build_uuid() {
  char *id;
  int id_len = 37;
  uuid_t uuid;
  char uuid_str[37];

  uuid_generate(uuid);
  uuid_unparse_lower(uuid, uuid_str);

  id = (char *)malloc(sizeof(char) * id_len);
  snprintf(id, id_len, "%s", uuid_str);

  return id;
}

/* dan bernstein's xor-based hash */
/* http://www.cse.yorku.ca/~oz/hash.html */
extern unsigned long request_hash_string(unsigned char *str)
{
  unsigned long hash = 5381;
  int c = *str++;
  while (c) {
    hash = hash * 33 ^ c;
    c = *str++;
  }
  return hash;
}

request_t* _request_init(char *request_id) {
  printf("request.c::_request_init()\n");
  request_t *request = malloc(sizeof(request_t));;

  request->request_id = (char *)malloc(sizeof(char) * (strlen(request_id)+1));
  request->mutex = malloc(sizeof(pthread_mutex_t));
  request->cond = malloc(sizeof(pthread_cond_t));
  
  snprintf(request->request_id, strlen(request_id)+1, "%s", request_id);
  printf("strlen(rquest_id): %d\n", (int)strlen(request_id));
  
  printf("libcyerus.c::_request_init(), building hash\n");

  request->hash = malloc(sizeof(unsigned long int));
  *request->hash = request_hash_string((unsigned char*)request->request_id);

  printf("request.c::_request_init(), %s\n", request_id);
  printf("request.c::_request_init(), %lu\n", *request->hash);
  
  if (pthread_mutex_init(request->mutex, NULL) != 0) {
    printf("request.c::_thread_osc_message_receive(), mutex init failed\n");
    exit(1);
  }
  if (pthread_cond_init(request->cond, NULL) != 0) {
    printf("request.c::_thread_osc_message_receive(), condition var init failed\n");
    exit(1);
  }

  printf("returning\n");
  
  return request;
} /* _request_init */

void _request_registry_register(request_t *request) {
  printf("request.c::_request_registry_register()\n");  
  unsigned int id;
  pthread_mutex_lock(request->mutex);
  id = global_registry->count;
  if(global_registry->count >= REQUEST_REGISTRY_MAX_ENTRIES - 1)
    global_registry->count = 0;
  else
    global_registry->count++;
  printf("about to.. '%d' \n", id);

  global_registry->entries[id]->id = id;
  request->id = id;
  memcpy(&(global_registry->entries[id][0]), request, sizeof(request_t));
  
  pthread_mutex_unlock(request->mutex);
} /* _request_registry_get_id */

extern void request_registry_init() {
  printf("request.c::request_registry_init()\n");
  global_registry = malloc(sizeof(request_registry_t));
  global_registry->count = 0;
  global_registry->entries = malloc(sizeof(request_t*) * REQUEST_REGISTRY_MAX_ENTRIES);
  for(int i=0; i < REQUEST_REGISTRY_MAX_ENTRIES; i++) {
    global_registry->entries[i] = malloc(sizeof(request_t));
    global_registry->entries[i]->response = (response_t *)malloc(sizeof(response_t));
    global_registry->entries[i]->response->path = malloc(1);;
  }
} /* _request_registry_init */

extern request_t* request_register() {
  printf("request.c::request_register()\n");
  char *request_id = _build_uuid();
  request_t *request = _request_init(request_id);
  _request_registry_register(request);
  return request;
} /* register_request */

extern void request_wait(request_t *request) {
  printf("request.c::request_wait()\n");  
  pthread_mutex_lock(request->mutex);
  pthread_cond_wait(request->cond, request->mutex);
  pthread_mutex_unlock(request->mutex);
} /* _request_wait */

extern void request_cleanup(request_t *request) {
  printf("request.c::request_cleanup()\n");  
  pthread_mutex_lock(request->mutex);
  free(request);
  pthread_mutex_unlock(request->mutex);
} /* _request_cleanup */

