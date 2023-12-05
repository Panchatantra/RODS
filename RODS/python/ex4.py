import RODS
import numpy as np
import matplotlib.pyplot as plt
plt.rcParams['font.sans-serif'] = ['Microsoft Yahei']  # 用来正常显示中文标签
plt.rcParams['axes.unicode_minus'] = False  # 用来正常显示负号

# 结构基本参数
mass = 0.05
E = 32.5
nu = 0.2
t = 250.0

# 设置固有阻尼比
RODS.set_damping_ratio(0.05)

# 采用 Rayleigh 阻尼，设置 Rayleigh 阻尼的两个计算圆频率
PI = 3.14159265
RODS.set_rayleigh_damping(2.0*PI/0.28, 2.0*PI/0.07)

nodeCoord = [
    [-1500, 0],
    [-500, 0],
    [-500, 1000],
    [-1500, 1000],
    [-500, 2000],
    [-1500, 2000],
    [-500, 3000],
    [-1500, 3000],
    [-500, 4000],
    [-1500, 4000],
    [-500, 5000],
    [-1500, 5000],
    [-500, 6000],
    [-1500, 6000],
    [500, 0],
    [500, 1000],
    [500, 2000],
    [500, 3000],
    [500, 4000],
    [500, 5000],
    [500, 6000],
    [1500, 0],
    [1500, 1000],
    [1500, 2000],
    [1500, 3000],
    [1500, 4000],
    [1500, 5000],
    [1500, 6000]
]

# 添加自由度和节点
i = 0
for x, z in nodeCoord:
    RODS.add_dof(2*i+1, RODS.Direction.X.value, mass)
    RODS.add_dof(2*i+2, RODS.Direction.Z.value, mass)
    RODS.add_node_2d(i+1, x, z, 2*i+1, 2*i+2, -1)
    i += 1
    
# 固定底部4个节点
for node_id in [1,2,15,22]:
    RODS.fix_node(node_id)

elementConnect = [
	[1, 2, 3, 4],
	[4, 3, 5, 6],
	[6, 5, 7, 8],
	[8, 7, 9, 10],
	[10, 9, 11, 12],
	[12, 11, 13, 14],
	[2, 15, 16, 3],
	[3, 16, 17, 5],
	[5, 17, 18, 7],
	[7, 18, 19, 9],
	[9, 19, 20, 11],
	[11, 20, 21, 13],
	[15, 22, 23, 16],
	[16, 23, 24, 17],
	[17, 24, 25, 18],
	[18, 25, 26, 19],
	[19, 26, 27, 20],
	[20, 27, 28, 21]
	]

# 添加四节点平面应力单元
i = 0
for nI, nJ, nP, nQ in elementConnect:
    RODS.add_quad4_elastic(i+1, nI, nJ, nP, nQ, E, nu, t)
    i += 1

RODS.assemble_matrix()
RODS.solve_eigen()
RODS.print_info(48)

gmsh_file = "wall2d.msh".encode()
RODS.export_gmsh(gmsh_file)

RODS.export_modal_gmsh(gmsh_file, 1)
RODS.export_modal_gmsh(gmsh_file, 2)
RODS.export_modal_gmsh(gmsh_file, 3)

# 设置每隔10个分析步记录一次节点位移到gmsh文件
RODS.set_response_gmsh(gmsh_file, 10)

# 添加地震波
dt = 0.005
wave_id = 0
wave_file = 'EQ-S-1.txt'.encode() # 文件名为 bytes 类型 
RODS.add_wave(wave_id, dt, wave_file)

# 添加响应记录器
dof_recorder_id = 0
dof_recorder_file = 'top_disp_wall.txt'
RODS.add_dof_recorder(dof_recorder_id, RODS.Response.DISP.value, dof_recorder_file.encode())
# 将节点21的X方向自由度加入记录器
node_id = 21
RODS.add_dof_to_recorder(2*(node_id-1)+1, dof_recorder_id)

# 设置动力分析方法
RODS.set_dynamic_solver(RODS.DynamicSolver.Newmark.value)
# 设置地震激励
scaling_factor = 700
RODS.active_ground_motion(RODS.Direction.X.value, wave_id, scaling_factor)
# 地震响应分析
n_sub_step = 1
RODS.solve_seismic_response(n_sub_step)

# 读取位移响应结果并绘制曲线
data_d = np.loadtxt(dof_recorder_file)
t = data_d[:,0]
d = data_d[:,1]
plt.figure("结构位移响应时程曲线", (12,4))
plt.plot(t, d)
plt.xlim((0, t[-1]))
plt.xlabel("时间")
plt.ylabel("位移")
plt.grid()
plt.show()