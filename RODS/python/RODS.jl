using Libdl

if Sys.iswindows()
    RODS = Libdl.dlopen("./RODS.dll")
elseif Sys.islinux()
    RODS = Libdl.dlopen("librods.so")
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