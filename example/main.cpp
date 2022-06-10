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

int main()
{
	example_truss_RODS();
	return 0;
}