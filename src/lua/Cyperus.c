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
  char *bus_id;
  int root_path_size = 0;
  
  lua_pushlightuserdata(L, (void *)&REGISTRY_CYPERUS_STATE_KEY);
  lua_gettable(L, LUA_REGISTRYINDEX);
 
  state = (libcyperus_lua_cyperus_t *)lua_touserdata(L, -1);

  if(state->root_bus_exists) {
    printf("i'm a meaningful error code about a root bus already existing!! boo!\n");
    return -1;
  }
  
  add_cyperus_bus(L, "/", "main0", "in", "out", &bus_id);

  printf("Cyperus.c::cyperus_get_root(), new bus_id: %s\n", bus_id);
  
  root_path_size = sizeof(char) + (1 + strlen(bus_id) + 1);
  state->root_bus_path = malloc(root_path_size);
  snprintf(state->root_bus_path, root_path_size, "/%s", bus_id);
  
  state->root_bus_exists = 1;
  
  luaL_getmetatable(L, "Cyperus_Bus");
  lua_setmetatable(L, -2);
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

int luaopen_Cyperus(lua_State *L) {
  register_cyperus_class(L);
  register_cyperus_bus_class(L);
  return 1;
}

/*
 * cyperus bus object ************************************
 */
static const luaL_Reg _bus_meta[] = {
    {"__gc", cyperus_bus_gc},
    {"__index", cyperus_bus_index},
    {"__newindex", cyperus_bus_newindex},
    {"bus_id", NULL},    
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

int add_cyperus_bus(lua_State *L,
                    char *path,
                    char *name,
                    char *ins,
                    char *outs,
                    char **bus_id) {
  printf("Cyperus.c::_add_cyperus_bus()\n");

  libcyperus_lua_bus_t *state = malloc(sizeof(libcyperus_lua_bus_t));
  int error_code, registry_key_size, idx;
  char *registry_key;

  error_code = libcyperus_add_bus(path, name, ins, outs, bus_id);
  build_cyperus_bus_registry_key(*bus_id, &registry_key);
  
  lua_pushlightuserdata(L, (void *)&registry_key);
  lua_pushlightuserdata(L, (void *)state);
  lua_settable(L, LUA_REGISTRYINDEX);

  /* luaL_getmetatable(L, "Cyperus_Bus"); */
  /* lua_setmetatable(L, -2); */
}

int cyperus_bus_get_ins(lua_State *L) {
  printf("Cyperus.c::cyperus_bus_get_ins()\n");
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
