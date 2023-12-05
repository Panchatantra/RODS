import RODS

# 结构基本参数
mass = 0.01
E = 206.0
A = 2148.84937505542
EA = E*A

nodeCoord = [
		[-4500 , 0],
		[-1500 , 0],
		[1500  , 0],
		[4500  , 0],
		[-3000 , 3000],
		[0     , 3000],
		[3000  , 3000]
	]

i = 0
for x, z in nodeCoord:
    RODS.add_dof(2*i+1, RODS.Direction.X.value, mass)
    RODS.add_dof(2*i+2, RODS.Direction.Z.value, mass)
    RODS.add_node_2d(i+1, x, z, 2*i+1, 2*i+2, -1)
    i += 1

RODS.fix_node(1)
RODS.fix_node(4)
# RODS.fix_node_dof(4,RODS.Direction.Z.value)

elementConnect = [
		[1, 2],
		[2, 3],
		[3, 4],
		[5, 6],
		[6, 7],
		[1, 5],
		[2, 6],
		[3, 7],
		[2, 5],
		[3, 6],
		[4, 7]
	]

i = 0
for ni, nj in elementConnect:
    RODS.add_truss_elastic_2d(i+1, ni, nj, EA)
    i += 1

RODS.assemble_matrix()
RODS.solve_eigen()
RODS.print_info(10)

gmsh_file = b"truss2d.msh"
RODS.export_gmsh(gmsh_file)
RODS.export_modal_gmsh(gmsh_file, 1)
RODS.export_modal_gmsh(gmsh_file, 2)
RODS.export_modal_gmsh(gmsh_file, 3)