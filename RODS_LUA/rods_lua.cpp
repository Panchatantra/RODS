#include <stdio.h>
#include "rods.h"

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

// Lua function wrapper macros: no variables, no return values
#define LUA_WRAP_0(FUNC)\
static int lua_##FUNC(lua_State *L) {\
FUNC(); return 0;}

// Lua function wrapper macros: no variables, with return values
#define LUA_WRAP_0_R(FUNC, RET_TYPE)\
static int lua_##FUNC(lua_State *L) {\
lua_push##RET_TYPE(L, FUNC()); return 1;}

// Lua function wrapper macros: one variable, no return values
#define LUA_WRAP_1(FUNC, ARG_1_TYPE)\
static int lua_##FUNC(lua_State *L) {\
FUNC(lua_to##ARG_1_TYPE(L, 1)); return 0;}

// Lua function wrapper macros: one variable, with return values
#define LUA_WRAP_1_R(FUNC, ARG_1_TYPE, RET_TYPE)\
static int lua_##FUNC(lua_State *L) {\
lua_push##RET_TYPE(L, FUNC(lua_to##ARG_1_TYPE(L, 1))); return 1;}

// Lua function wrapper macros: two variables, no return values
#define LUA_WRAP_2(FUNC,\
ARG_1_TYPE,\
ARG_2_TYPE\
)\
static int lua_##FUNC(lua_State *L) {\
FUNC(\
lua_to##ARG_1_TYPE(L, 1),\
lua_to##ARG_2_TYPE(L, 2)\
); return 0;}

// Lua function wrapper macros: two variables, with return values
#define LUA_WRAP_2_R(FUNC,\
ARG_1_TYPE,\
ARG_2_TYPE,\
RET_TYPE\
)\
static int lua_##FUNC(lua_State *L) {\
lua_push##RET_TYPE(L, FUNC(\
lua_to##ARG_1_TYPE(L, 1),\
lua_to##ARG_2_TYPE(L, 2)\
)); return 1;}

// Lua function wrapper macros: three variables, no return values
#define LUA_WRAP_3(FUNC,\
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

// Lua function wrapper macros: three variables, with return values
#define LUA_WRAP_3_R(FUNC,\
ARG_1_TYPE,\
ARG_2_TYPE,\
ARG_3_TYPE,\
RET_TYPE\
)\
static int lua_##FUNC(lua_State *L) {\
lua_push##RET_TYPE(L, FUNC(\
lua_to##ARG_1_TYPE(L, 1),\
lua_to##ARG_2_TYPE(L, 2),\
lua_to##ARG_3_TYPE(L, 3)\
)); return 1;}

// Lua function wrapper macros: four variables, no return values
#define LUA_WRAP_4(FUNC,\
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

// Lua function wrapper macros: four variables, with return values
#define LUA_WRAP_4_R(FUNC,\
ARG_1_TYPE,\
ARG_2_TYPE,\
ARG_3_TYPE,\
ARG_4_TYPE,\
RET_TYPE\
)\
static int lua_##FUNC(lua_State *L) {\
lua_push##RET_TYPE(L, FUNC( \
lua_to##ARG_1_TYPE(L, 1),\
lua_to##ARG_2_TYPE(L, 2),\
lua_to##ARG_3_TYPE(L, 3),\
lua_to##ARG_4_TYPE(L, 4)\
) ); return 1;}

// Lua function wrapper macros: five variables, no return values
#define LUA_WRAP_5(FUNC,\
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

// Lua function wrapper macros: five variables, with return values
#define LUA_WRAP_5_R(FUNC,\
ARG_1_TYPE,\
ARG_2_TYPE,\
ARG_3_TYPE,\
ARG_4_TYPE,\
ARG_5_TYPE,\
RET_TYPE\
)\
static int lua_##FUNC(lua_State *L) {\
lua_push##RET_TYPE(L, FUNC( \
lua_to##ARG_1_TYPE(L, 1),\
lua_to##ARG_2_TYPE(L, 2),\
lua_to##ARG_3_TYPE(L, 3),\
lua_to##ARG_4_TYPE(L, 4),\
lua_to##ARG_5_TYPE(L, 5)\
)) ; return 1;}

