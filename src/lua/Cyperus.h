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

static const char REGISTRY_CYPERUS_STATE_KEY[] = "CREGISTRY_LIBCYPERUS_LUA_CYPERUS"; 
static const char REGISTRY_CYPERUS_BUS_STATE_PREFIX_KEY[] = "CREGISTRY_LIBCYPERUS_LUA_CYPERUS_BUS";

typedef struct libcyperus_lua_cyperus_type {
  char *osc_port_in;
  char *osc_host_out;
  char *osc_port_out;
  int num_ins;
  int num_outs;
  char **ins;
  char **outs;
  int root_bus_exists;
  char *root_bus_path;
} libcyperus_lua_cyperus_t;

typedef struct libcyperus_lua_bus_type {
  char *bus_path;
  char *module_id;
  int num_ins;
  int num_outs;
  char **ids_in;
  char **names_in;
  char **ids_out;
  char **names_out;
} libcyperus_lua_bus_t;

typedef struct {
  char *id;
  char *name;
  char *full_path;
  char *type;
} cyperus_bus_info_t;

typedef struct {
  char *id;
  char *full_path;
  char *type;
} cyperus_module_info_t;

int cyperus_gc();
int cyperus_index();
int cyperus_newindex();
int cyperus_get_root();
int cyperus_new();
int cyperus_update(lua_State *L,
                   char *osc_port_in,
                   char *osc_host_out,
                   char *osc_port_out,
                   char **ins,
                   int num_ins,
                   char **outs,
                   int num_outs,
                   int root_bus_exists,
                   char *root_bus_path);
void register_cyperus_class(lua_State* L);

int cyperus_port_gc(lua_State* L);
int cyperus_port_newindex(lua_State* L);
int cyperus_port_index(lua_State* L);
int cyperus_port_add_connection(lua_State *L);
int _build_ports(lua_State* L);
void register_cyperus_port_class(lua_State *L);

int cyperus_bus_gc();
int cyperus_bus_index();
int cyperus_bus_id();
int build_cyperus_bus_registry_key(char *bus_id, char **registry_key);
int cyperus_bus_add_module();
int _add_cyperus_bus(lua_State *L,
                    char *path,
                    char *name,
                    char *ins,
                    char *outs,
                    char **bus_id);

int cyperus_bus_newindex();
int cyperus_bus_getid();

void register_cyperus_bus_class(lua_State* L);
int cyperus_bus_port_gc(lua_State* L);
int cyperus_bus_port_newindex(lua_State* L);
int cyperus_bus_port_index(lua_State* L);
int cyperus_bus_port_add_connection(lua_State *L);
int _build_bus_ports(lua_State* L, char *bus_path);
void register_cyperus_bus_port_class(lua_State* L);

int cyperus_module_gc();
int cyperus_module_index();
int _cyperus_make_connection(lua_State *L);
int cyperus_module_connect(lua_State *L);
int cyperus_module_newindex();
void register_cyperus_module_class(lua_State* L);

int cyperus_module_port_gc(lua_State* L);
int cyperus_module_port_newindex(lua_State* L);
int cyperus_module_port_index(lua_State* L);
int cyperus_module_port_add_connection(lua_State *L);
int _build_module_ports(lua_State* L, char *module_path);
void register_cyperus_module_port_class(lua_State *L);


#endif
