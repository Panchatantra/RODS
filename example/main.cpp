// This file contains the 'main' function. Program execution begins and ends there.

#include <math.h>
#include "rods.h"

//void example_truss_rods()
//{
//	double mass = 0.01;
//	double E = 206.0;
//	double A = 2148.84937505542;
//	double EA = E*A;
//
//	const int nnd = 7;
//	const int ne = 11;
//
//	double nodeCoord[nnd][2]{
//		{-4500 , 0},
//		{-1500 , 0},
//		{1500  , 0},
//		{4500  , 0},
//		{-3000 , 3000},
//		{0     , 3000},
//		{3000  , 3000}
//	};
//
//	double x = 0, z = 0;
//
//	for (int i = 0; i < nnd; i++)
//	{
//		add_dof(2 * i + 1, X, mass);
//		add_dof(2 * i + 2, Z, mass);
//
//		x = nodeCoord[i][0];
//		z = nodeCoord[i][1];
//		add_node_2d(i + 1, x, z, 2 * i + 1, 2 * i + 2, -1);
//	}
//
//	fix_node(1);
//	fix_node(4);
//
//	int elementConnect[ne][2]{
//		{1, 2},
//		{2, 3},
//		{3, 4},
//		{5, 6},
//		{6, 7},
//		{1, 5},
//		{2, 6},
//		{3, 7},
//		{2, 5},
//		{3, 6},
//		{4, 7}
//	};
//
//	int ni = 0, nj = 0;
//	for (int i = 0; i < ne; i++)
//	{
//		ni = elementConnect[i][0];
//		nj = elementConnect[i][1];
//		add_truss_elastic_2d(i + 1, ni, nj, EA);
//	}
//
//	assemble_matrix();
//	solve_eigen();
//
//	char gmshFile[] = "truss.msh";
//	export_gmsh(gmshFile);
//
//	print_info();
//}

// void example_wall_rods()
// {
// 	double PI = 3.14159265;
// 	set_damping_ratio(0.05);
// 	set_rayleigh_damping(2.0*PI/0.28, 2.0*PI/0.07);

// 	double mass = 0.05;
// 	double E = 32.5;
// 	double nu = 0.2;
// 	double t = 250.0;

// 	int nnd = 28;
// 	int ne = 18;

// 	double crds[28][2] {{-1500, 0},
// 	{-500, 0},
// 	{-500, 1000},
// 	{-1500, 1000},
// 	{-500, 2000},
// 	{-1500, 2000},
// 	{-500, 3000},
// 	{-1500, 3000},
// 	{-500, 4000},
// 	{-1500, 4000},
// 	{-500, 5000},
// 	{-1500, 5000},
// 	{-500, 6000},
// 	{-1500, 6000},
// 	{500, 0},
// 	{500, 1000},
// 	{500, 2000},
// 	{500, 3000},
// 	{500, 4000},
// 	{500, 5000},
// 	{500, 6000},
// 	{1500, 0},
// 	{1500, 1000},
// 	{1500, 2000},
// 	{1500, 3000},
// 	{1500, 4000},
// 	{1500, 5000},
// 	{1500, 6000}
// 	};

// 	double x = 0.0, z = 0.0;
// 	for (auto i = 0; i < nnd; i++)
// 	{
// 		add_dof(2 * i + 1, X, mass);
// 		add_dof(2 * i + 2, Z, mass);

// 		x = crds[i][0];
// 		z = crds[i][1];
// 		add_node_2d(i + 1, x, z, 2 * i + 1, 2 * i + 2, -1);
// 	}

// 	fix_node(1 );
// 	fix_node(2 );
// 	fix_node(15);
// 	fix_node(22);

