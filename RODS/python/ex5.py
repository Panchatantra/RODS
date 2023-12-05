import RODS
import numpy as np
import matplotlib.pyplot as plt
plt.rcParams['font.sans-serif'] = ['Microsoft Yahei']  # 用来正常显示中文标签
plt.rcParams['axes.unicode_minus'] = False  # 用来正常显示负号

# 定义基本参数
mass = 0.005
E = 32.5
G = 13.5416666666667
A_b = 80000.0
A_c = 160000.0
I_by = 266666666.666667
I_cy = 2133333333.33333
I_bz = 1066666666.66667
I_cz = 2133333333.33333
Ip_b = 732416666.666667
Ip_c = 3605333333.33333
EA_b = E * A_b
EA_c = E * A_c
EI_by = E * I_by
EI_cy = E * I_cy
EI_bz = E * I_bz
EI_cz = E * I_cz
GIp_c = G*Ip_c
GIp_b = G*Ip_b

# 设置固有阻尼比
RODS.set_damping_ratio(0.05)

# 采用 Rayleigh 阻尼，设置 Rayleigh 阻尼的两个计算圆频率
PI = 3.14159265
RODS.set_rayleigh_damping(2.0*PI/0.36, 2.0*PI/0.1)

nodeCoord = [
    [-6000,0,0],
    [0,0,0],
    [6000,0,0],
    [-6000,0,3000],
    [0,0,3000],
    [6000,0,3000],
    [-6000,0,6000],
    [0,0,6000],
    [6000,0,6000],
    [-6000,0,9000],
    [0,0,9000],
    [6000,0,9000],
    [-6000,6000,0],
    [0,6000,0],
    [6000,6000,0],
    [-6000,6000,3000],
    [0,6000,3000],
    [6000,6000,3000],
    [-6000,6000,6000],
    [0,6000,6000],
    [6000,6000,6000],
    [-6000,6000,9000],
    [0,6000,9000],
    [6000,6000,9000],
    [-6000,12000,0],
    [0,12000,0],
    [6000,12000,0],
    [-6000,12000,3000],
    [0,12000,3000],
    [6000,12000,3000],
    [-6000,12000,6000],
    [0,12000,6000],
    [6000,12000,6000],
    [-6000,12000,9000],
    [0,12000,9000],
    [6000,12000,9000]
	]

# 添加自由度和节点
i = 0
for x, y, z in nodeCoord:
    for dir in RODS.Direction:
        dir_value = dir.value
        RODS.add_dof(6*i+dir_value+1, dir_value, mass)
    RODS.add_node_3d(i+1, x, y, z, 6*i+1, 6*i+2, 6*i+3, 6*i+4, 6*i+5, 6*i+6)
    i += 1

# 固定底部节点
for node_id in [1, 2, 3, 13, 14, 15, 25, 26, 27]:
    RODS.fix_node(node_id)

elementConnect = [
	[1, 4, 1],
    [2, 5, 1],
    [3, 6, 1],
    [4, 5, 2],
    [5, 6, 2],
    [4, 7, 1],
    [5, 8, 1],
    [6, 9, 1],
    [7, 8, 2],
    [8, 9, 2],
    [7, 10, 1],
    [8, 11, 1],
    [9, 12, 1],
    [10, 11, 2],
    [11, 12, 2],
    [4, 16, 2],
    [5, 17, 2],
    [6, 18, 2],
    [7, 19, 2],
    [8, 20, 2],
    [9, 21, 2],
    [10, 22, 2],
    [11, 23, 2],
    [12, 24, 2],
    [13, 16, 1],
    [14, 17, 1],
    [15, 18, 1],
    [16, 17, 2],
    [17, 18, 2],
    [16, 19, 1],
    [17, 20, 1],
    [18, 21, 1],
    [19, 20, 2],
    [20, 21, 2],
    [19, 22, 1],
    [20, 23, 1],
    [21, 24, 1],
    [22, 23, 2],
    [23, 24, 2],
    [16, 28, 2],
    [17, 29, 2],
    [18, 30, 2],
    [19, 31, 2],
    [20, 32, 2],
    [21, 33, 2],
    [22, 34, 2],
    [23, 35, 2],
    [24, 36, 2],
    [25, 28, 1],
    [26, 29, 1],
    [27, 30, 1],
    [28, 29, 2],
    [29, 30, 2],
    [28, 31, 1],
    [29, 32, 1],
    [30, 33, 1],
    [31, 32, 2],
    [32, 33, 2],
    [31, 34, 1],
    [32, 35, 1],
    [33, 36, 1],
    [34, 35, 2],
    [35, 36, 2],
	]

# 添加三维框架单元
i = 0
for ni, nj, st in elementConnect:
    if st == 1:
        RODS.add_frame_elastic_3d(i+1, ni, nj, EA_c, EI_cy, EI_cz, GIp_c)
    else:
        RODS.add_frame_elastic_3d(i+1, ni, nj, EA_b, EI_by, EI_bz, GIp_b)
    i += 1

RODS.assemble_matrix()
RODS.solve_eigen()
RODS.print_info(12)

gmsh_file = "frame3d.msh".encode()
RODS.export_gmsh(gmsh_file)

RODS.export_modal_gmsh(gmsh_file, 1)
RODS.export_modal_gmsh(gmsh_file, 2)
RODS.export_modal_gmsh(gmsh_file, 3)

RODS.set_response_gmsh(gmsh_file, 10)

# 添加地震波
dt = 0.005
wave_id = 0
wave_file = 'EQ-S-1.txt'.encode() # 文件名为 bytes 类型 
RODS.add_wave(wave_id, dt, wave_file)

# 添加自由度响应记录器
dof_recorder_id = 0
RODS.add_dof_recorder(dof_recorder_id, RODS.Response.DISP.value, 'top_disp_frame_3d.txt'.encode())
# 将节点23的X方向自由度加入记录器
node_id = 23
# RODS.add_dof_to_recorder(6*(node_id-1)+1, dof_recorder_id)
RODS.add_node_dof_to_recorder(node_id, RODS.Direction.X.value, dof_recorder_id)

# 设置动力分析方法
RODS.set_dynamic_solver(RODS.DynamicSolver.Newmark.value)
# 设置地震激励
scaling_factor = 700
RODS.active_ground_motion(RODS.Direction.X.value, wave_id, scaling_factor)
# 地震响应分析
n_sub_step = 1
RODS.solve_seismic_response(n_sub_step)

# 读取位移响应结果并绘制曲线
data_sap = np.loadtxt('top_disp_frame_3d_sap.txt')
t_sap = data_sap[:,0]
d_sap = data_sap[:,1]
data = np.loadtxt('top_disp_frame_3d.txt')
t = data[:,0]
d = data[:,1]
plt.figure("结构位移响应时程曲线", (12,4))
plt.plot(t_sap, d_sap, label="SAP2000")
plt.plot(t, d, label="RODS", ls=":")
plt.xlabel("时间")
plt.ylabel("位移")
plt.grid()
plt.legend()
plt.show()