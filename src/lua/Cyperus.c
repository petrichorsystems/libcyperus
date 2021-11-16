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

static int newarray (lua_State *L) {
  int n = (int)luaL_checkinteger(L, 1);
  size_t nbytes = sizeof(NumArray) + (n - 1)*sizeof(double);
  NumArray *a = (NumArray *)lua_newuserdata(L, nbytes);
  a->size = n;
  return 1;  /* new userdatum is already on the stack */
}

static int setarray (lua_State *L) {
  NumArray *a = (NumArray *)lua_touserdata(L, 1);
  int index = (int)luaL_checkinteger(L, 2);
  double value = luaL_checknumber(L, 3);
  
  luaL_argcheck(L, a != NULL, 1, "`array' expected");
  
  luaL_argcheck(L, 1 <= index && index <= a->size, 2,
                "index out of range");
  
  a->values[index-1] = value;
  return 0;
}

static int getarray (lua_State *L) {
  NumArray *a = (NumArray *)lua_touserdata(L, 1);
  int index = (int)luaL_checkinteger(L, 2);
  
  luaL_argcheck(L, a != NULL, 1, "`array' expected");
    
  luaL_argcheck(L, 1 <= index && index <= a->size, 2,
                "index out of range");
    
  lua_pushnumber(L, a->values[index-1]);
  return 1;
}

static int getsize (lua_State *L) {
  NumArray *a = (NumArray *)lua_touserdata(L, 1);
  luaL_argcheck(L, a != NULL, 1, "`array' expected");
  lua_pushnumber(L, a->size);
  return 1;
}

static const struct luaL_Reg arraylib[] = {
  {"new", newarray},
  {"set", setarray},
  {"get", getarray},
  {"size", getsize},
  {NULL, NULL}
};
    
int luaopen_array (lua_State *L) {
  luaL_newlib(L, arraylib);
  return 1;
}

/*
 * Cyperus obj
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
int cyperus_new(lua_State* L) {
  printf("## new\n");

  char **ins, **outs;
  int num_ins, num_outs;
  int i;
  
  const char *osc_port_in = luaL_checkstring(L, 1);
  const char *osc_host_out = luaL_checkstring (L, 2);
  const char *osc_port_out = luaL_checkstring (L, 3);

  printf("osc_port_in: %s\n", osc_port_in);
  printf("osc_host_out: %s\n", osc_host_out);
  printf("osc_port_out: %s\n", osc_port_out);

  libcyperus_setup((char *)osc_port_in, (char *)osc_host_out, (char *)osc_port_out);
  libcyperus_list_mains(&ins, &num_ins, &outs, &num_outs);
  
  printf("num_ins: %d\n", num_ins);
  printf("num_outs: %d\n", num_outs);

  for(int i=0; i<4; i++) {
    printf("ins[%d]: %s\n", i, ins[i]);
    printf("outs[%d]: %s\n", i, outs[i]);
  }
  
  lua_newuserdata(L,sizeof(struct cyperus));
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
