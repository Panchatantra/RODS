using Libdl

if Sys.iswindows()
    RODS = Libdl.dlopen("./RODS.dll")
elseif Sys.islinux()
    RODS = Libdl.dlopen("librods.so")
end

@enum Response begin
    DISP = 0
    VEL = 1
    ACC = 2
    FORCE = 3
    DEF = 4
    ALL = 5
end

@enum DynamicSolver begin
    Newmark = 0
    Newmark_NL = 1
    StateSpace = 2
    StateSpace_NL = 3
end

@enum Direction begin
    X = 0
    Y = 1
    Z = 2
    RX = 3
    RY = 4
    RZ = 5
end

set_name_ = Libdl.dlsym(RODS, :set_name)
print_info_ = Libdl.dlsym(RODS, :print_info)
set_damping_ratio_ = Libdl.dlsym(RODS, :set_damping_ratio)
set_rayleigh_damping_ = Libdl.dlsym(RODS, :set_rayleigh_damping)
add_dof_x_ = Libdl.dlsym(RODS, :add_dof_x)
fix_dof_ = Libdl.dlsym(RODS, :fix_dof)
add_spring_ = Libdl.dlsym(RODS, :add_spring)
add_dashpot_ = Libdl.dlsym(RODS, :add_dashpot)
assemble_matrix_ = Libdl.dlsym(RODS, :assemble_matrix)
solve_eigen_ = Libdl.dlsym(RODS, :solve_eigen)
add_wave_ = Libdl.dlsym(RODS, :add_wave)
add_dof_recorder_ = Libdl.dlsym(RODS, :add_dof_recorder)
add_dof_to_recorder_ = Libdl.dlsym(RODS, :add_dof_to_recorder)
add_ele_recorder_ = Libdl.dlsym(RODS, :add_ele_recorder)
add_ele_to_recorder_ = Libdl.dlsym(RODS, :add_ele_to_recorder)
set_dynamic_solver_ = Libdl.dlsym(RODS, :set_dynamic_solver)
active_ground_motion_ = Libdl.dlsym(RODS, :active_ground_motion)
solve_seismic_response_ = Libdl.dlsym(RODS, :solve_seismic_response)

function set_name(name::AbstractString)
    @ccall $set_name_(name::Cstring)::Cvoid
end

function print_info(num_modes::Int)
    @ccall $print_info_(num_modes::Cint)::Cvoid
end

function set_damping_ratio(damping_ratio::Float64)
    @ccall $set_damping_ratio_(damping_ratio::Cdouble)::Cvoid
end

function set_rayleigh_damping(omg1::Float64, omg2::Float64)
    @ccall $set_rayleigh_damping_(omg1::Cdouble, omg2::Cdouble)::Cvoid
end

function add_dof_x(id::Int, m::Float64)
    @ccall $add_dof_x_(id::Cint, m::Cdouble)::Csize_t
end

function fix_dof(id::Int)
    @ccall $fix_dof_(id::Cint)::Cvoid
end

function add_spring(id::Int, i::Int, j::Int, k::Float64)
    @ccall $add_spring_(id::Cint, i::Cint, j::Cint, k::Cdouble)::Csize_t
end

function add_dashpot(id::Int, i::Int, j::Int, c::Float64)
    @ccall $add_dashpot_(id::Cint, i::Cint, j::Cint, c::Cdouble)::Csize_t
end

function assemble_matrix()
    @ccall $assemble_matrix_()::Cvoid
end

function solve_eigen()
    @ccall $solve_eigen_()::Cvoid
end

function add_wave(id::Int, dt::Float64, fileName::AbstractString)
    @ccall $add_wave_(id::Cint, dt::Cdouble, fileName::Cstring)::Csize_t
end

function add_dof_recorder(id::Int, responseType::Response, fileName::AbstractString)
    @ccall $add_dof_recorder_(id::Cint, Int(responseType)::Cint, fileName::Cstring)::Csize_t
end

function add_dof_to_recorder(dofId::Int, recorderId::Int)
    @ccall $add_dof_to_recorder_(dofId::Cint, recorderId::Cint)::Csize_t
end

function add_ele_recorder(id::Int, responseType::Response, fileName::AbstractString)
    @ccall $add_ele_recorder_(id::Cint, Int(responseType)::Cint, fileName::Cstring)::Csize_t
end

function add_ele_to_recorder(eleId::Int, recorderId::Int)
    @ccall $add_ele_to_recorder_(eleId::Cint, recorderId::Cint)::Csize_t
end

function set_dynamic_solver(solver::DynamicSolver)
    @ccall $set_dynamic_solver_(Int(solver)::Cint)::Cvoid
end

function active_ground_motion(dir::Direction, waveId::Int, waveScale::Float64)
    @ccall $active_ground_motion_(Int(dir)::Cint, waveId::Cint, waveScale::Cdouble)::Cvoid
end

function solve_seismic_response(nsub::Int=1)
    @ccall $solve_seismic_response_(nsub::Cint)::Cvoid
end