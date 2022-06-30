import sys
from ctypes import cdll, POINTER, c_size_t, c_int, c_double, c_char_p
import numpy as np

import enum

if sys.platform == "linux":
    RODSDLL = cdll.LoadLibrary("./RODS_.so")
elif sys.platform == "win32":
    RODSDLL = cdll.LoadLibrary("./RODS.dll")


class DynamicSolver(enum.Enum):
    Newmark = 0
    Newmark_NL = 1
    StateSpace = 2
    StateSpace_NL = 3


class Direction(enum.Enum):
    X = 0
    Y = 1
    Z = 2
    RX = 3
    RY = 4
    RZ = 5


class Response(enum.Enum):
    DISP = 0
    VEL = 1
    ACC = 2
    FORCE = 3
    DEF = 4
    ALL = 5

class LocalAxis(enum.Enum):
    U1 = 0
    U2 = 1
    U3 = 2


set_damping_ratio = RODSDLL.set_damping_ratio
set_damping_ratio.argtypes = [c_double]

set_rayleigh_damping = RODSDLL.set_rayleigh_damping
set_rayleigh_damping.argtypes = [c_double, c_double]

set_num_modes_inherent_damping = RODSDLL.set_num_modes_inherent_damping
set_num_modes_inherent_damping.argtypes = [c_int]

add_dof_x = RODSDLL.add_dof_x
add_dof_x.restype = c_size_t
add_dof_x.argtypes = [c_int, c_double]

add_dof = RODSDLL.add_dof
add_dof.restype = c_size_t
add_dof.argtypes = [c_int, c_int, c_double]

set_dof_mass = RODSDLL.set_dof_mass
set_dof_mass.restype = c_size_t
set_dof_mass.argtypes = [c_int, c_double]

set_node_mass = RODSDLL.set_node_mass
set_node_mass.restype = c_size_t
set_node_mass.argtypes = [c_int, c_double]

add_node_1d = RODSDLL.add_node_1d
add_node_1d.restype = c_size_t
add_node_1d.argtypes = [c_int, c_double, c_int]

add_node_2d = RODSDLL.add_node_2d
add_node_2d.restype = c_size_t
add_node_2d.argtypes = [c_int, c_double, c_double, c_int, c_int, c_int]

add_node_3d = RODSDLL.add_node_3d
add_node_3d.restype = c_size_t
add_node_3d.argtypes = [c_int, c_double, c_double, c_double, c_int, c_int, c_int, c_int, c_int, c_int]

fix_dof = RODSDLL.fix_dof
fix_dof.argtypes = [c_int]

fix_node = RODSDLL.fix_node
fix_node.argtypes = [c_int]

fix_node_dof = RODSDLL.fix_node_dof
fix_node_dof.argtypes = [c_int, c_int]

add_spring = RODSDLL.add_spring
add_spring.restype = c_size_t
add_spring.argtypes = [c_int, c_int, c_int, c_double]

add_dashpot = RODSDLL.add_dashpot
add_dashpot.restype = c_size_t
add_dashpot.argtypes = [c_int, c_int, c_int, c_double]

add_inerter = RODSDLL.add_inerter
add_inerter.restype = c_size_t
add_inerter.argtypes = [c_int, c_int, c_int, c_double]

add_spis2 = RODSDLL.add_spis2
add_spis2.restype = c_size_t
add_spis2.argtypes = [c_int, c_int, c_int, c_int, c_double, c_double, c_double]

add_tvmd = RODSDLL.add_tvmd
add_tvmd.restype = c_size_t
add_tvmd.argtypes = [c_int, c_int, c_int, c_double, c_double, c_double]

add_spring_bilinear = RODSDLL.add_spring_bilinear
add_spring_bilinear.restype = c_size_t
add_spring_bilinear.argtypes = [c_int, c_int, c_int, c_double, c_double, c_double]

add_dashpot_maxwell = RODSDLL.add_dashpot_maxwell
add_dashpot_maxwell.restype = c_size_t
add_dashpot_maxwell.argtypes = [c_int, c_int, c_int, c_double, c_double, c_double]

add_spring_2d = RODSDLL.add_spring_2d
add_spring_2d.restype = c_size_t
add_spring_2d.argtypes = [c_int, c_int, c_int, c_double, c_int]

add_dashpot_2d = RODSDLL.add_dashpot_2d
add_dashpot_2d.restype = c_size_t
add_dashpot_2d.argtypes = [c_int, c_int, c_int, c_double, c_int]

add_inerter_2d = RODSDLL.add_inerter_2d
add_inerter_2d.restype = c_size_t
add_inerter_2d.argtypes = [c_int, c_int, c_int, c_double, c_int]

add_tvmd_2d = RODSDLL.add_tvmd_2d
add_tvmd_2d.restype = c_size_t
add_tvmd_2d.argtypes = [c_int, c_int, c_int, c_double, c_double, c_double, c_int]

add_spring_bilinear_2d = RODSDLL.add_spring_bilinear_2d
add_spring_bilinear_2d.restype = c_size_t
add_spring_bilinear_2d.argtypes = [c_int, c_int, c_int, c_double, c_double, c_double, c_int]

add_dashpot_maxwell_2d = RODSDLL.add_dashpot_maxwell_2d
add_dashpot_maxwell_2d.restype = c_size_t
add_dashpot_maxwell_2d.argtypes = [c_int, c_int, c_int, c_double, c_double, c_double, c_int]

add_spring_3d = RODSDLL.add_spring_3d
add_spring_3d.restype = c_size_t
add_spring_3d.argtypes = [c_int, c_int, c_int, c_double, c_int]

