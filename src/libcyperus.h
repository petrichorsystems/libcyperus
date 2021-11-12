/* libcyperus.h
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

#ifndef LIBCYPERUS_H_
#define LIBCYPERUS_H_
#include <stdio.h> //printf
#include <string.h> //memset
#include <stdlib.h> //exit(0);
#include <lo/lo.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <uuid/uuid.h>

#define REQUEST_REGISTRY_MAX_ENTRIES 65536

typedef struct response_type {
  const char *path;
  const char *types;
  lo_arg **argv;
  int argc;
  void *data;
  void *user_data;
} response_t;

typedef struct request_type {
  unsigned int id;
  char *request_id;
  unsigned long *hash;
  pthread_mutex_t *mutex;
  pthread_cond_t *cond;

  response_t *response;
} request_t;

typedef struct request_registry_type {
  pthread_mutex_t mutex;  
  unsigned int count;
  request_t *entries;
} request_registry_t;

/* dan bernstein's xor-based hash */
/* http://www.cse.yorku.ca/~oz/hash.html */
unsigned long hash_string(unsigned char *str)
{
  unsigned long hash = 5381;
  int c = *str++;
  while (c) {
    hash = hash * 33 ^ c;
    c = *str++;
  }
  return hash;
}

char* build_uuid() {
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

extern int libcyperus_setup(char *osc_port_in, char *osc_port_out);
extern void libcyperus_list_mains();
#endif
