#include <lua.h>
#include <lauxlib.h>
#include <stdlib.h>
#include <stdio.h>

void register_cyperus_class(lua_State* L);

int cyperus_gc();
int cyperus_index();
int cyperus_newindex();
int cyperus_dosomething();
int cyperus_new();


struct cyperus {
  int x;
};

static const luaL_Reg _meta[] = {
    {"__gc", cyperus_gc},
    {"__index", cyperus_index},
    {"__newindex", cyperus_newindex},
    { NULL, NULL }
};
static const luaL_Reg _methods[] = {
    {"new", cyperus_new},
    {"dosomething", cyperus_dosomething},
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
int cyperus_dosomething(lua_State* L) {
  printf("## dosomething\n");
  return 0;
}
int cyperus_new(lua_State* L) {
  printf("## new\n");

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
