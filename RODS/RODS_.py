from ctypes import cdll, POINTER, c_size_t, c_int, c_double, c_char_p, create_string_buffer, byref
import numpy as np

# RODSDLL = cdll.LoadLibrary("../x64/DLLDebug/RODS.dll")
RODSDLL = cdll.LoadLibrary("../x64/DLLRelease/RODS.dll")

add_dof = RODSDLL.add_dof
add_dof.restype = c_size_t
add_dof.argtypes = [c_int, c_double]

fix_dof = RODSDLL.fix_dof
fix_dof.argtypes = [c_int]

add_spring = RODSDLL.add_spring
add_spring.restype = c_size_t
add_spring.argtypes = [c_int, c_int, c_int, c_double]

add_dashpot = RODSDLL.add_dashpot
add_dashpot.restype = c_size_t
add_dashpot.argtypes = [c_int, c_int, c_int, c_double]

add_timeseries = RODSDLL.add_timeseries
add_timeseries.restype = c_size_t
add_timeseries.argtypes = [c_int, c_double, c_char_p]

assemble_matrix = RODSDLL.assemble_matrix
assemble_matrix.restype = c_size_t

add_dof_recorder = RODSDLL.add_dof_recorder
add_dof_recorder.restype = c_size_t
add_dof_recorder.argtypes = [c_int, POINTER(c_int), c_int, c_int, c_char_p]

add_ele_recorder = RODSDLL.add_ele_recorder
add_ele_recorder.restype = c_size_t
add_ele_recorder.argtypes = [c_int, POINTER(c_int), c_int, c_int, c_char_p]

solve_seismic_response = RODSDLL.solve_seismic_response
solve_seismic_response.restype = c_size_t
solve_seismic_response.argtypes = [c_int, c_double, c_int]

m = 1.0
add_dof(0,m)
add_dof(1,m)

fix_dof(0)

k = 100
add_spring(0,0,1,k)

zeta = 0.02
c = 2.0*zeta*np.sqrt(m*k)
add_dashpot(1,0,1,c)

dt = 0.005
add_timeseries(0, dt, c_char_p(b'EQ-S-1.txt'))

dofIds_ = np.array([0, 1], dtype=np.int32)
# dofIds = (c_int*len(dofIds_))(*dofIds_)
dofIds = dofIds_.ctypes.data_as(POINTER(c_int))
n = len(dofIds_)

assemble_matrix()

add_dof_recorder(0, dofIds, n, 0, c_char_p(b'disp.csv'))
add_ele_recorder(0, dofIds, n, 3, c_char_p(b'force.csv'))

solve_seismic_response(0, 1.0, 1)