// 	int cn[18][4] {{1, 2, 3, 4},
// 	{4, 3, 5, 6},
// 	{6, 5, 7, 8},
// 	{8, 7, 9, 10},
// 	{10, 9, 11, 12},
// 	{12, 11, 13, 14},
// 	{2, 15, 16, 3},
// 	{3, 16, 17, 5},
// 	{5, 17, 18, 7},
// 	{7, 18, 19, 9},
// 	{9, 19, 20, 11},
// 	{11, 20, 21, 13},
// 	{15, 22, 23, 16},
// 	{16, 23, 24, 17},
// 	{17, 24, 25, 18},
// 	{18, 25, 26, 19},
// 	{19, 26, 27, 20},
// 	{20, 27, 28, 21}};

// 	int nodeI = 0, nodeJ = 0, nodeP = 0, nodeQ = 0;
// 	for (int i = 0; i < ne; i++)
// 	{
// 		nodeI = cn[i][0];
// 		nodeJ = cn[i][1];
// 		nodeP = cn[i][2];
// 		nodeQ = cn[i][3];
// 		add_quad4_elastic(i+1, nodeI, nodeJ, nodeP, nodeQ, E, nu, t);
// 	}

// 	assemble_matrix();

// 	solve_eigen();
// 	print_info();

// 	//char gmshFile[] = "data/wall.msh";
// 	//ds->exportGmsh(gmshFile);
// 	//ds->exportModalGmsh(gmshFile, 1);
// 	//ds->exportModalGmsh(gmshFile, 2);
// 	//ds->exportModalGmsh(gmshFile, 3);

// 	//ds->setResponseGmsh(gmshFile, 10);

// 	//int eqId = 1;
// 	//double dt = 0.005;
// 	//char eqFile[] = "data/EQ-S-1.txt";
// 	//ds->addWave(eqId, dt, eqFile);

// 	//ds->setDynamicSolver(RODS::DynamicSolver::Newmark);
// 	//ds->activeGroundMotion(RODS::Direction::X, eqId, 700.0);
// 	//ds->solveSeismicResponse(1);
// }

