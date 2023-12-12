set_name("SDOF_BL")

-- 设置固有阻尼比
set_damping_ratio(0.02)

-- 固有阻尼可用两种方式实现（二者选一）：
-- 采用 Rayleigh 阻尼，设置 Rayleigh 阻尼的两个计算圆频率
set_rayleigh_damping(0.1, 0.05)
-- 按模态阻尼比计算固有阻尼矩阵，考虑 1 阶模态
-- set_num_modes_inherent_damping(1)

-- 质量
m = 1.0
-- 添加 X 方向自由度
fixed_dof_id = 0
free_dof_id = 1
add_dof_x(fixed_dof_id, m)
add_dof_x(free_dof_id, m)
-- 固定自由度
fix_dof(fixed_dof_id)

-- 刚度
k = m * 100.0
-- 添加弹簧
spring_id = 0
-- add_spring(spring_id, fixed_dof_id, free_dof_id, k)
uy = 0.01
alpha = 0.01
add_spring_bilinear(spring_id, fixed_dof_id, free_dof_id, k, uy, alpha)

-- 附加阻尼比
zeta = 0.02
-- 附加阻尼系数
c = 2.0 * zeta * math.sqrt(m * k)
-- 添加阻尼器
dashpot_id = 1
add_dashpot(dashpot_id, fixed_dof_id, free_dof_id, c)

-- 组装结构矩阵
assemble_matrix()

-- 模态分析
solve_eigen()

-- 打印基本信息和模态分析结果
print_info(1)

-- 添加地震波
dt = 0.005
wave_id = 0
wave_file = 'EQ-S-1.txt'
add_wave(wave_id, dt, wave_file)

-- 添加自由度和单元响应记录器
dof_recorder_id = 0
add_dof_recorder(dof_recorder_id, DISP, 'sdof_bl_disp.txt')
add_dof_to_recorder(free_dof_id, dof_recorder_id)
ele_recorder_id = 0
add_ele_recorder(ele_recorder_id, FORCE, 'sdof_bl_force.txt')
add_ele_to_recorder(spring_id, ele_recorder_id)
add_ele_to_recorder(dashpot_id, ele_recorder_id)

-- 设置动力分析方法
set_dynamic_solver(Newmark_NL)
-- 设置地震激励
scaling_factor = 1.0
active_ground_motion(X, wave_id, scaling_factor)
-- 地震响应分析
n_sub_step = 1
solve_seismic_response(n_sub_step)

save_to_json("SDOF_BL.json")
