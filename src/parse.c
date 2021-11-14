/* parse.h
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

  *num_ins = number_ins;
  *num_outs = number_outs;

  free(raw_mains_copy_count);
  free(raw_mains_copy_parsing);  
} /* parse_mains */
