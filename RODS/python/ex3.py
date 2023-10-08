import RODS
import numpy as np
import matplotlib.pyplot as plt
plt.rcParams['font.sans-serif'] = ['Microsoft Yahei']  # 用来正常显示中文标签
plt.rcParams['axes.unicode_minus'] = False  # 用来正常显示负号

mass = 0.005
E = 32.5
A_b = 80000.0
A_c = 160000.0
I_b = 1066666666.66667
I_c = 2133333333.33333
EA_b = E * A_b
EA_c = E * A_c
EI_b = E * I_b
EI_c = E * I_c

print("%e, %e"%(EA_b, EI_b))
print("%e, %e"%(EA_c, EI_c))

# 设置固有阻尼比
RODS.set_damping_ratio(0.05)

# 采用 Rayleigh 阻尼，设置 Rayleigh 阻尼的两个计算圆频率
PI = 3.14159265
RODS.set_rayleigh_damping(2.0*PI/0.36, 2.0*PI/0.1)

nodeCoord = [
    [-6000, 0],
	[-6000, 3000],
	[-6000, 6000],
	[-6000, 9000],
	[0    , 0],
	[0    , 3000],
	[0    , 6000],
	[0    , 9000],
	[6000 , 0],
	[6000 , 3000],
	[6000 , 6000],
	[6000 , 9000]
	]

i = 0
for x, z in nodeCoord:
    RODS.add_dof(3*i+1, RODS.Direction.X.value, mass)
    RODS.add_dof(3*i+2, RODS.Direction.Z.value, mass)
    RODS.add_dof(3*i+3, RODS.Direction.RY.value, mass)
    RODS.add_node_2d(i+1, x, z, 3*i+1, 3*i+2, 3*i+3)
    i += 1

RODS.fix_node(1)
RODS.fix_node(5)
RODS.fix_node(9)

elementConnect = [
	[1, 2, 1],
	[2, 3, 1],
	[3, 4, 1],
	[5, 6, 1],
	[6, 7, 1],
	[7, 8, 1],
	[9, 10, 1],
	[10, 11, 1],
	[11, 12, 1],
	[2, 6, 2],
	[3, 7, 2],
	[4, 8, 2],
	[6, 10, 2],
	[7, 11, 2],
	[8, 12, 2]
	]

i = 0
for ni, nj, st in elementConnect:
    if st == 1:
        RODS.add_frame_elastic_2d(i+1, ni, nj, EA_c, EI_c)
    else:
        RODS.add_frame_elastic_2d(i+1, ni, nj, EA_b, EI_b)
    i += 1

RODS.assemble_matrix()
RODS.solve_eigen()
RODS.print_info()

gmsh_file = "frame2d.msh".encode()
RODS.export_gmsh(gmsh_file)

# RODS.export_modal_gmsh(gmsh_file, 1)
# RODS.export_modal_gmsh(gmsh_file, 2)
# RODS.export_modal_gmsh(gmsh_file, 3)

RODS.set_response_gmsh(gmsh_file, 10)

# 添加地震波
dt = 0.005
wave_id = 0
wave_file = 'EQ-S-1.txt'.encode() # 文件名为 bytes 类型 
RODS.add_wave(wave_id, dt, wave_file)

# 添加自由度和单元响应记录器
dof_recorder_id = 0
RODS.add_dof_recorder(dof_recorder_id, RODS.Response.DISP.value, 'top_disp_frame.txt'.encode())
# 将节点4的X方向自由度加入记录器
node_id = 4
RODS.add_dof_to_recorder(3*(node_id-1)+1, dof_recorder_id)

# 设置动力分析方法
RODS.set_dynamic_solver(RODS.DynamicSolver.Newmark.value)
# 设置地震激励
scaling_factor = 700
RODS.active_ground_motion(RODS.Direction.X.value, wave_id, scaling_factor)
# 地震响应分析
n_sub_step = 1
RODS.solve_seismic_response(n_sub_step)

# 读取位移响应结果并绘制曲线
data_d = np.loadtxt('top_disp_frame.txt')
t = data_d[:,0]
d = data_d[:,1]
plt.figure("结构位移响应时程曲线", (12,4))
plt.plot(t, d)
plt.xlabel("时间")
plt.ylabel("位移")
plt.grid()
plt.show()