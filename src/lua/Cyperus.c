/* Cyperus.c
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

#include "Cyperus.h"

int luaopen_Cyperus(lua_State *L) {
  register_cyperus_class(L);
  register_cyperus_bus_class(L);
  register_cyperus_bus_port_class(L);
  return 1;
}

/*
 * cyperus object ****************************************
 */
static const luaL_Reg _meta[] = {
    {"__gc", cyperus_gc},
    {"__index", cyperus_index},
    {"__newindex", cyperus_newindex},
    { NULL, NULL }
};
static const luaL_Reg _methods[] = {
    {"new", cyperus_new},
    {"get_root", cyperus_get_root},
    {"get_ins", cyperus_get_ins},
    {"get_outs", cyperus_get_outs},
    { NULL, NULL }
};

int cyperus_gc(lua_State* L) {
  printf("## __gc\n");
  return 0;
}
int cyperus_newindex(lua_State* L) {
  printf("## __newindex\n");
  return 0;
}
int cyperus_index(lua_State* L) {
  printf("## __index\n");
  return 0;
}

int cyperus_get_root(lua_State* L) {
  printf("Cyperus.c::cyperus_get_root()\n");
  libcyperus_lua_cyperus_t *state;
  libcyperus_lua_bus_t *bus_state;
  char *bus_id, *full_path,
    **bus_port_id_ins,
    **bus_port_name_ins,
    **bus_port_id_outs,
    **bus_port_name_outs;
  int error_code, root_path_size = 0, idx, num_ins, num_outs;
  
  lua_pushlightuserdata(L, (void *)&REGISTRY_CYPERUS_STATE_KEY);
  lua_gettable(L, LUA_REGISTRYINDEX);
 
  state = (libcyperus_lua_cyperus_t *)lua_touserdata(L, -1);

  if(state->root_bus_exists) {
    printf("i'm a meaningful error code about a root bus already existing!! boo!\n");
    return -1;
  }
  
  error_code = _add_cyperus_bus(L, "/", "main0", "in", "out", &bus_id);

  printf("Cyperus.c::cyperus_get_root(), new bus_id: %s\n", bus_id);

  root_path_size = sizeof(char) + (1 + strlen(bus_id) + 1);
  state->root_bus_path = malloc(root_path_size);
  full_path = malloc(root_path_size);
  snprintf(state->root_bus_path, root_path_size, "/%s", bus_id);
  snprintf(full_path, root_path_size, "/%s", bus_id);
    
  state->root_bus_exists = 1;


  lua_createtable(L, 1, 0);
    
  _build_bus_ports(L, full_path);
  
  lua_pushstring(L, "bus_id");
  lua_pushstring(L, bus_id);
  lua_rawset(L, -3);

  lua_pushstring(L, "name");
  lua_pushstring(L, "main0");
  lua_rawset(L, -3);

  lua_pushstring(L, "full_path");
  lua_pushstring(L, full_path);
  lua_rawset(L, -3);

  luaL_getmetatable(L, "Cyperus_Bus");
  lua_setmetatable(L, -2);
  
  free(full_path);
  
  return 1;
}

int cyperus_get_ins(lua_State *L) {
  printf("Cyperus.c::cyperus_get_ins()\n");
  int idx;

  lua_pushlightuserdata(L, (void *)&REGISTRY_CYPERUS_STATE_KEY);
  lua_gettable(L, LUA_REGISTRYINDEX);
 
  libcyperus_lua_cyperus_t *state;
  state = (libcyperus_lua_cyperus_t *)lua_touserdata(L, -1);

  lua_createtable(L, state->num_ins, 0);
  for(idx=0; idx<state->num_ins; idx++) {
    lua_pushstring(L, state->ins[idx]);
    lua_rawseti(L, -2, idx+1);
  }
  return 1;
}

int cyperus_get_outs(lua_State *L) {
  printf("Cyperus.c::cyperus_get_outs()\n");  
  int idx;

  lua_pushlightuserdata(L, (void *)&REGISTRY_CYPERUS_STATE_KEY);
  lua_gettable(L, LUA_REGISTRYINDEX);
 
  libcyperus_lua_cyperus_t *state;
  state = (libcyperus_lua_cyperus_t *)lua_touserdata(L, -1);

  lua_createtable(L, state->num_outs, 0);
  for(idx=0; idx<state->num_outs; idx++) {
    lua_pushstring(L, state->outs[idx]);
    lua_rawseti(L, -2, idx+1);
  }
  return 1;
}

