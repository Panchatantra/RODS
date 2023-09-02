# 导入 RODS 模块
import RODS
# 导入 numpy 和 matplotlib 模块（用于绘制曲线）
import numpy as np
import matplotlib.pyplot as plt
plt.rcParams['font.sans-serif'] = ['Microsoft Yahei']  # 用来正常显示中文标签
plt.rcParams['axes.unicode_minus'] = False  # 用来正常显示负号

# 设置固有阻尼比
RODS.set_damping_ratio(0.02)

# 固有阻尼可用两种方式实现（二者选一）：
# 采用 Rayleigh 阻尼，设置 Rayleigh 阻尼的两个计算圆频率
# RODS.set_rayleigh_damping(9.04, 11.35)
# 按模态阻尼比计算固有阻尼矩阵，考虑 1 阶模态
RODS.set_num_modes_inherent_damping(2)

# 结构质量
m = 1.0
# 添加 X 方向自由度
fixed_dof_id = 0
free_dof_id = 1
RODS.add_dof_x(fixed_dof_id, m)
RODS.add_dof_x(free_dof_id, m)
# 固定自由度
RODS.fix_dof(fixed_dof_id)

# 结构刚度
k = m*100.0
# 添加结构弹簧
spring_id = 0
RODS.add_spring(spring_id, fixed_dof_id, free_dof_id, k)

# 添加SPIS2
spis2_id = 10
mu = 0.05
kp = mu / (1.0 - mu)
xi = mu / 2.0*np.sqrt(kp / 2.0)
m_in = mu * m
k_s = kp * k
c_d = 2.0*xi*np.sqrt(m*k)

inner_dof_id = 10
RODS.add_dof_x(inner_dof_id, 0.0)
RODS.add_spis2(spis2_id, fixed_dof_id, free_dof_id, inner_dof_id, m_in, c_d, k_s)

RODS.print_info()

# 组装结构矩阵
RODS.assemble_matrix()

# 模态分析
RODS.solve_eigen()

# 打印基本信息和模态分析结果
RODS.print_info()

# 添加地震波
dt = 0.005
wave_id = 0
wave_file = 'EQ-S-1.txt'.encode() # 文件名为 bytes 类型
RODS.add_wave(wave_id, dt, wave_file)

# 添加自由度和单元响应记录器
dof_recorder_id = 0
RODS.add_dof_recorder(dof_recorder_id, RODS.Response.DISP.value, 'd.txt'.encode())
RODS.add_dof_to_recorder(free_dof_id, dof_recorder_id)
ele_recorder_id = 0
RODS.add_ele_recorder(dof_recorder_id, RODS.Response.FORCE.value, 'f.txt'.encode())
RODS.add_ele_to_recorder(spring_id, ele_recorder_id)
RODS.add_ele_to_recorder(spis2_id, ele_recorder_id)

# 设置动力分析方法
RODS.set_dynamic_solver(RODS.DynamicSolver.Newmark_NL.value)
# 设置地震激励
scaling_factor = 1.0
RODS.active_ground_motion(RODS.Direction.X.value, wave_id, scaling_factor)
# 地震响应分析
n_sub_step = 1
RODS.solve_seismic_response(n_sub_step)

# 读取位移响应结果并绘制曲线
data_d = np.loadtxt('d.txt')
t = data_d[:,0]
d = data_d[:,1]
plt.figure("结构位移响应时程曲线", (12,4))
plt.plot(t, d)
plt.xlabel("时间")
plt.ylabel("位移")
plt.grid()

# 读取力响应结果并绘制曲线
data_f = np.loadtxt('f.txt')
t = data_f[:,0]
f = data_f[:,2]
plt.figure("阻尼器滞回曲线", (6,6))
plt.plot(d, f)
plt.xlabel("位移")
plt.ylabel("力")
plt.grid()

plt.show()