// Lua function wrapper macros: six variables, no return values
#define LUA_WRAP_6(FUNC,\
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

// Lua function wrapper macros: six variables, with return values
#define LUA_WRAP_6_R(FUNC,\
ARG_1_TYPE,\
ARG_2_TYPE,\
ARG_3_TYPE,\
ARG_4_TYPE,\
ARG_5_TYPE,\
ARG_6_TYPE,\
RET_TYPE\
)\
static int lua_##FUNC(lua_State *L) {\
lua_push##RET_TYPE(L, FUNC(\
lua_to##ARG_1_TYPE(L, 1),\
lua_to##ARG_2_TYPE(L, 2),\
lua_to##ARG_3_TYPE(L, 3),\
lua_to##ARG_4_TYPE(L, 4),\
lua_to##ARG_5_TYPE(L, 5),\
lua_to##ARG_6_TYPE(L, 6)\
)) ; return 1;}


// Lua function wrapper macros: seven variables, no return values
#define LUA_WRAP_7(FUNC,\
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

// Lua function wrapper macros: seven variables, with return values
#define LUA_WRAP_7_R(FUNC,\
ARG_1_TYPE,\
ARG_2_TYPE,\
ARG_3_TYPE,\
ARG_4_TYPE,\
ARG_5_TYPE,\
ARG_6_TYPE,\
ARG_7_TYPE,\
RET_TYPE\
)\
static int lua_##FUNC(lua_State *L) {\
lua_push##RET_TYPE(L, FUNC(\
lua_to##ARG_1_TYPE(L, 1),\
lua_to##ARG_2_TYPE(L, 2),\
lua_to##ARG_3_TYPE(L, 3),\
lua_to##ARG_4_TYPE(L, 4),\
lua_to##ARG_5_TYPE(L, 5),\
lua_to##ARG_6_TYPE(L, 6),\
lua_to##ARG_7_TYPE(L, 7)\
)) ; return 1;}

// Lua function wrapper macros: eight variables, no return values
#define LUA_WRAP_8(FUNC,\
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

// Lua function wrapper macros: eight variables, with return values
#define LUA_WRAP_8_R(FUNC,\
ARG_1_TYPE,\
ARG_2_TYPE,\
ARG_3_TYPE,\
ARG_4_TYPE,\
ARG_5_TYPE,\
ARG_6_TYPE,\
ARG_7_TYPE,\
ARG_8_TYPE,\
RET_TYPE\
)\
static int lua_##FUNC(lua_State *L) {\
lua_push##RET_TYPE(L, FUNC(\
lua_to##ARG_1_TYPE(L, 1),\
lua_to##ARG_2_TYPE(L, 2),\
lua_to##ARG_3_TYPE(L, 3),\
lua_to##ARG_4_TYPE(L, 4),\
lua_to##ARG_5_TYPE(L, 5),\
lua_to##ARG_6_TYPE(L, 6),\
lua_to##ARG_7_TYPE(L, 7),\
lua_to##ARG_8_TYPE(L, 8)\
)); return 1;}

// Lua function wrapper macros: nine variables, no return values
#define LUA_WRAP_9(FUNC,\
ARG_1_TYPE,\
ARG_2_TYPE,\
ARG_3_TYPE,\
ARG_4_TYPE,\
ARG_5_TYPE,\
ARG_6_TYPE,\
ARG_7_TYPE,\
ARG_8_TYPE,\
ARG_9_TYPE\
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
lua_to##ARG_8_TYPE(L, 8),\
lua_to##ARG_9_TYPE(L, 9)\
); return 0;}