int cyperus_update(lua_State *L,
                   char *osc_port_in,
                   char *osc_host_out,
                   char *osc_port_out,
                   char **ins,
                   int num_ins,
                   char **outs,
                   int num_outs,
                   int root_bus_exists,
                   char *root_bus_path) {
  lua_pushlightuserdata(L, (void *)&REGISTRY_CYPERUS_STATE_KEY);
  lua_gettable(L, LUA_REGISTRYINDEX);
 
  libcyperus_lua_cyperus_t *state;
  state = (libcyperus_lua_cyperus_t *)lua_touserdata(L, -1);

  state->osc_port_in = osc_port_in;
  state->osc_host_out = osc_host_out;
  state->osc_port_out = osc_port_out;
  state->ins = ins;
  state->num_ins = num_ins;
  state->outs = outs;
  state->num_outs = num_outs;
  state->root_bus_exists = 0;

  return 0;
}

int cyperus_new(lua_State* L) {
  printf("## new\n");

  libcyperus_lua_cyperus_t *state = malloc(sizeof(libcyperus_lua_cyperus_t));
  int idx;
  char *osc_port_in, *osc_host_out, *osc_port_out, **ins, **outs;
  int num_ins, num_outs;
  
  osc_port_in = (char *)luaL_checkstring(L, 1);
  osc_host_out = (char *)luaL_checkstring (L, 2);
  osc_port_out = (char *)luaL_checkstring (L, 3);

  printf("osc_port_in: %s\n", osc_port_in);
  printf("osc_host_out: %s\n", osc_host_out);
  printf("osc_port_out: %s\n", osc_port_out);
  
  libcyperus_setup(osc_port_in, osc_host_out, osc_port_out);
  libcyperus_list_mains(&ins, &num_ins, &outs, &num_outs);  

  state->osc_port_in = osc_port_in;
  state->osc_host_out = osc_host_out;
  state->osc_port_out = osc_port_out;
  state->ins = ins;
  state->num_ins = num_ins;
  state->outs = outs;
  state->num_outs = num_outs;
  state->root_bus_exists = 0;
  
  lua_pushlightuserdata(L, (void *)&REGISTRY_CYPERUS_STATE_KEY);
  lua_pushlightuserdata(L, (void *)state);
  lua_settable(L, LUA_REGISTRYINDEX);

  luaL_getmetatable(L, "Cyperus");
  lua_setmetatable(L, -2);
  
  return 1;
}

void register_cyperus_class(lua_State* L) {
  int lib_id, meta_id;

  /* newclass = {} */
  lua_createtable(L, 0, 0);
  lib_id = lua_gettop(L);

  /* metatable = {} */
  luaL_newmetatable(L, "Cyperus");
  meta_id = lua_gettop(L);
  luaL_setfuncs(L, _meta, 0);

  /* metatable.__index = _methods */
  luaL_newlib(L, _methods);
  lua_setfield(L, meta_id, "__index");  

  /* metatable.__metatable = _meta */
  luaL_newlib(L, _meta);
  lua_setfield(L, meta_id, "__metatable");

  /* class.__metatable = metatable */
  lua_setmetatable(L, lib_id);

  /* _G["Cyperus"] = newclass */
  lua_setglobal(L, "Cyperus");
}

/*
 * cyperus bus object ************************************
 */
static const luaL_Reg _bus_meta[] = {
  {"__gc", cyperus_bus_gc},
  {"__index", cyperus_bus_index},
  {"__newindex", cyperus_bus_newindex},
  { NULL, NULL }
};
static const luaL_Reg _bus_methods[] = {
  {"get_ins", cyperus_bus_get_ins},
  {"get_outs", cyperus_bus_get_outs},
  { NULL, NULL }
};

int cyperus_bus_gc(lua_State* L) {
  printf("## __gc\n");
  return 0;
}
int cyperus_bus_newindex(lua_State* L) {
  printf("## __newindex\n");
  return 0;
}
int cyperus_bus_index(lua_State* L) {
  printf("## __index\n");
  return 0;
}

int build_cyperus_bus_registry_key(char *bus_id, char **registry_key) {
  int registry_key_size = sizeof(char) * (strlen(REGISTRY_CYPERUS_BUS_STATE_PREFIX_KEY) \
                                          + 1 + strlen(bus_id) + 1);
  *registry_key = malloc(registry_key_size);
  snprintf(*registry_key, registry_key_size, "%s-%s", REGISTRY_CYPERUS_BUS_STATE_PREFIX_KEY, bus_id);

  return 0;
}

