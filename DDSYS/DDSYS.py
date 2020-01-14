from ctypes import *

DDSYS = cdll.LoadLibrary("../x64/DLLDebug/DDSYS.dll")

dof = DDSYS.dof
dof.restype = c_size_t
dof.argtypes = [ c_int, c_double ]

dof_fixed = DDSYS.dof_fixed
dof_fixed.restype = c_size_t
dof_fixed.argtypes = [ c_int, c_double ]

print(dof_fixed(0,1.0))
print(dof(1,1.0))