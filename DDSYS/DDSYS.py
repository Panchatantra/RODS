from ctypes import *

DDSYS = cdll.LoadLibrary("DDSYS.dll")

dof = DDSYS.dof
dof.restype = c_size_t
dof.argtypes = [ c_int, c_double, c_bool ]

print(dof(0,1,True))
print(dof(1,1,False))