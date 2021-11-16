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

#include "request.h"
#include "parse.h"

request_registry_t *global_registry;

extern int libcyperus_setup(char *osc_port_in,
                            char *osc_host_out,
                            char *osc_port_out);

extern int libcyperus_list_mains(char ***ins,
                                  int *num_ins,
                                  char ***outs,
                                  int *num_outs);

extern int libcyperus_add_bus(char *path,
                               char *name,
                               char *ins,
                               char *outs,
                               char **bus_id);

extern int libcyperus_list_bus(char *path,
                                char ***bus_ids,
                                char ***bus_names,
                                int **num_ins,
                                int **num_outs,
                                int *num_busses);

extern int libcyperus_list_bus_port(char *path,
                                     char ***bus_port_id_ins,
                                     char ***bus_port_name_ins,
                                     int *num_ins,
                                     char ***bus_port_id_outs,
                                     char ***bus_port_name_outs,
                                     int *num_outs);

extern int libcyperus_add_connection(char *path_out,
                                      char *path_in);

#endif
