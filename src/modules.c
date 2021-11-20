/* modules.c
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

#include "modules.h"

extern int libcyperus_add_module_audio_oscillator_pulse(char *bus_path,
                                                        float frequency,
                                                        float pulse_width,
                                                        float mul,
                                                        float add,
                                                        char **module_id) {
  printf("libcyperus.c::libcyperus_add_module_audio_oscillator_pulse()\n");

  request_t *request = request_register();
  
  printf("libcyperus.c::libcyperus_add_module_audio_oscillator_pulse(), request->requst_id: %s\n", request->request_id);
  
  lo_send(lo_addr_send, "/cyperus/add/module/audio/oscillator/pulse", "ssffff", request->request_id, bus_path, frequency, pulse_width, mul, add);

  request_wait(request);
  request_cleanup(request);

  return 0;
} /* libcyperus_add_bus */
