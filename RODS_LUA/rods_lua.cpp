#include <stdio.h>
#include "rods.h"

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#define LUA_WRAPPER(FUNC, ARG_1_TYPE)\
static int lua_##FUNC(lua_State *L) {\
FUNC(lua_to##ARG_1_TYPE(L, 1)); return 0;}

#define LUA_WRAPPER_2(FUNC,\
ARG_1_TYPE,\
ARG_2_TYPE\
)\
static int lua_##FUNC(lua_State *L) {\
FUNC(\
lua_to##ARG_1_TYPE(L, 1),\
lua_to##ARG_2_TYPE(L, 2)\
); return 0;}

#define LUA_WRAPPER_3(FUNC,\
ARG_1_TYPE,\
ARG_2_TYPE,\
ARG_3_TYPE\
)\
static int lua_##FUNC(lua_State *L) {\
FUNC(\
lua_to##ARG_1_TYPE(L, 1),\
lua_to##ARG_2_TYPE(L, 2),\
lua_to##ARG_3_TYPE(L, 3)\
); return 0;}

#define LUA_REG(FUNC) lua_register(L, #FUNC, lua_##FUNC);

LUA_WRAPPER(set_name, string)
LUA_WRAPPER(set_work_dir, string)

int main(int argc, char* argv[])
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    LUA_REG(set_name)
    LUA_REG(set_work_dir)

    if (argc < 2)
    {
        printf("Error: A filename must be provided!\n");
    }
    else
    {
        luaL_dofile(L, argv[1]);
    }
    
    lua_close(L);
    return 0;
}