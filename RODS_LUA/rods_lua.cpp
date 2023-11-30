#include <stdio.h>
#include "rods.h"

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#define LUA_WRAPPER_0(FUNC)\
static int lua_##FUNC(lua_State *L) {\
FUNC(); return 0;}

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

#define LUA_WRAPPER_4(FUNC,\
ARG_1_TYPE,\
ARG_2_TYPE,\
ARG_3_TYPE,\
ARG_4_TYPE\
)\
static int lua_##FUNC(lua_State *L) {\
FUNC(\
lua_to##ARG_1_TYPE(L, 1),\
lua_to##ARG_2_TYPE(L, 2),\
lua_to##ARG_3_TYPE(L, 3),\
lua_to##ARG_4_TYPE(L, 4)\
); return 0;}

#define LUA_WRAPPER_5(FUNC,\
ARG_1_TYPE,\
ARG_2_TYPE,\
ARG_3_TYPE,\
ARG_4_TYPE,\
ARG_5_TYPE\
)\
static int lua_##FUNC(lua_State *L) {\
FUNC(\
lua_to##ARG_1_TYPE(L, 1),\
lua_to##ARG_2_TYPE(L, 2),\
lua_to##ARG_3_TYPE(L, 3),\
lua_to##ARG_4_TYPE(L, 4),\
lua_to##ARG_5_TYPE(L, 5)\
); return 0;}

#define LUA_WRAPPER_6(FUNC,\
ARG_1_TYPE,\
ARG_2_TYPE,\
ARG_3_TYPE,\
ARG_4_TYPE,\
ARG_5_TYPE,\
ARG_6_TYPE\
)\
static int lua_##FUNC(lua_State *L) {\
FUNC(\
lua_to##ARG_1_TYPE(L, 1),\
lua_to##ARG_2_TYPE(L, 2),\
lua_to##ARG_3_TYPE(L, 3),\
lua_to##ARG_4_TYPE(L, 4),\
lua_to##ARG_5_TYPE(L, 5),\
lua_to##ARG_6_TYPE(L, 6)\
); return 0;}

#define LUA_WRAPPER_7(FUNC,\
ARG_1_TYPE,\
ARG_2_TYPE,\
ARG_3_TYPE,\
ARG_4_TYPE,\
ARG_5_TYPE,\
ARG_6_TYPE,\
ARG_7_TYPE\
)\
static int lua_##FUNC(lua_State *L) {\
FUNC(\
lua_to##ARG_1_TYPE(L, 1),\
lua_to##ARG_2_TYPE(L, 2),\
lua_to##ARG_3_TYPE(L, 3),\
lua_to##ARG_4_TYPE(L, 4),\
lua_to##ARG_5_TYPE(L, 5),\
lua_to##ARG_6_TYPE(L, 6),\
lua_to##ARG_7_TYPE(L, 7)\
); return 0;}

#define LUA_WRAPPER_8(FUNC,\
ARG_1_TYPE,\
ARG_2_TYPE,\
ARG_3_TYPE,\
ARG_4_TYPE,\
ARG_5_TYPE,\
ARG_6_TYPE,\
ARG_7_TYPE,\
ARG_8_TYPE\
)\
static int lua_##FUNC(lua_State *L) {\
FUNC(\
lua_to##ARG_1_TYPE(L, 1),\
lua_to##ARG_2_TYPE(L, 2),\
lua_to##ARG_3_TYPE(L, 3),\
lua_to##ARG_4_TYPE(L, 4),\
lua_to##ARG_5_TYPE(L, 5),\
lua_to##ARG_6_TYPE(L, 6),\
lua_to##ARG_7_TYPE(L, 7),\
lua_to##ARG_8_TYPE(L, 8)\
); return 0;}

#define LUA_REG(FUNC) lua_register(L, #FUNC, lua_##FUNC);

LUA_WRAPPER(set_name, string)
LUA_WRAPPER(set_work_dir, string)

LUA_WRAPPER(set_damping_ratio, number)
LUA_WRAPPER_2(set_rayleigh_damping, number, number)
LUA_WRAPPER(set_num_modes_inherent_damping, number)

LUA_WRAPPER_2(add_dof_x, number, number)
LUA_WRAPPER(fix_dof, number)

LUA_WRAPPER_4(add_spring, number, number, number, number)
LUA_WRAPPER_4(add_dashpot, number, number, number, number)

LUA_WRAPPER_0(assemble_matrix)
LUA_WRAPPER_0(solve_eigen)
LUA_WRAPPER(print_info, number)

LUA_WRAPPER_3(add_wave, number, number, string)

LUA_WRAPPER_3(add_dof_recorder, number, number, string)
LUA_WRAPPER_2(add_dof_to_recorder, number, number)
LUA_WRAPPER_3(add_ele_recorder, number, number, string)
LUA_WRAPPER_2(add_ele_to_recorder, number, number)

LUA_WRAPPER(set_dynamic_solver, number)
LUA_WRAPPER_3(active_ground_motion, number, number, number)
LUA_WRAPPER(solve_seismic_response, number)
LUA_WRAPPER(save_to_json, string)


int main(int argc, char* argv[])
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    LUA_REG(set_name)
    LUA_REG(set_work_dir)
    LUA_REG(set_damping_ratio)
    LUA_REG(set_rayleigh_damping)
    LUA_REG(set_num_modes_inherent_damping)
    LUA_REG(add_dof_x)
    LUA_REG(fix_dof)
    LUA_REG(add_spring)
    LUA_REG(add_dashpot)

    LUA_REG(assemble_matrix)
    LUA_REG(solve_eigen)
    LUA_REG(print_info)

    LUA_REG(add_wave)
    LUA_REG(add_dof_recorder)
    LUA_REG(add_dof_to_recorder)
    LUA_REG(add_ele_recorder)
    LUA_REG(add_ele_to_recorder)
    LUA_REG(set_dynamic_solver)
    LUA_REG(active_ground_motion)
    LUA_REG(solve_seismic_response)
    LUA_REG(save_to_json)

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