// Lua function wrapper macros: nine variables, with return values
#define LUA_WRAP_9_R(FUNC,\
ARG_1_TYPE,\
ARG_2_TYPE,\
ARG_3_TYPE,\
ARG_4_TYPE,\
ARG_5_TYPE,\
ARG_6_TYPE,\
ARG_7_TYPE,\
ARG_8_TYPE,\
ARG_9_TYPE,\
RET_TYPE\
)\
static int lua_##FUNC(lua_State *L) {\
lua_push##RET_TYPE(L, FUNC(\
lua_to##ARG_1_TYPE(L, 1),\
lua_to##ARG_2_TYPE(L, 2),\
lua_to##ARG_3_TYPE(L, 3),\
lua_to##ARG_4_TYPE(L, 4),\
lua_to##ARG_5_TYPE(L, 5),\
lua_to##ARG_6_TYPE(L, 6),\
lua_to##ARG_7_TYPE(L, 7),\
lua_to##ARG_8_TYPE(L, 8),\
lua_to##ARG_9_TYPE(L, 9)\
)); return 1;}

// Lua function wrapper macros: ten variables, no return values
#define LUA_WRAP_10(FUNC,\
ARG_1_TYPE,\
ARG_2_TYPE,\
ARG_3_TYPE,\
ARG_4_TYPE,\
ARG_5_TYPE,\
ARG_6_TYPE,\
ARG_7_TYPE,\
ARG_8_TYPE,\
ARG_9_TYPE,\
ARG_10_TYPE\
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
lua_to##ARG_8_TYPE(L, 8),\
lua_to##ARG_9_TYPE(L, 9),\
lua_to##ARG_10_TYPE(L, 10)\
); return 0;}

// Lua function wrapper macros: ten variables, with return values
#define LUA_WRAP_10_R(FUNC,\
ARG_1_TYPE,\
ARG_2_TYPE,\
ARG_3_TYPE,\
ARG_4_TYPE,\
ARG_5_TYPE,\
ARG_6_TYPE,\
ARG_7_TYPE,\
ARG_8_TYPE,\
ARG_9_TYPE,\
ARG_10_TYPE,\
RET_TYPE\
)\
static int lua_##FUNC(lua_State *L) {\
lua_push##RET_TYPE(L, FUNC(\
lua_to##ARG_1_TYPE(L, 1),\
lua_to##ARG_2_TYPE(L, 2),\
lua_to##ARG_3_TYPE(L, 3),\
lua_to##ARG_4_TYPE(L, 4),\
lua_to##ARG_5_TYPE(L, 5),\
lua_to##ARG_6_TYPE(L, 6),\
lua_to##ARG_7_TYPE(L, 7),\
lua_to##ARG_8_TYPE(L, 8),\
lua_to##ARG_9_TYPE(L, 9),\
lua_to##ARG_10_TYPE(L, 10)\
)); return 1;}


#define LUA_REG(FUNC) lua_register(L, #FUNC, lua_##FUNC);

#define lua_register_integer(L,n,i) (lua_pushinteger(L, (i)), lua_setglobal(L, (n)))
#define LUA_REG_INT(i) lua_register_integer(L, #i, i);

LUA_WRAP_1(set_name, string)
LUA_WRAP_1(set_work_dir, string)

LUA_WRAP_1(set_damping_ratio, number)
LUA_WRAP_2(set_rayleigh_damping, number, number)
LUA_WRAP_1(set_num_modes_inherent_damping, integer)

LUA_WRAP_2_R(add_dof_x, integer, number, integer)
LUA_WRAP_3_R(add_dof, integer, integer, number, integer)
LUA_WRAP_1(fix_dof, integer)
LUA_WRAP_3_R(add_node_1d, integer, number, integer, integer)
LUA_WRAP_6_R(add_node_2d, integer, number, number, integer, integer, integer, integer)
LUA_WRAP_10_R(add_node_3d, integer, number, number, number, integer, integer, integer,
                            integer, integer, integer, integer)

