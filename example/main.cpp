// This file contains the 'main' function. Program execution begins and ends there.

#include <math.h>
#include "RODS.h"
#include "DOF.h"

void example_truss_RODS()
{
	double mass = 0.01;
	double E = 206.0;
	double A = 2148.84937505542;
	double EA = E*A;

	const int nnd = 7;
	const int ne = 11;

	double nodeCoord[nnd][2]{
		{-4500 , 0},
	{-1500 , 0},
	{1500  , 0},
	{4500  , 0},
	{-3000 , 3000},
	{0     , 3000},
	{3000  , 3000}
	};

	double x = 0, z = 0;

	for (int i = 0; i < nnd; i++)
	{
		add_dof(2 * i + 1, (int)RODS::Direction::X, mass);
		add_dof(2 * i + 2, (int)RODS::Direction::Z, mass);

		x = nodeCoord[i][0];
		z = nodeCoord[i][1];
		add_node_2d(i + 1, x, z, 2 * i + 1, 2 * i + 2, -1);
	}

	fix_node(1);
	fix_node(4);

	int elementConnect[ne][2]{
		{1, 2},
	{2, 3},
	{3, 4},
	{5, 6},
	{6, 7},
	{1, 5},
	{2, 6},
	{3, 7},
	{2, 5},
	{3, 6},
	{4, 7}
	};

	int ni = 0, nj = 0;
	for (int i = 0; i < ne; i++)
	{
		ni = elementConnect[i][0];
		nj = elementConnect[i][1];
		add_truss_elastic_2d(i + 1, ni, nj, EA);
	}

	assemble_matrix();
	solve_eigen();

	char gmshFile[] = "truss.msh";
	export_gmsh(gmshFile);

	print_info();
}

void example_wall_RODS()
{
	double PI = 3.14159265;
	set_damping_ratio(0.05);
	set_rayleigh_damping(2.0*PI/0.28, 2.0*PI/0.07);

	double mass = 0.05;
	double E = 32.5;
	double nu = 0.2;
	double t = 250.0;

	int nnd = 28;
	int ne = 18;

	double crds[28][2] {{-1500, 0},
	{-500, 0},
	{-500, 1000},
	{-1500, 1000},
	{-500, 2000},
	{-1500, 2000},
	{-500, 3000},
	{-1500, 3000},
	{-500, 4000},
	{-1500, 4000},
	{-500, 5000},
	{-1500, 5000},
	{-500, 6000},
	{-1500, 6000},
	{500, 0},
	{500, 1000},
	{500, 2000},
	{500, 3000},
	{500, 4000},
	{500, 5000},
	{500, 6000},
	{1500, 0},
	{1500, 1000},
	{1500, 2000},
	{1500, 3000},
	{1500, 4000},
	{1500, 5000},
	{1500, 6000}
	};

	double x = 0.0, z = 0.0;
	for (auto i = 0; i < nnd; i++)
	{
		add_dof(2 * i + 1, (int)RODS::Direction::X, mass);
		add_dof(2 * i + 2, (int)RODS::Direction::Z, mass);

		x = crds[i][0];
		z = crds[i][1];
		add_node_2d(i + 1, x, z, 2 * i + 1, 2 * i + 2, -1);
	}

	fix_node(1 );
	fix_node(2 );
	fix_node(15);
	fix_node(22);

	int cn[18][4] {{1, 2, 3, 4},
	{4, 3, 5, 6},
	{6, 5, 7, 8},
	{8, 7, 9, 10},
	{10, 9, 11, 12},
	{12, 11, 13, 14},
	{2, 15, 16, 3},
	{3, 16, 17, 5},
	{5, 17, 18, 7},
	{7, 18, 19, 9},
	{9, 19, 20, 11},
	{11, 20, 21, 13},
	{15, 22, 23, 16},
	{16, 23, 24, 17},
	{17, 24, 25, 18},
	{18, 25, 26, 19},
	{19, 26, 27, 20},
	{20, 27, 28, 21}};

	int nodeI = 0, nodeJ = 0, nodeP = 0, nodeQ = 0;
	for (int i = 0; i < ne; i++)
	{
		nodeI = cn[i][0];
		nodeJ = cn[i][1];
		nodeP = cn[i][2];
		nodeQ = cn[i][3];
		add_quad4_elastic(i+1, nodeI, nodeJ, nodeP, nodeQ, E, nu, t);
	}

	assemble_matrix();

	solve_eigen();
	print_info();

	//char gmshFile[] = "data/wall.msh";
	//ds->exportGmsh(gmshFile);
	//ds->exportModalGmsh(gmshFile, 1);
	//ds->exportModalGmsh(gmshFile, 2);
	//ds->exportModalGmsh(gmshFile, 3);

	//ds->setResponseGmsh(gmshFile, 10);

	//int eqId = 1;
	//double dt = 0.005;
	//char eqFile[] = "data/EQ-S-1.txt";
	//ds->addWave(eqId, dt, eqFile);

	//ds->setDynamicSolver(RODS::DynamicSolver::Newmark);
	//ds->activeGroundMotion(RODS::Direction::X, eqId, 700.0);
	//ds->solveSeismicResponse(1);
}

int main()
{
	//example_truss_RODS();
	example_wall_RODS();
	return 0;
}