add_dashpot_3d = RODSDLL.add_dashpot_3d
add_dashpot_3d.restype = c_size_t
add_dashpot_3d.argtypes = [c_int, c_int, c_int, c_double, c_int]

add_inerter_3d = RODSDLL.add_inerter_3d
add_inerter_3d.restype = c_size_t
add_inerter_3d.argtypes = [c_int, c_int, c_int, c_double, c_int]

add_tvmd_3d = RODSDLL.add_tvmd_3d
add_tvmd_3d.restype = c_size_t
add_tvmd_3d.argtypes = [c_int, c_int, c_int, c_double, c_double, c_double, c_int]

add_spring_bilinear_3d = RODSDLL.add_spring_bilinear_3d
add_spring_bilinear_3d.restype = c_size_t
add_spring_bilinear_3d.argtypes = [c_int, c_int, c_int, c_double, c_double, c_double, c_int]

add_dashpot_maxwell_3d = RODSDLL.add_dashpot_maxwell_3d
add_dashpot_maxwell_3d.restype = c_size_t
add_dashpot_maxwell_3d.argtypes = [c_int, c_int, c_int, c_double, c_double, c_double, c_int]

add_truss_elastic_2d = RODSDLL.add_truss_elastic_2d
add_truss_elastic_2d.restype = c_size_t
add_truss_elastic_2d.argtypes = [c_int, c_int, c_int, c_double]

add_truss_elastic_3d = RODSDLL.add_truss_elastic_3d
add_truss_elastic_3d.restype = c_size_t
add_truss_elastic_3d.argtypes = [c_int, c_int, c_int, c_double]

add_frame_elastic_2d = RODSDLL.add_frame_elastic_2d
add_frame_elastic_2d.restype = c_size_t
add_frame_elastic_2d.argtypes = [c_int, c_int, c_int, c_double, c_double]

add_frame_elastic_3d = RODSDLL.add_frame_elastic_3d
add_frame_elastic_3d.restype = c_size_t
add_frame_elastic_3d.argtypes = [c_int, c_int, c_int, c_double, c_double, c_double, c_double]

add_quad4_elastic = RODSDLL.add_quad4_elastic
add_quad4_elastic.restype = c_size_t
add_quad4_elastic.argtypes = [c_int, c_int, c_int, c_int, c_int, c_double, c_double, c_double]

add_wave = RODSDLL.add_wave
add_wave.restype = c_size_t
add_wave.argtypes = [c_int, c_double, c_char_p]

assemble_matrix = RODSDLL.assemble_matrix
assemble_matrix.restype = c_size_t

solve_eigen = RODSDLL.solve_eigen

add_dof_recorder = RODSDLL.add_dof_recorder
add_dof_recorder.restype = c_size_t
add_dof_recorder.argtypes = [c_int, c_int, c_char_p]

add_dof_to_recorder = RODSDLL.add_dof_to_recorder
add_dof_to_recorder.restype = c_size_t
add_dof_to_recorder.argtypes = [c_int, c_int]

add_node_dof_to_recorder = RODSDLL.add_node_dof_to_recorder
add_node_dof_to_recorder.restype = c_size_t
add_node_dof_to_recorder.argtypes = [c_int, c_int, c_int]

add_ele_recorder = RODSDLL.add_ele_recorder
add_ele_recorder.restype = c_size_t
add_ele_recorder.argtypes = [c_int, c_int, c_char_p]

add_ele_to_recorder = RODSDLL.add_ele_to_recorder
add_ele_to_recorder.restype = c_size_t
add_ele_to_recorder.argtypes = [c_int, c_int]

add_dof_recorder_with_dofs_ = RODSDLL.add_dof_recorder_with_dofs
add_dof_recorder_with_dofs_.restype = c_size_t
add_dof_recorder_with_dofs_.argtypes = [c_int, POINTER(c_int), c_int, c_int, c_char_p]
def add_dof_recorder_with_dofs(id, dofIds_, rt, fn):
    n = len(dofIds_)
    dofIds_ = np.array(dofIds_, dtype=np.int32)
    dofIds = dofIds_.ctypes.data_as(POINTER(c_int))
    return add_dof_recorder_with_dofs_(id, dofIds, n, rt, c_char_p(fn))

add_ele_recorder_with_eles_ = RODSDLL.add_ele_recorder_with_eles
add_ele_recorder_with_eles_.restype = c_size_t
add_ele_recorder_with_eles_.argtypes = [c_int, POINTER(c_int), c_int, c_int, c_char_p]
def add_ele_recorder_with_eles(id, dofIds_, rt, fn):
    n = len(dofIds_)
    dofIds_ = np.array(dofIds_, dtype=np.int32)
    dofIds = dofIds_.ctypes.data_as(POINTER(c_int))
    return add_ele_recorder_with_eles_(id, dofIds, n, rt, c_char_p(fn))

set_dynamic_solver = RODSDLL.set_dynamic_solver
set_dynamic_solver.argtypes = [c_int]

active_ground_motion = RODSDLL.active_ground_motion
active_ground_motion.argtypes = [c_int, c_int, c_double]

solve_seismic_response = RODSDLL.solve_seismic_response
solve_seismic_response.restype = c_size_t
solve_seismic_response.argtypes = [c_int]

print_info = RODSDLL.print_info
clear = RODSDLL.clear

export_gmsh = RODSDLL.export_gmsh
export_gmsh.argtypes = [c_char_p]

export_modal_gmsh = RODSDLL.export_modal_gmsh
export_modal_gmsh.argtypes = [c_char_p, c_int]

set_response_gmsh = RODSDLL.set_response_gmsh
set_response_gmsh.argtypes = [c_char_p, c_int]