LUA_WRAP_4_R(add_spring, integer, integer, integer, number, integer)
LUA_WRAP_4_R(add_dashpot, integer, integer, integer, number, integer)
LUA_WRAP_4_R(add_inerter, integer, integer, integer, number, integer)
LUA_WRAP_6_R(add_tvmd, integer, integer, integer, number, number, number, integer)
LUA_WRAP_5_R(add_dashpot_exp, integer, integer, integer, number, number, integer)
LUA_WRAP_6_R(add_dashpot_maxwell, integer, integer, integer, number, number, number, integer)
LUA_WRAP_6_R(add_spring_bilinear, number, number, number, number, number, number, integer)

LUA_WRAP_0_R(assemble_matrix, integer)
LUA_WRAP_0_R(solve_eigen, integer)
LUA_WRAP_1(print_info, integer)

LUA_WRAP_3_R(add_wave, integer, number, string, integer)

LUA_WRAP_3_R(add_dof_recorder, integer, integer, string, integer)
LUA_WRAP_2_R(add_dof_to_recorder, integer, integer, integer)
LUA_WRAP_3_R(add_ele_recorder, integer, integer, string, integer)
LUA_WRAP_2_R(add_ele_to_recorder, integer, integer, integer)

LUA_WRAP_1(record_all_dof_response, number)
LUA_WRAP_1(record_all_ele_response, number)

LUA_WRAP_1(set_dynamic_solver, integer)
LUA_WRAP_3(active_ground_motion, integer, integer, number)
LUA_WRAP_1_R(solve_seismic_response, integer, integer)
LUA_WRAP_1(save_to_json, string)
LUA_WRAP_1(load_from_json, string)


int main(int argc, char* argv[])
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    const int DISP = 0;
    const int VEL = 1;
    const int ACC = 2;
    const int FORCE = 3;
    const int DEF = 4;
    const int ALL = 5;

    LUA_REG_INT(DISP)
    LUA_REG_INT(VEL)
    LUA_REG_INT(ACC)
    LUA_REG_INT(FORCE)
    LUA_REG_INT(DEF)
    LUA_REG_INT(ALL)

    const int Newmark = 0;
    const int Newmark_NL = 1;
    const int StateSpace = 2;
    const int StateSpace_NL = 3;

    LUA_REG_INT(Newmark)
    LUA_REG_INT(Newmark_NL)
    LUA_REG_INT(StateSpace)
    LUA_REG_INT(StateSpace_NL)

    const int X = 0;
    const int Y = 1;
    const int Z = 2;
    const int RX = 3;
    const int RY = 4;
    const int RZ = 5;

    LUA_REG_INT(X)
    LUA_REG_INT(Y)
    LUA_REG_INT(Z)
    LUA_REG_INT(RX)
    LUA_REG_INT(RY)
    LUA_REG_INT(RZ)

    const int U1 = 0;
    const int U2 = 1;
    const int U3 = 2;

    LUA_REG_INT(U1)
    LUA_REG_INT(U2)
    LUA_REG_INT(U3)

    LUA_REG(set_name)
    LUA_REG(set_work_dir)
    LUA_REG(set_damping_ratio)
    LUA_REG(set_rayleigh_damping)
    LUA_REG(set_num_modes_inherent_damping)

    LUA_REG(add_dof_x)
    LUA_REG(add_dof)
    LUA_REG(fix_dof)
    LUA_REG(add_node_1d)
    LUA_REG(add_node_2d)
    LUA_REG(add_node_3d)

    LUA_REG(add_spring)
    LUA_REG(add_dashpot)
    LUA_REG(add_inerter)
    LUA_REG(add_tvmd)
    LUA_REG(add_dashpot_exp)
    LUA_REG(add_dashpot_maxwell)
    LUA_REG(add_spring_bilinear)

    LUA_REG(assemble_matrix)
    LUA_REG(solve_eigen)
    LUA_REG(print_info)

    LUA_REG(add_wave)
    LUA_REG(add_dof_recorder)
    LUA_REG(add_dof_to_recorder)
    LUA_REG(add_ele_recorder)
    LUA_REG(add_ele_to_recorder)

    LUA_REG(record_all_dof_response)
    LUA_REG(record_all_ele_response)

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