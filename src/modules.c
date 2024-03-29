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
  *module_id = (char *)(global_registry->entries[request->id]->response->argv[2]);
  request_cleanup(request);  
  return 0;
} /* libcyperus_add_module_audio_oscillator_pulse */

extern int libcyperus_add_module_audio_filter_moogff(char *bus_path,
                                                     float frequency,
                                                     float gain,
                                                     float reset,
                                                     float mul,
                                                     float add,
                                                     char **module_id) {
  printf("libcyperus.c::libcyperus_add_module_audio_filter_moogff()\n");

  request_t *request = request_register();
  
  printf("libcyperus.c::libcyperus_add_module_audio_filter_moogff(), request->request_id: %s\n", request->request_id);
  lo_send(lo_addr_send, "/cyperus/add/module/audio/filter/moogff", "ssfffff", request->request_id, bus_path, frequency, gain, reset, mul, add);
  request_wait(request);
  *module_id = (char *)(global_registry->entries[request->id]->response->argv[2]);
  request_cleanup(request);
  return 0;
} /* libcyperus_add_module_audio_filter_moogff */

extern int libcyperus_add_module_motion_envelope_stdshape(char *bus_path,
                                                          int stdshape,
                                                          float attack_time,
                                                          float release_time,
                                                          float level,
                                                          float curve,
                                                          char **module_id) {
  printf("libcyperus.c::libcyperus_add_module_motion_envelope_stdshape()\n");
  request_t *request = request_register();
  printf("libcyperus.c::libcyperus_add_module_motion_envelope_stdshape(), request->request_id: %s\n", request->request_id);
  lo_send(lo_addr_send, "/cyperus/add/module/motion/envelope/stdshape", "ssiffff", request->request_id, bus_path, stdshape, attack_time, release_time, level, curve);
  request_wait(request);
  *module_id = (char *)(global_registry->entries[request->id]->response->argv[2]);
  request_cleanup(request);
  return 0;
} /* libcyperus_add_module_motion_envelope_stdshape */

extern int libcyperus_edit_module_motion_envelope_segment(char *module_path,
                                                           int release_node,
                                                           int loop_node,
                                                           int offset,
                                                           float gate,
                                                           float level_scale,
                                                           float level_bias,
                                                           float time_scale) {
  printf("libcyperus.c::libcyperus_edit_module_motion_envelope_segment()\n");
  request_t *request = request_register();  
  printf("libcyperus.c::libcyperus_edit_module_motion_envelope_segment(), request->request_id: %s\n", request->request_id);
  lo_send(lo_addr_send, "/cyperus/edit/module/motion/envelope/segment",
          "ssiiiffff",
          request->request_id,
          module_path,
          release_node,
          loop_node,
          offset,
          gate,
          level_scale,
          level_bias,
          time_scale);
  request_wait(request);
  request_cleanup(request);
  return 0;
} /* libcyperus_edit_module_motion_envelope_segment */
