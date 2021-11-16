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
  printf("## dosomething\n");
  return 0; 
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
  return 1;
}
