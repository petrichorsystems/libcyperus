/* modules.h
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

#ifndef LIBCYPERUS_MODULES_H_
#define LIBCYPERUS_MODULES_H_

#include <stdio.h> //printf
#include <string.h> //memset
#include <stdlib.h> //exit(0);
#include <lo/lo.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

#include "libcyperus.h"

extern request_registry_t *global_registry;
extern lo_address lo_addr_send;

extern int libcyperus_add_module_audio_oscillator_pulse(char *bus_path,
                                                        float frequency,
                                                        float pulse_width,
                                                        float mul,
                                                        float add,
                                                        char **module_id);
extern int libcyperus_add_module_audio_filter_moogff(char *bus_path,
                                                     float frequency,
                                                     float gain,
                                                     float reset,
                                                     float mul,
                                                     float add,
                                                     char **module_id);
#endif
