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

#ifndef PARSE_H_
#define PARSE_H_
#include <stdio.h> //printf
#include <string.h> //memset
#include <stdlib.h> //exit(0);
#include <unistd.h>

extern void parse_mains(char *raw_mains,
                        char ****ins,
                        int *num_ins,
                        char ****outs,
                        int *num_outs);

extern void parse_list_bus(char *raw_bus_list,
                           char ****bus_ids,
                           char ****bus_names,
                           int ***num_ins,
                           int ***num_outs,
                           int **num_busses);

#endif
