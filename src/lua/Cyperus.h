/* Cyperus.h
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

Copyright 2021 murray foster

original inspiration for this lua 'class object' model came from @tony19 on stack:
   https://stackoverflow.com/questions/11100435/how-do-i-create-a-class-object-in-lua-c-api-5-2 */


#ifndef LIBCYPERUS_LUA_CYPERUS_H_
#define LIBCYPERUS_LUA_CYPERUS_H_

#include <lua.h>
#include <lauxlib.h>
#include <stdlib.h>
#include <stdio.h>

#include "../libcyperus.h"

struct cyperus {
  int num_main_ins;
  int num_main_outs;
  char **main_ins;
  char **main_outs;  
};

typedef struct NumArray
{
  int size;
  double values [1];
} NumArray;

int cyperus_gc();
int cyperus_index();
int cyperus_newindex();
int cyperus_get_root();
int cyperus_new();

void register_cyperus_class(lua_State* L);

#endif