int _add_cyperus_bus(lua_State *L,
                    char *path,
                    char *name,
                    char *ins,
                    char *outs,
                    char **bus_id) {
  printf("Cyperus.c::_add_cyperus_bus()\n");

  libcyperus_lua_bus_t *state = malloc(sizeof(libcyperus_lua_bus_t));
  int error_code, registry_key_size, idx;
  char *registry_key;

  return libcyperus_add_bus(path, name, ins, outs, bus_id);
}

int cyperus_bus_get_ins(lua_State *L) {
  printf("Cyperus.c::cyperus_bus_get_ins()\n");
  char *bus_id, *full_path;
  
  lua_pushstring(L, "bus_id");
  lua_rawget(L, -3);
  bus_id = (char *)lua_tostring(L, -1);

  lua_pushstring(L, "full_path");
  lua_rawget(L, -4);
  full_path = (char *)lua_tostring(L, -1);

  printf("full_path !!! %s\n", full_path);
  
  return 0;
}

int cyperus_bus_get_outs(lua_State *L) {
  printf("Cyperus.c::cyperus_bus_get_outs()\n");  
  int idx;

  lua_pushlightuserdata(L, (void *)&REGISTRY_CYPERUS_STATE_KEY);
  lua_gettable(L, LUA_REGISTRYINDEX);
 
  libcyperus_lua_cyperus_t *state;
  state = (libcyperus_lua_cyperus_t *)lua_touserdata(L, -1);

  lua_createtable(L, state->num_outs, 0);
  for(idx=0; idx<state->num_outs; idx++) {
    lua_pushstring(L, state->outs[idx]);
    lua_rawseti(L, -2, idx+1);
  }
  return 1;
}

int cyperus_bus_new(lua_State* L) {
  printf("## new\n");

  
  return 1;
}

void register_cyperus_bus_class(lua_State* L) {
  int lib_id, meta_id;

  /* newclass = {} */
  lua_createtable(L, 0, 0);
  lib_id = lua_gettop(L);

  /* metatable = {} */
  luaL_newmetatable(L, "Cyperus_Bus");
  meta_id = lua_gettop(L);
  luaL_setfuncs(L, _bus_meta, 0);

  /* metatable.__index = _bus_methods */
  luaL_newlib(L, _bus_methods);
  lua_setfield(L, meta_id, "__index");  
  
  /* metatable.__bus_metatable = _bus_meta */
  luaL_newlib(L, _bus_meta);
  lua_setfield(L, meta_id, "__bus_metatable");

  /* class.__bus_metatable = metatable */
  lua_setmetatable(L, lib_id);    
  
  /* _G["Cyperus_Bus"] = newclass */
  lua_setglobal(L, "Cyperus_Bus");
}

/*
 * cyperus bus port object ************************************
 */
static const luaL_Reg _bus_port_meta[] = {
  {"__gc", cyperus_bus_port_gc},
  {"__index", cyperus_bus_port_index},
  {"__newindex", cyperus_bus_port_newindex},
  { NULL, NULL }
};
static const luaL_Reg _bus_port_methods[] = {
  {"connect", cyperus_bus_port_add_connection},
  { NULL, NULL }
};

int cyperus_bus_port_gc(lua_State* L) {
  printf("## __gc\n");
  return 0;
}
int cyperus_bus_port_newindex(lua_State* L) {
  printf("## __newindex\n");
  return 0;
}
int cyperus_bus_port_index(lua_State* L) {
  printf("## __index\n");
  return 0;
}

int cyperus_bus_port_add_connection(lua_State *L) {
  printf("Cyperus.c::cyperus_bus_port_add_onnection()\n");
}

