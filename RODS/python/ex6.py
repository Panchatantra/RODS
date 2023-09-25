# 导入 RODS 模块
import RODS
# 导入 numpy 和 matplotlib 模块（用于绘制曲线）
import numpy as np
import matplotlib.pyplot as plt
plt.rcParams['font.sans-serif'] = ['Microsoft Yahei']  # 用来正常显示中文标签
plt.rcParams['axes.unicode_minus'] = False  # 用来正常显示负号

# 设置模型名
RODS.set_name(b"MDOF")

# 设置固有阻尼比
zeta_0 = 0.02
RODS.set_damping_ratio(zeta_0)

# 固有阻尼可用两种方式实现（二者选一）：
# 采用 Rayleigh 阻尼，设置 Rayleigh 阻尼的两个计算圆频率
RODS.set_rayleigh_damping(6, 18)
# 按模态阻尼比计算固有阻尼矩阵，考虑 1 阶模态
# RODS.set_num_modes_inherent_damping(1)

m = 1.0 # 质量
k = m*1600.0 # 刚度

fixed_dof_id = 0
RODS.add_dof_x(fixed_dof_id, m)
RODS.fix_dof(fixed_dof_id)

n = 6

# 添加 X 方向自由度
for id in range(1, n+1):
    RODS.add_dof_x(id, m)

# 添加弹簧
for id in range(1, n+1):
    RODS.add_spring(id, id-1, id, k)

# 组装结构矩阵
RODS.assemble_matrix()

# 模态分析
RODS.solve_eigen()

# 打印基本信息和模态分析结果
RODS.print_info()

P = RODS.get_period()

omg_1 = 2.0*3.1416/P[0]

zeta = 0.02 # 附加阻尼比
c = 2.0*zeta*k/omg_1 # 附加阻尼系数

# 添加阻尼器
for id in range(1, n+1):
    RODS.add_dashpot(id+100, id-1, id, c)

print("=================================================")
print("MDOF Structure Information")
print("inherent damping ratio: ", zeta_0)
print("number of free DOF: ", n)
print("mass: ", m)
print("stiffness: ", k)
print("damping coefficient: ", c)
print("=================================================")

RODS.assemble_matrix()

# 添加地震波
dt = 0.005
wave_id = 0
wave_file = b'EQ-S-1.txt' # 文件名为 bytes 类型
RODS.add_wave(wave_id, dt, wave_file)

# 添加自由度和单元响应记录器
dof_recorder_id = 1
RODS.add_dof_recorder(dof_recorder_id, RODS.Response.DISP.value, b'mdof_disp.txt')
for id in range(1, n+1):
    RODS.add_dof_to_recorder(id, dof_recorder_id)

ele_recorder_id = 1
RODS.add_ele_recorder(dof_recorder_id, RODS.Response.ALL.value, b'mdof_ele_force_def.txt')
for id in range(1, n+1):
    RODS.add_ele_to_recorder(id, ele_recorder_id)
for id in range(1, n+1):
    RODS.add_ele_to_recorder(id+100, ele_recorder_id)

# 设置动力分析方法
RODS.set_dynamic_solver(RODS.DynamicSolver.Newmark.value)
# 设置地震激励
scaling_factor = 1.0
RODS.active_ground_motion(RODS.Direction.X.value, wave_id, scaling_factor)
# 地震响应分析
n_sub_step = 1
RODS.solve_seismic_response(n_sub_step)

# 读取位移响应结果并绘制曲线
data_d = np.loadtxt('mdof_disp.txt')
t = data_d[:,0]
plt.figure("结构位移响应时程曲线", (12,4))
for id in range(1, n+1):
    d = data_d[:,id]
    plt.plot(t, d, label="自由度 %d"%id)
plt.xlabel("时间")
plt.ylabel("位移")
plt.grid()
plt.legend()

# 读取力响应结果并绘制曲线
data_f = np.loadtxt('mdof_ele_force_def.txt')
t = data_f[:,0]
plt.figure("阻尼器滞回曲线", (6,6))
for id in range(1, n+1):
    d = data_f[:,2*n+id*2-1]
    f = data_f[:,2*n+id*2]
    plt.plot(d, f, label="阻尼器 %d"%(id+100))
plt.xlabel("位移")
plt.ylabel("力")
plt.grid()
plt.legend()

plt.show()