/* parse.c
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

#include "parse.h"

extern void parse_mains(char *raw_mains, char ****ins, int *num_ins, char ****outs, int *num_outs) {
  char *raw_mains_copy_count, *raw_mains_copy_parsing, *token;
  int number_ins = 0, number_outs = 0, parsing_ins = 0, ins_idx = 0, outs_idx = 0;
  raw_mains_copy_count = strdup(raw_mains);
  raw_mains_copy_parsing = strdup(raw_mains);
  
  while ((token = strsep(&raw_mains_copy_count, "\n")) != NULL) {
    if(strcmp(token, "in:") == 0) {
      parsing_ins = 1;
      continue;
    }
    if(strcmp(token, "out:") == 0) {
      parsing_ins = 0;
      continue;
    }
    if(strcmp(token, "") == 0)
      continue;
    if(parsing_ins)
      number_ins++;
    if(!parsing_ins)
      number_outs++;
  }

  (**ins) = malloc(sizeof(char *)*number_ins);
  (**outs) = malloc(sizeof(char *)*number_outs);
  while ((token = strsep(&raw_mains_copy_parsing, "\n")) != NULL) {
    if(strcmp(token, "in:") == 0) {
      parsing_ins = 1;
      continue;
    }
    if(strcmp(token, "out:") == 0) {
      parsing_ins = 0;
      continue;
    }
    if(strcmp(token, "") == 0)
      continue;
    if(parsing_ins) {
      (**ins)[ins_idx] = malloc(sizeof(char)*(strlen(token)+1));
      (**ins)[ins_idx] = token;
      ins_idx++;
    }
    if(!parsing_ins) {
      (**outs)[outs_idx] = malloc(sizeof(char)*(strlen(token)+1));
      (**outs)[outs_idx] = token;      
      outs_idx++;
    }
  }

  free(raw_mains_copy_count);
  free(raw_mains_copy_parsing);  
} /* parse_mains */

extern void parse_list_bus(char *raw_bus_list, char ****bus_ids, char ****bus_names, int ***num_ins, int ***num_outs, int **num_busses) {
  printf("parse.c::parse_bus_list()\n");
  
  char *raw_bus_list_copy_count, *raw_bus_list_copy_parsing, *token, *token_copy, *token_metadata;
  int number_busses = 0, idx = 0, idx_metadata = 0;
  raw_bus_list_copy_count = strdup(raw_bus_list);
  raw_bus_list_copy_parsing = strdup(raw_bus_list);
  
  while ((token = strsep(&raw_bus_list_copy_count, "\n")) != NULL) {
    if(strcmp(token, "") == 0)
      continue;
    printf("bus: %s\n", token);
    number_busses++;
  }
  (**bus_ids) = malloc(sizeof(char *)*number_busses);
  (**bus_names) = malloc(sizeof(char *)*number_busses);
  (**num_ins) = malloc(sizeof(int)*number_busses);
  (**num_outs) = malloc(sizeof(int)*number_busses);
  
  while ((token = strsep(&raw_bus_list_copy_parsing, "\n")) != NULL) {
    if(strcmp(token, "") == 0) 
      continue;
    token_copy = strdup(token);
    idx_metadata = 0;
    while((token_metadata = strsep(&token_copy, "|")) != NULL) {
      if(strcmp(token_metadata, "") != 0) {
        switch(idx_metadata) {
        case 0:
          (**bus_ids)[idx] = malloc(sizeof(char)*(strlen(token_metadata)+1));
          (**bus_ids)[idx] = token_metadata;
          break;
        case 1:
          (**bus_names)[idx] = malloc(sizeof(char)*(strlen(token_metadata)+1));
          (**bus_names)[idx] = token_metadata;
          break;
        case 2:
          (**num_ins)[idx] = atoi(token_metadata);
          break;
        case 3:
          (**num_outs)[idx] = atoi(token_metadata);        
          break;        
        default:
          break;
        }
        idx_metadata++;
      }
    }
    idx++;
  }
  **num_busses = number_busses;
  free(raw_bus_list_copy_count);
  free(raw_bus_list_copy_parsing);
} /* parse_bus_list */
