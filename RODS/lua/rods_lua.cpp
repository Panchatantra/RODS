#include <stdio.h>
#include "rods.h"

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

static int lua_set_name(lua_State *L) {
    const char* name = lua_tostring(L, 1);
    set_name(name);
    return 0;
}

int main(int argc, char* argv[])
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    lua_register(L, "set_name", lua_set_name);

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