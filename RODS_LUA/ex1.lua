set_name("SDOF")
set_damping_ratio(0.02)
set_rayleigh_damping(10.0, 20.0)

m = 1.0
fixed_dof_id = 0
free_dof_id = 1

add_dof_x(fixed_dof_id, m)
add_dof_x(free_dof_id, m)

fix_dof(fixed_dof_id)

k = m*2500.0
spring_id = 0
add_spring(spring_id, fixed_dof_id, free_dof_id, k)

zeta = 0.02
c = 2.0*zeta*math.sqrt(m*k)
dashpot_id = 1
add_dashpot(dashpot_id, fixed_dof_id, free_dof_id, c)

neq = assemble_matrix()
print("Number of equations:", neq)
solve_eigen()
print_info(1)

dt = 0.005
wave_id = 0
wave_file = "EQ-S-1.txt"
add_wave(wave_id, dt, wave_file)

dof_recorder_id = 0
add_dof_recorder(dof_recorder_id, DISP, "sdof_disp.txt")
add_dof_to_recorder(free_dof_id, dof_recorder_id)
ele_recorder_id = 0
add_ele_recorder(dof_recorder_id, FORCE, "sdof_force.txt")
add_ele_to_recorder(spring_id, ele_recorder_id)
add_ele_to_recorder(dashpot_id, ele_recorder_id)

set_dynamic_solver(0)
scaling_factor = 1.0
active_ground_motion(0, wave_id, scaling_factor)
n_sub_step = 1
solve_seismic_response(n_sub_step)

save_to_json("SDOF.json")