import RODS
import RODSE
import numpy as np
import matplotlib.pyplot as plt
plt.rcParams['font.sans-serif'] = ['Microsoft Yahei']  # 用来正常显示中文标签
plt.rcParams['axes.unicode_minus'] = False  # 用来正常显示负号

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

nodeInfo, frameInfo, fixedNodes = RODSE.read_frame_from_sap2000("frame3d.xlsx")

for nodeId, x, y, z, mass in nodeInfo:
    i = nodeId - 1
    j = 1
    for dir in RODS.Direction:
        dir_value = dir.value
        RODS.add_dof(6*i+dir_value+1, dir.value, mass)
        j += 1
    RODS.add_node_3d(nodeId, x, y, z, 6*i+1, 6*i+2, 6*i+3, 6*i+4, 6*i+5, 6*i+6)

for node_id in fixedNodes:
    RODS.fix_node(node_id)

for frameId, ni, nj, st in frameInfo:
    if st == 'COL':
        RODS.add_frame_elastic_3d(frameId, ni, nj, EA_c, EI_cy, EI_cz, GIp_c)
    else:
        RODS.add_frame_elastic_3d(frameId, ni, nj, EA_b, EI_by, EI_bz, GIp_b)

RODS.assemble_matrix()
RODS.solve_eigen()
RODS.print_info()

gmsh_file = "frame3d.msh".encode()
RODS.export_gmsh(gmsh_file)

RODS.export_modal_gmsh(gmsh_file, 1)
RODS.export_modal_gmsh(gmsh_file, 2)
RODS.export_modal_gmsh(gmsh_file, 3)

# RODS.set_response_gmsh(gmsh_file, 10)

# # 添加地震波
# dt = 0.005
# wave_id = 0
# wave_file = 'EQ-S-1.txt'.encode() # 文件名为 bytes 类型 
# RODS.add_wave(wave_id, dt, wave_file)

# # 添加自由度和单元响应记录器
# dof_recorder_id = 0
# RODS.add_dof_recorder(dof_recorder_id, RODS.Response.DISP.value, 'top_disp_frame.txt'.encode())
# # 将节点4的X方向自由度加入记录器
# node_id = 4
# RODS.add_dof_to_recorder(3*(node_id-1)+1, dof_recorder_id)

# # 设置动力分析方法
# RODS.set_dynamic_solver(RODS.DynamicSolver.Newmark.value)
# # 设置地震激励
# scaling_factor = 700
# RODS.active_ground_motion(RODS.Direction.X.value, wave_id, scaling_factor)
# # 地震响应分析
# n_sub_step = 1
# RODS.solve_seismic_response(n_sub_step)

# # 读取位移响应结果并绘制曲线
# data_d = np.loadtxt('top_disp_frame.txt')
# t = data_d[:,0]
# d = data_d[:,1]
# plt.figure("结构位移响应时程曲线", (12,4))
# plt.plot(t, d)
# plt.xlabel("时间")
# plt.ylabel("位移")
# plt.grid()
# plt.show()