int _build_bus_ports(lua_State* L, char *bus_path) {
  printf("Cyperus.c::cyperus_build_bus_ports()\n");
  char **bus_port_id_ins,
    **bus_port_name_ins,
    **bus_port_id_outs,
    **bus_port_name_outs;
  int error_code, root_path_size = 0, idx, num_ins, num_outs;

  error_code = libcyperus_list_bus_port(bus_path,
                                        &bus_port_id_ins,
                                        &bus_port_name_ins,
                                        &num_ins,
                                        &bus_port_id_outs,
                                        &bus_port_name_outs,
                                        &num_outs);

  printf("Cyperus.c::_build_bus_ports(), list_bus_port error_code: %d\n", error_code);
  char *full_path = malloc(sizeof(char));
  int full_path_length;

  /* bus port ins */
  lua_createtable(L, 1, 0);
  lua_createtable(L, 1, 0);
  for(idx=0; idx<num_ins; idx++) {

    lua_pushstring(L, "id");
    lua_pushstring(L, bus_port_id_ins[idx]);
    lua_rawset(L, -3);

    lua_pushstring(L, "name");
    lua_pushstring(L, bus_port_name_ins[idx]);
    lua_rawset(L, -3);

    full_path_length = strlen(bus_path) + 1 + strlen(bus_port_id_ins[idx]) + 1;
    full_path = realloc(full_path, sizeof(char) * full_path_length);
    snprintf(full_path, full_path_length, "%s:%s", bus_path, bus_port_id_ins[idx]);
    
    lua_pushstring(L, "full_path");
    lua_pushstring(L, full_path);
    lua_rawset(L, -3);

    lua_pushstring(L, "type");
    lua_pushstring(L, "bus");
    lua_rawset(L, -3);
    
    luaL_getmetatable(L, "Cyperus_Bus_Port");
    lua_setmetatable(L, -2);

    lua_rawseti(L, -2, idx + 1);
  }
  lua_pushstring(L, "ins");
  lua_insert(L, -2);
  lua_rawset(L, -3);

  /* bus port outs */
  lua_createtable(L, 1, 0);
  lua_createtable(L, 1, 0);
  for(idx=0; idx<num_outs; idx++) {

    lua_pushstring(L, "id");
    lua_pushstring(L, bus_port_id_outs[idx]);
    lua_rawset(L, -3);

    lua_pushstring(L, "name");
    lua_pushstring(L, bus_port_name_outs[idx]);
    lua_rawset(L, -3);

    full_path_length = strlen(bus_path) + 1 + strlen(bus_port_id_outs[idx]) + 1;
    full_path = realloc(full_path, sizeof(char) * full_path_length);
    snprintf(full_path, full_path_length, "%s:%s", bus_path, bus_port_id_outs[idx]);
    
    lua_pushstring(L, "full_path");
    lua_pushstring(L, full_path);
    lua_rawset(L, -3);

    lua_pushstring(L, "type");
    lua_pushstring(L, "bus");
    lua_rawset(L, -3);
    
    luaL_getmetatable(L, "Cyperus_Bus_Port");
    lua_setmetatable(L, -2);

    lua_rawseti(L, -2, idx + 1);
  }
  lua_pushstring(L, "outs");
  lua_insert(L, -2);
  lua_rawset(L, -3);
  
  free(full_path);
  return 1;
}

/* int add_cyperus_bus_port(lua_State *L, */
/*                          char *path, */
/*                          char *name, */
/*                          char *ins, */
/*                          char *outs, */
/*                          char **bus_id) { */
/*   printf("Cyperus.c::_add_cyperus_bus_port()\n"); */

/*   libcyperus_lua_bus_port_t *state = malloc(sizeof(libcyperus_lua_bus_port_t)); */
/*   int error_code, registry_key_size, idx; */
/*   char *registry_key; */

/*   error_code = libcyperus_add_bus_port(path, name, ins, outs, bus_id); */
/* } */

void register_cyperus_bus_port_class(lua_State* L) {
  int lib_id, meta_id;

  /* newclass = {} */
  lua_createtable(L, 0, 0);
  lib_id = lua_gettop(L);

  /* metatable = {} */
  luaL_newmetatable(L, "Cyperus_Bus_Port");
  meta_id = lua_gettop(L);
  luaL_setfuncs(L, _bus_port_meta, 0);

  /* metatable.__index = _bus_port_methods */
  luaL_newlib(L, _bus_port_methods);
  lua_setfield(L, meta_id, "__index");  
  
  /* metatable.__bus_port_metatable = _bus_port_meta */
  luaL_newlib(L, _bus_port_meta);
  lua_setfield(L, meta_id, "__bus_port_metatable");

  /* class.__bus_port_metatable = metatable */
  lua_setmetatable(L, lib_id);    
  
  /* _G["Cyperus_Bus_Port"] = newclass */
  lua_setglobal(L, "Cyperus_Bus_Port");
}