void example_frame3d_rods()
{
	double PI = 3.14159265;
	set_damping_ratio(0.05);
	set_rayleigh_damping(2.0*PI / 0.368, 2.0*PI / 0.1);
	
	double mass = 0.005;
	double E = 32.5, G = 13.5416666666667;
	double A_b = 80000.0, A_c = 160000.0;
	double I_by = 266666666.666667, I_cy = 2133333333.33333;
	double I_bz = 1066666666.66667, I_cz = 2133333333.33333;
	double Ip_b = 732416666.666667, Ip_c = 3605333333.33333;
	double EA_b = E * A_b, EA_c = E * A_c;
	double EI_by = E * I_by, EI_cy = E * I_cy;
	double EI_bz = E * I_bz, EI_cz = E * I_cz;
	double GIp_c = G*Ip_c, GIp_b = G*Ip_b;

	int nnd = 36;
	int ne = 63;

	double nodeCoord[36][3]{
		{-6000,0,0},
		{0,0,0},
		{6000,0,0},
		{-6000,0,3000},
		{0,0,3000},
		{6000,0,3000},
		{-6000,0,6000},
		{0,0,6000},
		{6000,0,6000},
		{-6000,0,9000},
		{0,0,9000},
		{6000,0,9000},
		{-6000,6000,0},
		{0,6000,0},
		{6000,6000,0},
		{-6000,6000,3000},
		{0,6000,3000},
		{6000,6000,3000},
		{-6000,6000,6000},
		{0,6000,6000},
		{6000,6000,6000},
		{-6000,6000,9000},
		{0,6000,9000},
		{6000,6000,9000},
		{-6000,12000,0},
		{0,12000,0},
		{6000,12000,0},
		{-6000,12000,3000},
		{0,12000,3000},
		{6000,12000,3000},
		{-6000,12000,6000},
		{0,12000,6000},
		{6000,12000,6000},
		{-6000,12000,9000},
		{0,12000,9000},
		{6000,12000,9000}
	};

	double x = 0, y = 0, z = 0;
	for (int i = 0; i < nnd; i++)
	{
		add_dof(6*i + 1, X, mass);
		add_dof(6*i + 2, Y, mass);
		add_dof(6*i + 3, Z, mass);
		add_dof(6*i + 4, RX, mass);
		add_dof(6*i + 5, RY, mass);
		add_dof(6*i + 6, RZ, mass);

		x = nodeCoord[i][0];
		y = nodeCoord[i][1];
		z = nodeCoord[i][2];
		add_node_3d(i + 1, x, y, z, 6*i+1, 6*i+2, 6*i+3, 6*i+4, 6*i+5, 6*i+6);
	}

	fix_node(1 );
	fix_node(2 );
	fix_node(3 );
	fix_node(13);
	fix_node(14);
	fix_node(15);
	fix_node(25);
	fix_node(26);
	fix_node(27);

	int elementConnect[63][3]{
			{1, 4, 1},
			{2, 5, 1},
			{3, 6, 1},
			{4, 5, 2},
			{5, 6, 2},
			{4, 7, 1},
			{5, 8, 1},
			{6, 9, 1},
			{7, 8, 2},
			{8, 9, 2},
			{7, 10, 1},
			{8, 11, 1},
			{9, 12, 1},
			{10, 11, 2},
			{11, 12, 2},
			{4, 16, 2},
			{5, 17, 2},
			{6, 18, 2},
			{7, 19, 2},
			{8, 20, 2},
			{9, 21, 2},
			{10, 22, 2},
			{11, 23, 2},
			{12, 24, 2},
			{13, 16, 1},
			{14, 17, 1},
			{15, 18, 1},
			{16, 17, 2},
			{17, 18, 2},
			{16, 19, 1},
			{17, 20, 1},
			{18, 21, 1},
			{19, 20, 2},
			{20, 21, 2},
			{19, 22, 1},
			{20, 23, 1},
			{21, 24, 1},
			{22, 23, 2},
			{23, 24, 2},
			{16, 28, 2},
			{17, 29, 2},
			{18, 30, 2},
			{19, 31, 2},
			{20, 32, 2},
			{21, 33, 2},
			{22, 34, 2},
			{23, 35, 2},
			{24, 36, 2},
			{25, 28, 1},
			{26, 29, 1},
			{27, 30, 1},
			{28, 29, 2},
			{29, 30, 2},
			{28, 31, 1},
			{29, 32, 1},
			{30, 33, 1},
			{31, 32, 2},
			{32, 33, 2},
			{31, 34, 1},
			{32, 35, 1},
			{33, 36, 1},
			{34, 35, 2},
			{35, 36, 2}
	};

	int ni = 0, nj = 0, st = 0;
	for (int i = 0; i < ne; i++)
	{
		ni = elementConnect[i][0];
		nj = elementConnect[i][1];
		st = elementConnect[i][2];
		if (st == 1)
		{
			add_frame_elastic_3d(i + 1, ni, nj, EA_c, EI_cy, EI_cz, GIp_c);
		}
		else
		{
			add_frame_elastic_3d(i + 1, ni, nj, EA_b, EI_by, EI_bz, GIp_b);
		}
	}

	int masterNodeId_1 = 17;
	int numSlaveNodes_1 = 8;
	int slaveNodes_1[8] = {4,5,6,16,18,28,29,30};
	add_rigid_diagram_with_slave_nodes(1, masterNodeId_1, slaveNodes_1, numSlaveNodes_1);

	int masterNodeId_2 = 20;
	int numSlaveNodes_2 = 8;
	int slaveNodes_2[8] = {7,8,9,19,21,31,32,33};
	add_rigid_diagram_with_slave_nodes(2, masterNodeId_2, slaveNodes_2, numSlaveNodes_2);

	int masterNodeId_3 = 23;
	int numSlaveNodes_3 = 8;
	int slaveNodes_3[8] = {10,11,12,22,24,34,35,36};
	add_rigid_diagram_with_slave_nodes(3, masterNodeId_3, slaveNodes_3, numSlaveNodes_3);

	set_penalty_weight(1.0e4);

	assemble_matrix();
	solve_eigen();

	print_info(30);
	save_to_json("frame3d.json");
}

int main()
{
	// example_truss_rods();
	// example_wall_RODS();
	example_frame3d_rods();
	return 0;
}