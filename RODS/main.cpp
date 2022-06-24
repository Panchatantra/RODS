// This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "armadillo"
#include "DynamicSystem.h"
#include "material/Elastoplastic.h"
#include "material/SteelBilinear.h"
#include "material/ConcreteTrilinear.h"
#include "material/SMABilinear.h"
#include "material/CyclicHardenTrilinear.h"

#include "plot.h"
#include "RODS.h"

using namespace std;

//void example_sdof()
//{
//	double m = 800.0;
//	double k = m*400.0;
//	double zeta = 0.02;
//	double c = 2.0*zeta*sqrt(m*k);
//
//	DynamicSystem *ds = new DynamicSystem(0.02);
//
//	ds->setRayleighDamping(2.0*PI/0.3142, 2.0*PI/0.1);
//	//ds->setNumModesInherentDamping(1);
//
//	ds->addDOF(0, m, FIXED);
//	ds->addDOF(1, m);
//
//	ds->addSpring(0, 0, 1, k);
//	ds->addDashpot(1, 0, 1, c);
//
//	ds->assembleMatrix();
//
//	ds->solveEigen();
//	ds->P.print("Natural Periods:");
//
//	//ds->solveStochasticSeismicResponse();
//	//ds->dsp.print("StochasticSeismicResponse:");
//	//cout << "AnalyticalSolution: " << sqrt(PI / 4 / ds->omg(0) / zeta) / ds->omg(0) << endl;
//
//	double dt = 0.01;
//	int nsteps = 1000;
//	vec t = linspace(0.0, dt*(nsteps - 1), nsteps);
//	double Omg = 1 * ds->omg(0);
//	vec ag = arma::sin(Omg*t);
//	ds->addWave(1, dt, ag);
//
//	char eq[] = "data/EQ-S-1.txt";
//	ds->addWave(2, 0.005, eq);
//
//	int nrd = 2;
//	int *dofIds = new int[nrd] { 0, 1 };
//	char dispOutput[] = "data/disp0.dat";
//	ds->addDOFRecorder(0, dofIds, nrd, RODS::Response::DISP, dispOutput);
//
//	int nre = 2;
//	int *eleIds = new int[nre] { 0, 1 };
//	char eleOutput[] = "data/force0.dat";
//	ds->addElementRecorder(0, eleIds, nre, RODS::Response::FORCE, eleOutput);
//
//	int ts = 2;
//	ds->setDynamicSolver(RODS::DynamicSolver::Newmark);
//	ds->activeGroundMotion(RODS::Direction::X, ts, 4000.0);
//	ds->solveSeismicResponse(1);
//}

//void example_sdof_inerter_system()
//{
//	double zeta = 0.02;
//	double m = 800.0;
//	double k = m*400.0;
//	double c = 2.0*zeta*sqrt(m*k);
//
//	DynamicSystem *ds = new DynamicSystem();
//
//	ds->addDOF(1, m, FIXED);
//	ds->addDOF(2, m);
//
//	ds->addSpring(1, 1, 2, k);
//	ds->addDashpot(2, 1, 2, c);
//
//	double mu = 0.1;
//	double kp = mu / (1.0 - mu);
//	double xi = mu / 2.0*sqrt(kp / 2.0);
//	double m_in = mu * m;
//	double k_s = kp * k;
//	double c_d = 2.0*xi*sqrt(m*k);
//
//	ds->addDOF(11, 0.0);
//	ds->addInerter(11, 1, 11, m_in);
//	ds->addDashpot(12, 1, 11, c_d);
//	ds->addSpring(13, 11, 2, k_s);
//
//	//ds->addSPIS2(3, 1, 2, 11, m_in, c_d, k_s);
//
//	//ds->addTVMD(3, 1, 2, m_in, c_d, k_s);
//
//	ds->assembleMatrix();
//
//	ds->solveEigen();
//	ds->P.print("Natural Periods:");
//
//	char eq[] = "data/EQ-S-1.txt";
//	double dt = 0.005;
//	ds->addWave(1, dt, eq);
//
//	int nrd = 1;
//	int *dofIds = new int[nrd] { 2 };
//	//char dispOutput[] = "data/disp0.dat";
//	char dispOutput[] = "data/disp.dat";
//	ds->addDOFRecorder(0, dofIds, nrd, RODS::Response::DISP, dispOutput);
//
//	int ts = 1;
//	ds->setDynamicSolver(RODS::DynamicSolver::StateSpace_NL);
//	ds->activeGroundMotion(RODS::Direction::X, ts, 4000.0);
//	ds->solveSeismicResponse(30);
//}
//
//void example_sdof_inerter_system_nl()
//{
//	double zeta = 0.02;
//	double m = 800.0;
//	double k = m*400.0;
//	double c = 2.0*zeta*sqrt(m*k);
//
//	double uy = 10.0;
//	double alpha = 0.1;
//
//	DynamicSystem *ds0 = new DynamicSystem();
//	DynamicSystem *ds = new DynamicSystem();
//
//	ds0->addDOF(1, m, FIXED);
//	ds0->addDOF(2, m);
//
//	ds->addDOF(1, m, FIXED);
//	ds->addDOF(2, m);
//
//	//ds0->addSpring(1, 1, 2, k);
//	//ds0->addSpringBilinear(1, 1, 2, k, uy, alpha);
//	ds0->addMaterialSteelBilinear(1, k, k*uy, alpha, 0.0);
//	ds0->addSpringNonlinear(1, 1, 2, 1);
//	ds0->addDashpot(2, 1, 2, c);
//
//	//ds->addSpring(1, 1, 2, k);
//	//ds->addSpringBilinear(1, 1, 2, k, uy, alpha);
//	ds->addMaterialSteelBilinear(1, k, k*uy, alpha, 0.0);
//	ds->addSpringNonlinear(1, 1, 2, 1);
//	ds->addDashpot(2, 1, 2, c);
//
//	double mu = 0.1;
//	double kp = mu / (1.0 - mu);
//	double xi = mu / 2.0*sqrt(kp / 2.0);
//	double m_in = mu * m;
//	double k_s = kp * k;
//	double c_d = 2.0*xi*sqrt(m*k);
//
//	ds->addDOF(11, 0.0);
//	ds->addInerter(11, 1, 11, m_in);
//	ds->addDashpot(12, 1, 11, c_d);
//	//ds->addSpring(13, 11, 2, k_s);
//	//ds->addSpringBilinear(13, 11, 2, k_s, 1.4*uy, alpha);
//	ds->addMaterialSMABilinear(2, k_s, 1.5*k_s*uy, alpha, 0.5*k_s*uy);
//	ds->addSpringNonlinear(13, 11, 2, 2);
//
//	//ds->addSPIS2(3, 1, 2, 11, m_in, c_d, k_s);
//
//	//ds->addTVMD(3, 1, 2, m_in, c_d, k_s);
//
//	ds0->assembleMatrix();
//	ds->assembleMatrix();
//
//	ds->solveEigen();
//	ds->P.print("Natural Periods:");
//
//	char eq[] = "data/EQ-S-1.txt";
//	double dt = 0.005;
//	ds0->addWave(1, dt, eq);
//	ds0->setDynamicSolver(RODS::DynamicSolver::StateSpace_NL);
//	ds0->activeGroundMotion(RODS::Direction::X, 1, 4000.0);
//
//	ds->addWave(1, dt, eq);
//	ds->setDynamicSolver(RODS::DynamicSolver::StateSpace_NL);
//	ds->activeGroundMotion(RODS::Direction::X, 1, 4000.0);
//
//	int nrd = 1;
//	int *dofIds = new int[nrd] { 2 };
//	char dispOutput0[] = "data/disp0_nl.dat";
//	//char dispOutput[] = "data/disp_nl.dat";
//	char dispOutput[] = "data/disp_nl_nl.dat";
//	ds0->addDOFRecorder(1, dofIds, nrd, RODS::Response::DISP, dispOutput0);
//	ds->addDOFRecorder(1, dofIds, nrd, RODS::Response::DISP, dispOutput);
//
//	int nre = 2;
//	int *eleIds = new int[nre] { 1, 13 };
//	char eleOutput[] = "data/def_force_nl.dat";
//	ds->addElementRecorder(1, eleIds, nre, RODS::Response::ALL, eleOutput);
//
//	ds0->solveSeismicResponse(30);
//	ds->solveSeismicResponse(30);
//}
//
//void example_sdof_bl()
//{
//	double m = 1.0;
//	double k = 100.0;
//	double zeta = 0.02;
//	double c = 2.0*zeta*sqrt(m*k);
//
//	double uy = 0.01;
//	double alpha = 0.1;
//
//	DynamicSystem *ds = new DynamicSystem();
//
//	ds->addDOF(0, m, FIXED);
//	ds->addDOF(1, m);
//
//	ds->addMaterialElastoplastic(0, k, k*uy, alpha);
//	ds->addMaterialSMABilinear(1, k, k*uy, alpha, 0.5*k*uy);
//	ds->addSpringNonlinear(0, 0, 1, 0);
//
//	//ds->addSpring(0, 0, 1, k);
//	//ds->addSpring(10, 0, 1, k);
//	//ds->addSlider(0, 0, 1, 0.1);
//	//ds->addSpringBilinear(0, 0, 1, k, uy, alpha);
//	//ds->addSpringBW(0, 0, 1, k, uy, alpha);
//	ds->addDashpot(1, 0, 1, c);
//	//ds->addDashpotExp(1, 0, 1, c, 0.2);
//
//	ds->assembleMatrix();
//	ds->solveEigen();
//
//	double dt = 0.01;
//	int nsteps = 1000;
//	vec t = linspace(0.0, dt*(nsteps - 1), nsteps);
//	double Omg = 1 * ds->omg(0);
//	vec ag = arma::sin(Omg*t);
//	ds->addWave(1, dt, ag);
//
//	char eq[] = "EQ-S-1.txt";
//	ds->addWave(2, 0.005, eq);
//
//	int nrd = 2;
//	int *dofIds = new int[nrd] { 0, 1 };
//	char dispOutput[] = "disp.dat";
//	ds->addDOFRecorder(0, dofIds, nrd, RODS::Response::DISP, dispOutput);
//
//	int nre = 2;
//	int *eleIds = new int[nre] { 0, 1 };
//	char eleOutput[] = "force.dat";
//	ds->addElementRecorder(0, eleIds, nre, RODS::Response::FORCE, eleOutput);
//
//	int ts = 2;
//	ds->setDynamicSolver(RODS::DynamicSolver::StateSpace_NL);
//	ds->activeGroundMotion(RODS::Direction::X, ts, 700.0);
//	ds->solveSeismicResponse(30);
//
//	system("python post.py");
//}
//
//void example_shear_building()
//{
//	double m = 1.0;
//	double k = 100.0;
//	double c = 1.0;
//	double zeta = 0.0;
//	int ndof = 1;
//	auto *ds = new DynamicSystem(zeta);
//
//	ds->addDOF(0, m, FIXED);
//	for (int i = 1; i <= ndof; i++)
//	{
//		ds->addDOF(i, m);
//		ds->addSpring(i, i - 1, i, k);
//	}
//
//	ds->assembleMassMatrix();
//	ds->M.print("Mass Matrix:");
//
//	ds->assembleStiffnessMatrix();
//	ds->K.print("Stiffness Matrix:");
//
//	ds->solveEigen();
//	ds->P.print("Natural Periods:");
//	ds->omg.print("Natural Circular Frequencies:");
//
//	zeta = 0.02;
//	c = 2.0*zeta*k / ds->omg(0);
//	cout << "Damping Coefficient: " << c << endl;
//
//	for (int i = 1; i <= ndof; i++)
//	{
//		ds->addDashpot(i, i - 1, i, c);
//	}
//
//	cout << "Numbers of equations: " << ds->eqnCount << endl;
//
//	ds->buildInherentDampingMatrix();
//	ds->assembleDampingMatrix();
//	ds->C.print("Damping Matrix:");
//
//	//ds->solveComplexEigen();
//	ds->solveStochasticSeismicResponse();
//	ds->dsp.print("StochasticSeismicResponse:");
//	cout << "AnalyticalSolution: " << sqrt(PI / 4 / ds->omg(0) / zeta) / ds->omg(0) << endl;
//
//	double dt = 0.02;
//	int nsteps = 1000;
//	vec t = linspace(0.0, dt*(nsteps - 1), nsteps);
//	double Omg = 0.9*ds->omg(0);
//	vec ag = arma::sin(Omg*t);
//	ds->addWave(1, dt, ag);
//	char eq[] = "EQ-S-1.txt";
//	ds->addWave(2, 0.005, eq);
//
//	int ts = 2;
//	ds->setDynamicSolver(RODS::DynamicSolver::StateSpace);
//	ds->activeGroundMotion(RODS::Direction::X, ts, 700.0);
//	ds->solveSeismicResponse(30);
//}
//
//void example_shear_building_spis2()
//{
//	double m = 1.0;
//	double k = 2025.0;
//	double zeta = 0.0;
//	int ndof = 3;
//	DynamicSystem *ds = new DynamicSystem(zeta);
//
//	double mu = 0.05;
//	double xi = 0.005;
//	double kp = 0.05;
//
//	double m_in = mu * m;
//	double c_d = 2.0*xi*sqrt(m*k);
//	double k_s = kp * k;
//
//	ds->addDOF(0, m, FIXED);
//	for (int i = 1; i <= ndof; i++)
//	{
//		ds->addDOF(i * 10, m);
//		ds->addSpring(i, (i - 1) * 10, i * 10, k);
//		ds->addDOF(i * 10 + 1, 0.0);
//		ds->addSPIS2(i, (i - 1) * 10, i * 10, i * 10 + 1, m_in, c_d, k_s);
//	}
//
//	ds->assembleMassMatrix();
//	ds->M.print("Mass Matrix:");
//
//	ds->assembleStiffnessMatrix();
//	ds->K.print("Stiffness Matrix:");
//
//	ds->solveEigen();
//	ds->P.print("Natural Periods:");
//	ds->omg.print("Natural Circular Frequencies:");
//	ds->Phi.print("Eigen Vectors:");
//
//	//mat M_d = ds->Phi.t()*ds->M*ds->Phi;
//	//M_d.print();
//
//	cout << "Numbers of equations: " << ds->eqnCount << endl;
//
//	ds->buildInherentDampingMatrix();
//	ds->assembleDampingMatrix();
//	ds->C.print("Damping Matrix:");
//
//	//ds->solveStochasticSeismicResponse();
//	//ds->dsp.print();
//}
//
//void test_material()
//{
//	double E = 2.0e5, fy = 400.0;
//	double eps_y = fy / E;
//	double alpha = 0.02;
//	Material1D *tmat;
//	Elastoplastic *mat0 = new Elastoplastic(0, E, fy, alpha);
//	SteelBilinear *mat1 = new SteelBilinear(1, E, fy, alpha);
//
//	double Ec = 3.0e4, fc = 30.0;
//	double eps_c = 0.002, eps_u = 0.0033;
//	double sigma_cr = 0.4*fc, sigma_u = 0.85*fc;
//	ConcreteTrilinear *mat2 = new ConcreteTrilinear(2, Ec, fc, eps_c, sigma_cr, sigma_u, eps_u);
//
//	double sigma_shift = 0.5*fy;
//	SMABilinear *mat3 = new SMABilinear(3, E, fy, alpha, sigma_shift);
//
//	auto *mat4 = new CyclicHardenTrilinear(4, E, fy, 0.2, 1.5*fy, 0.02);
//
//	tmat = mat4->copy();
//	size_t n = 1001;
//	mat s = zeros<mat>(n, 2);
//	vec t = linspace(0, 5, n);
//	s.col(0) = eps_y * t%arma::sin(2.0*PI / 0.5*t);
//	//s.col(0) = eps_y *5.0* arma::sin(2.0*PI / 5.0*t);
//
//	for (size_t i = 0; i < n; i++)
//	{
//		tmat->setStrain(&s(i, 0));
//		tmat->getResponse(true);
//		s(i, 1) = tmat->sigma;
//	}
//
//	s.save("data/mat.txt", raw_ascii);
//	system(R"(gnuplot -e "set term pdf; set output 'data/mat.pdf'; plot 'data/mat.txt' using 1:2 with line")");
//}
//
//void example_truss()
//{
//	DynamicSystem *ds = new DynamicSystem();
//
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
//		ds->addDOF(2 * i + 1, RODS::Direction::X, mass);
//		ds->addDOF(2 * i + 2, RODS::Direction::Z, mass);
//
//		x = nodeCoord[i][0];
//		z = nodeCoord[i][1];
//		ds->addNode(i + 1, x, z, 2 * i + 1, 2 * i + 2, -1);
//	}
//
//	ds->fixNode(1);
//	ds->fixNode(4);
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
//		ds->addTrussElastic2D(i + 1, ni, nj, EA);
//	}
//
//	ds->assembleMatrix();
//	ds->solveEigen();
//
//	char gmshFile[] = "truss.msh";
//	ds->exportGmsh(gmshFile);
//
//	ds->printInfo();
//}

//void example_frame()
//{
//	DynamicSystem *ds = new DynamicSystem(0.05);
//
//	double mass = 0.005;
//	double E = 32.5;
//	double A_b = 80000.0, A_c = 160000.0;
//	double I_b = 1066666666.66667, I_c = 2133333333.33333;
//	double EA_b = E * A_b, EA_c = E * A_c;
//	double EI_b = E * I_b, EI_c = E * I_c;
//
//	int nnd = 12;
//	int ne = 15;
//
//	double nodeCoord[12][2]{
//	{-6000, 0},
//	{-6000, 3000},
//	{-6000, 6000},
//	{-6000, 9000},
//	{0    , 0},
//	{0    , 3000},
//	{0    , 6000},
//	{0    , 9000},
//	{6000 , 0},
//	{6000 , 3000},
//	{6000 , 6000},
//	{6000 , 9000} };
//
//	double x = 0, z = 0;
//	for (int i = 0; i < nnd; i++)
//	{
//		ds->addDOF(3 * i + 1, RODS::Direction::X, mass);
//		ds->addDOF(3 * i + 2, RODS::Direction::Z, mass);
//		ds->addDOF(3 * i + 3, RODS::Direction::RY, mass);
//
//		x = nodeCoord[i][0];
//		z = nodeCoord[i][1];
//		ds->addNode(i + 1, x, z, 3 * i + 1, 3 * i + 2, 3 * i + 3);
//	}
//
//	ds->fixNode(1);
//	ds->fixNode(5);
//	ds->fixNode(9);
//
//	int elementConnect[15][3]{
//	{1, 2, 1},
//	{2, 3, 1},
//	{3, 4, 1},
//	{5, 6, 1},
//	{6, 7, 1},
//	{7, 8, 1},
//	{9, 10, 1},
//	{10, 11, 1},
//	{11, 12, 1},
//	{2, 6, 2},
//	{3, 7, 2},
//	{4, 8, 2},
//	{6, 10, 2},
//	{7, 11, 2},
//	{8, 12, 2} };
//
//	int ni = 0, nj = 0, st = 0;
//	for (int i = 0; i < ne; i++)
//	{
//		ni = elementConnect[i][0];
//		nj = elementConnect[i][1];
//		st = elementConnect[i][2];
//		if (st == 1)
//		{
//			ds->addFrameElastic2D(i + 1, ni, nj, EA_c, EI_c);
//		}
//		else
//		{
//			ds->addFrameElastic2D(i + 1, ni, nj, EA_b, EI_b);
//		}
//	}
//
//	double c = 1.0;
//	
//	ds->addDashpot2D(101, 6, 9, c);
//	ds->addDashpot2D(102, 6, 11, c);
//	ds->addDashpot2D(103, 8, 11, c);
//
//	c = 5.0;
//	double k = 100.0;
//	double alpha = 0.15;
//	//ds->addDashpotExp2D(101, 6, 9, c, alpha);
//	//ds->addDashpotExp2D(102, 6, 11, c, alpha);
//	//ds->addDashpotExp2D(103, 8, 11, c, alpha);
//
//	//ds->addDashpotMaxwell2D(101, 6, 9, k, c, alpha);
//	//ds->addDashpotMaxwell2D(102, 6, 11, k, c, alpha);
//	//ds->addDashpotMaxwell2D(103, 8, 11, k, c, alpha);
//
//	//double k0 = 16.0, uy = 0.5, alfa = 0.02;
//	//ds->addSpringBoucWen(101, ds->Nodes.at(6)->dofX->id, ds->Nodes.at(9)->dofX->id, k0, uy, alfa);
//	//ds->addSpringBoucWen(102, ds->Nodes.at(6)->dofX->id, ds->Nodes.at(11)->dofX->id, k0, uy, alfa);
//	//ds->addSpringBoucWen(103, ds->Nodes.at(8)->dofX->id, ds->Nodes.at(11)->dofX->id, k0, uy, alfa);
//
//	//ds->addSpringBoucWen2D(101, 6, 9, k0, uy, alfa);
//	//ds->addSpringBoucWen2D(102, 6, 11, k0, uy, alfa);
//	//ds->addSpringBoucWen2D(103, 8, 11, k0, uy, alfa);
//
//	ds->setRayleighDamping(2.0*PI / 0.36, 2.0*PI / 0.1);
//
//	ds->assembleMatrix();
//	ds->solveEigen();
//
//	ds->printInfo();
//
//	char gmshFile[] = "data/frame.msh";
//	ds->setResponseGmsh(gmshFile, 1);
//	ds->exportGmsh(gmshFile);
//
//	int nrd = 1;
//	int *dofIds = new int[nrd] {
//		ds->Nodes.at(4)->dofX->id
//	};
//	char dispOutput[] = "data/disp_frame_4d.dat";
//	ds->addDOFRecorder(0, dofIds, nrd, RODS::Response::DISP, dispOutput);
//
//	int nre = 1;
//	int *eleIds = new int[nre] { 102 };
//	char eleOutput[] = "data/damper.dat";
//	ds->addElementRecorder(0, eleIds, nre, RODS::Response::ALL, eleOutput);
//
//	int eqId = 1;
//	double dt = 0.005;
//	char eqFile[] = "data/EQ-S-1.txt";
//	ds->addWave(eqId, dt, eqFile);
//
//	ds->setDynamicSolver(RODS::DynamicSolver::StateSpace);
//	ds->activeGroundMotion(RODS::Direction::X, eqId, 700.0);
//	ds->solveSeismicResponse(1);
//
//	//ds->setDynamicSolver(RODS::DynamicSolver::StateSpace_NL);
//	//ds->activeGroundMotion(RODS::Direction::X, eqId, 700.0);
//	//ds->solveSeismicResponse(30);
//}
//
void example_frame3D()
{
	DynamicSystem *ds = new DynamicSystem(0.05);
	ds->setRayleighDamping(2.0*PI / 0.368, 2.0*PI / 0.1);
	
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
		ds->addDOF(6*i + 1, RODS::Direction::X, mass);
		ds->addDOF(6*i + 2, RODS::Direction::Y, mass);
		ds->addDOF(6*i + 3, RODS::Direction::Z, mass);
		ds->addDOF(6*i + 4, RODS::Direction::RX, mass);
		ds->addDOF(6*i + 5, RODS::Direction::RY, mass);
		ds->addDOF(6*i + 6, RODS::Direction::RZ, mass);

		x = nodeCoord[i][0];
		y = nodeCoord[i][1];
		z = nodeCoord[i][2];
		ds->addNode(i + 1, x, y, z, 6*i+1, 6*i+2, 6*i+3, 6*i+4, 6*i+5, 6*i+6);
	}

	ds->fixNode(1 );
	ds->fixNode(2 );
	ds->fixNode(3 );
	ds->fixNode(13);
	ds->fixNode(14);
	ds->fixNode(15);
	ds->fixNode(25);
	ds->fixNode(26);
	ds->fixNode(27);

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
			ds->addFrameElastic3D(i + 1, ni, nj, EA_c, EI_cy, EI_cz, GIp_c);
		}
		else
		{
			ds->addFrameElastic3D(i + 1, ni, nj, EA_b, EI_by, EI_bz, GIp_b);
		}
	}

	//double c = 5.0;
	//double k = 100.0;
	//double alpha = 0.15;

	//ds->addDashpot2D(101, 6, 11, c);
	//ds->addDashpot2D(102, 6, 11, c);
	//ds->addDashpot2D(103, 8, 11, c);

	//ds->addDashpotExp2D(101, 6, 9, c, alpha);
	//ds->addDashpotExp2D(102, 6, 11, c, alpha);
	//ds->addDashpotExp2D(103, 8, 11, c, alpha);

	//ds->addDashpotMaxwell2D(101, 6, 9, k, c, alpha);
	//ds->addDashpotMaxwell2D(102, 6, 11, k, c, alpha);
	//ds->addDashpotMaxwell2D(103, 8, 11, k, c, alpha);

	//double k0 = 16.0, uy = 0.5, alfa = 0.02;
	//ds->addSpringBoucWen2D(101, 6, 9, k0, uy, alfa);
	//ds->addSpringBoucWen2D(102, 6, 11, k0, uy, alfa);
	//ds->addSpringBoucWen2D(103, 8, 11, k0, uy, alfa);

	ds->assembleMatrix();
	ds->solveEigen();

	ds->printInfo();

	ds->Phi.cols(0, 2).print();

	ds->M.save("M.txt", raw_ascii);
	ds->K.save("K.txt", raw_ascii);

	//cout << ds->Phi.col(0).t()*ds->M*ds->Phi.col(0) << endl;
	//cout << ds->Phi.col(1).t()*ds->M*ds->Phi.col(1) << endl;
	//cout << ds->Phi.col(0).t()*ds->M*ds->Phi.col(1) << endl;
	//cout << ds->Phi.col(0).t()*ds->K*ds->Phi.col(1) << endl;
	//cout << ds->omg(0)*ds->omg(0)*ds->M*ds->Phi.col(0) - ds->K*ds->Phi.col(0) << endl;
	//cout << ds->omg(1)*ds->omg(1)*ds->M*ds->Phi.col(1) - ds->K*ds->Phi.col(1) << endl;

	//char gmshFile[] = "data/frame.msh";
	//ds->exportGmsh(gmshFile);

	//int nrd = 1;
	//int *dofIds = new int[nrd] {
	//	ds->Nodes.at(4)->dofX->id
	//};
	//char dispOutput[] = "data/disp_frame_damped.dat";
	//ds->addDOFRecorder(0, dofIds, nrd, RODS::Response::DISP, dispOutput);

	//int nre = 1;
	//int *eleIds = new int[nre] { 102 };
	//char eleOutput[] = "data/damper.dat";
	//ds->addElementRecorder(0, eleIds, nre, RODS::Response::ALL, eleOutput);

	//int eqId = 1;
	//double dt = 0.005;
	//char eqFile[] = "data/EQ-S-1.txt";
	//ds->addWave(eqId, dt, eqFile);

	//ds->setDynamicSolver(RODS::DynamicSolver::StateSpace_NL);
	//ds->activeGroundMotion(RODS::Direction::X, eqId, 700.0);
	//ds->solveSeismicResponse(30);
}
//
//void example_cantilever()
//{
//	DynamicSystem *ds = new DynamicSystem();
//
//	double mass = 0.005;
//	double E = 32.5;
//	double A_b = 80000.0, A_c = 160000.0;
//	double I_b = 1066666666.66667, I_c = 2133333333.33333;
//	double EA_b = E * A_b, EA_c = E * A_c;
//	double EI_b = E * I_b, EI_c = E * I_c;
//
//	int nnd = 2;
//	int ne = 1;
//
//	double nodeCoord[2][2]{ {-6000, 0},
//	{0    , 0}
//	};
//
//	double x = 0, z = 0;
//	for (int i = 0; i < nnd; i++)
//	{
//		ds->addDOF(3 * i + 1, RODS::Direction::X, mass);
//		ds->addDOF(3 * i + 2, RODS::Direction::Z, mass);
//		ds->addDOF(3 * i + 3, RODS::Direction::RY, mass*1.0e-1);
//
//		x = nodeCoord[i][0];
//		z = nodeCoord[i][1];
//		ds->addNode(i + 1, x, z, 3 * i + 1, 3 * i + 2, 3 * i + 3);
//	}
//
//	ds->fixNode(1);
//
//	int elementConnect[1][3]{ {1, 2, 1} };
//
//	int ni = 0, nj = 0, st = 0;
//	for (int i = 0; i < ne; i++)
//	{
//		ni = elementConnect[i][0];
//		nj = elementConnect[i][1];
//		st = elementConnect[i][2];
//		if (st == 1)
//		{
//			ds->addFrameElastic2D(i + 1, ni, nj, EA_b, EI_b);
//			//ds->addBeamElastic2D(i + 1, ni, nj, EI_b);
//		}
//		else
//		{
//			ds->addFrameElastic2D(i + 1, ni, nj, EA_c, EI_c);
//			//ds->addBeamElastic2D(i + 1, ni, nj, EI_c);
//		}
//	}
//
//	ds->assembleMatrix();
//	ds->solveEigen();
//	cout << ds->eqnCount << endl;
//
//	ds->M.print("Mass Matrix:");
//	ds->K.print("Stiffness Matrix:");
//	//ds->K0.print("Stiffness Matrix:");
//	ds->P.print("Natural Periods:");
//	//ds->Phi.print("Modes:");
//
//	//char gmshFile[] = "frame.msh";
//	//ds->exportGmsh(gmshFile);
//}
//
void example_wall()
{
	DynamicSystem *ds = new DynamicSystem(0.05);
	ds->setRayleighDamping(2.0*PI/0.28, 2.0*PI/0.07);

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
		ds->addDOF(2 * i + 1, RODS::Direction::X, mass);
		ds->addDOF(2 * i + 2, RODS::Direction::Z, mass);

		x = crds[i][0];
		z = crds[i][1];
		ds->addNode(i + 1, x, z, 2*i+1, 2*i+2, -1);
	}

	ds->fixNode(1 );
	ds->fixNode(2 );
	ds->fixNode(15);
	ds->fixNode(22);

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
		ds->addQuad4Elastic(i+1, nodeI, nodeJ, nodeP, nodeQ, E, nu, t);
	}

	ds->assembleMatrix();

	ds->solveEigen();
	ds->printInfo();

	char gmshFile[] = "data/wall.msh";
	ds->exportGmsh(gmshFile);
	ds->exportModalGmsh(gmshFile, 1);
	ds->exportModalGmsh(gmshFile, 2);
	ds->exportModalGmsh(gmshFile, 3);

	ds->setResponseGmsh(gmshFile, 10);

	int eqId = 1;
	double dt = 0.005;
	char eqFile[] = "data/EQ-S-1.txt";
	ds->addWave(eqId, dt, eqFile);

	int dofRecorderId = 0;
	char dofRecorderFile[] = "data/top_disp_wall.txt";
	ds->addDOFRecorder(dofRecorderId, RODS::Response::DISP, dofRecorderFile);
	ds->addDOFToRecorder(ds->Nodes.at(21)->dofX->id, dofRecorderId);

	ds->setDynamicSolver(RODS::DynamicSolver::Newmark);
	ds->activeGroundMotion(RODS::Direction::X, eqId, 700.0);
	ds->solveSeismicResponse(1);

}

void example_wall_RODS()
{
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
//
//void example_plane_stress_tri3elastic()
//{
//	DynamicSystem *ds = new DynamicSystem(0.05);
//
//	double mass = 0.0;
//	double E = 200;
//	double nu = 0.3;
//	double t = 100.0;
//
//	int nnd = 63;
//	int ne = 80;
//
//	double crds[63][2] {
//	{   0.,   0.},
//	{ 150.,   0.},
//	{ 300.,   0.},
//	{ 450.,   0.},
//	{ 600.,   0.},
//	{ 750.,   0.},
//	{ 900.,   0.},
//	{1050.,   0.},
//	{1200.,   0.},
//	{1350.,   0.},
//	{1500.,   0.},
//	{1650.,   0.},
//	{1800.,   0.},
//	{1950.,   0.},
//	{2100.,   0.},
//	{2250.,   0.},
//	{2400.,   0.},
//	{2550.,   0.},
//	{2700.,   0.},
//	{2850.,   0.},
//	{3000.,   0.},
//	{   0., 150.},
//	{ 150., 150.},
//	{ 300., 150.},
//	{ 450., 150.},
//	{ 600., 150.},
//	{ 750., 150.},
//	{ 900., 150.},
//	{1050., 150.},
//	{1200., 150.},
//	{1350., 150.},
//	{1500., 150.},
//	{1650., 150.},
//	{1800., 150.},
//	{1950., 150.},
//	{2100., 150.},
//	{2250., 150.},
//	{2400., 150.},
//	{2550., 150.},
//	{2700., 150.},
//	{2850., 150.},
//	{3000., 150.},
//	{   0., 300.},
//	{ 150., 300.},
//	{ 300., 300.},
//	{ 450., 300.},
//	{ 600., 300.},
//	{ 750., 300.},
//	{ 900., 300.},
//	{1050., 300.},
//	{1200., 300.},
//	{1350., 300.},
//	{1500., 300.},
//	{1650., 300.},
//	{1800., 300.},
//	{1950., 300.},
//	{2100., 300.},
//	{2250., 300.},
//	{2400., 300.},
//	{2550., 300.},
//	{2700., 300.},
//	{2850., 300.},
//	{3000., 300.},
//	};
//
//	double x = 0.0, z = 0.0;
//	for (auto i = 0; i < nnd; i++)
//	{
//		ds->addDOF(2 * i + 1, RODS::Direction::X, mass);
//		ds->addDOF(2 * i + 2, RODS::Direction::Z, mass);
//
//		x = crds[i][0];
//		z = crds[i][1];
//		ds->addNode(i + 1, x, z, 2*i+1, 2*i+2, -1);
//	}
//
//	ds->fixNode(1);
//	ds->fixNode(22);
//	ds->fixNode(43);
//	
//	int cn[80][4] {
//	{ 1,  1,  2, 23},
//	{ 2, 23, 22,  1},
//	{ 3,  2,  3, 24},
//	{ 4, 24, 23,  2},
//	{ 5,  3,  4, 25},
//	{ 6, 25, 24,  3},
//	{ 7,  4,  5, 26},
//	{ 8, 26, 25,  4},
//	{ 9,  5,  6, 27},
//	{10, 27, 26,  5},
//	{11,  6,  7, 28},
//	{12, 28, 27,  6},
//	{13,  7,  8, 29},
//	{14, 29, 28,  7},
//	{15,  8,  9, 30},
//	{16, 30, 29,  8},
//	{17,  9, 10, 31},
//	{18, 31, 30,  9},
//	{19, 10, 11, 32},
//	{20, 32, 31, 10},
//	{21, 11, 12, 33},
//	{22, 33, 32, 11},
//	{23, 12, 13, 34},
//	{24, 34, 33, 12},
//	{25, 13, 14, 35},
//	{26, 35, 34, 13},
//	{27, 14, 15, 36},
//	{28, 36, 35, 14},
//	{29, 15, 16, 37},
//	{30, 37, 36, 15},
//	{31, 16, 17, 38},
//	{32, 38, 37, 16},
//	{33, 17, 18, 39},
//	{34, 39, 38, 17},
//	{35, 18, 19, 40},
//	{36, 40, 39, 18},
//	{37, 19, 20, 41},
//	{38, 41, 40, 19},
//	{39, 20, 21, 42},
//	{40, 42, 41, 20},
//	{41, 22, 23, 44},
//	{42, 44, 43, 22},
//	{43, 23, 24, 45},
//	{44, 45, 44, 23},
//	{45, 24, 25, 46},
//	{46, 46, 45, 24},
//	{47, 25, 26, 47},
//	{48, 47, 46, 25},
//	{49, 26, 27, 48},
//	{50, 48, 47, 26},
//	{51, 27, 28, 49},
//	{52, 49, 48, 27},
//	{53, 28, 29, 50},
//	{54, 50, 49, 28},
//	{55, 29, 30, 51},
//	{56, 51, 50, 29},
//	{57, 30, 31, 52},
//	{58, 52, 51, 30},
//	{59, 31, 32, 53},
//	{60, 53, 52, 31},
//	{61, 32, 33, 54},
//	{62, 54, 53, 32},
//	{63, 33, 34, 55},
//	{64, 55, 54, 33},
//	{65, 34, 35, 56},
//	{66, 56, 55, 34},
//	{67, 35, 36, 57},
//	{68, 57, 56, 35},
//	{69, 36, 37, 58},
//	{70, 58, 57, 36},
//	{71, 37, 38, 59},
//	{72, 59, 58, 37},
//	{73, 38, 39, 60},
//	{74, 60, 59, 38},
//	{75, 39, 40, 61},
//	{76, 61, 60, 39},
//	{77, 40, 41, 62},
//	{78, 62, 61, 40},
//	{79, 41, 42, 63},
//	{80, 63, 62, 41},
//	};
//
//	int nodeI = 0, nodeJ = 0, nodeP = 0;
//	for (int i = 0; i < ne; i++)
//	{
//		nodeI = cn[i][1];
//		nodeJ = cn[i][2];
//		nodeP = cn[i][3];
//		ds->addTri3Elastic(i+1, nodeI, nodeJ, nodeP, E, nu, t);
//	}
//
//	int loadId = 1;
//	int nP = 3;
//	double time[3] {0,1,2};
//	double p[3] {0,1,1};
//	double P = -100.0;
//	
//	ds->addLoad(loadId,time,p,nP,0.0,P);
//	
//	ds->addDofLoad(ds->Nodes.at(21)->dofZ->id, loadId);
//	ds->addDofLoad(ds->Nodes.at(63)->dofZ->id, loadId);
//
//	ds->assembleMatrix();
//	ds->solveLinearStaticResponse();
//
//	ds->Nodes.at(21)->dofX->printResponse();
//	ds->Nodes.at(21)->dofZ->printResponse();
//
//	ds->Nodes.at(63)->dofX->printResponse();
//	ds->Nodes.at(63)->dofZ->printResponse();
//
//
//	char gmshFile[] = "data/plane_stress_tri3elastic.msh";
//	ds->exportGmsh(gmshFile);
//	ds->exportResponseGmsh(gmshFile);
//}


void example_plane_stress_rect4elastic()
{
	DynamicSystem *ds = new DynamicSystem(0.05);

	double mass = 0.0;
	double E = 200.0;
	double nu = 0.3;
	double t = 100.0;

	int nnd = 63;
	int ne = 40;

	double crds[63][2] {
	{   0.,   0.},
	{ 150.,   0.},
	{ 300.,   0.},
	{ 450.,   0.},
	{ 600.,   0.},
	{ 750.,   0.},
	{ 900.,   0.},
	{1050.,   0.},
	{1200.,   0.},
	{1350.,   0.},
	{1500.,   0.},
	{1650.,   0.},
	{1800.,   0.},
	{1950.,   0.},
	{2100.,   0.},
	{2250.,   0.},
	{2400.,   0.},
	{2550.,   0.},
	{2700.,   0.},
	{2850.,   0.},
	{3000.,   0.},
	{   0., 150.},
	{ 150., 150.},
	{ 300., 150.},
	{ 450., 150.},
	{ 600., 150.},
	{ 750., 150.},
	{ 900., 150.},
	{1050., 150.},
	{1200., 150.},
	{1350., 150.},
	{1500., 150.},
	{1650., 150.},
	{1800., 150.},
	{1950., 150.},
	{2100., 150.},
	{2250., 150.},
	{2400., 150.},
	{2550., 150.},
	{2700., 150.},
	{2850., 150.},
	{3000., 150.},
	{   0., 300.},
	{ 150., 300.},
	{ 300., 300.},
	{ 450., 300.},
	{ 600., 300.},
	{ 750., 300.},
	{ 900., 300.},
	{1050., 300.},
	{1200., 300.},
	{1350., 300.},
	{1500., 300.},
	{1650., 300.},
	{1800., 300.},
	{1950., 300.},
	{2100., 300.},
	{2250., 300.},
	{2400., 300.},
	{2550., 300.},
	{2700., 300.},
	{2850., 300.},
	{3000., 300.},
	};

	double x = 0.0, z = 0.0;
	for (auto i = 0; i < nnd; i++)
	{
		ds->addDOF(2 * i + 1, RODS::Direction::X, mass);
		ds->addDOF(2 * i + 2, RODS::Direction::Z, mass);

		x = crds[i][0];
		z = crds[i][1];
		ds->addNode(i + 1, x, z, 2*i+1, 2*i+2, -1);
	}

	ds->fixNode(1);
	ds->fixNode(22);
	ds->fixNode(43);

	int cn[80][5] {
	{ 1,  1,  2, 23, 22},
	{ 2,  2,  3, 24, 23},
	{ 3,  3,  4, 25, 24},
	{ 4,  4,  5, 26, 25},
	{ 5,  5,  6, 27, 26},
	{ 6,  6,  7, 28, 27},
	{ 7,  7,  8, 29, 28},
	{ 8,  8,  9, 30, 29},
	{ 9,  9, 10, 31, 30},
	{10, 10, 11, 32, 31},
	{11, 11, 12, 33, 32},
	{12, 12, 13, 34, 33},
	{13, 13, 14, 35, 34},
	{14, 14, 15, 36, 35},
	{15, 15, 16, 37, 36},
	{16, 16, 17, 38, 37},
	{17, 17, 18, 39, 38},
	{18, 18, 19, 40, 39},
	{19, 19, 20, 41, 40},
	{20, 20, 21, 42, 41},
	{21, 22, 23, 44, 43},
	{22, 23, 24, 45, 44},
	{23, 24, 25, 46, 45},
	{24, 25, 26, 47, 46},
	{25, 26, 27, 48, 47},
	{26, 27, 28, 49, 48},
	{27, 28, 29, 50, 49},
	{28, 29, 30, 51, 50},
	{29, 30, 31, 52, 51},
	{30, 31, 32, 53, 52},
	{31, 32, 33, 54, 53},
	{32, 33, 34, 55, 54},
	{33, 34, 35, 56, 55},
	{34, 35, 36, 57, 56},
	{35, 36, 37, 58, 57},
	{36, 37, 38, 59, 58},
	{37, 38, 39, 60, 59},
	{38, 39, 40, 61, 60},
	{39, 40, 41, 62, 61},
	{40, 41, 42, 63, 62},
	};

	int nodeI = 0, nodeJ = 0, nodeP = 0, nodeQ = 0;
	for (int i = 0; i < ne; i++)
	{
		nodeI = cn[i][1];
		nodeJ = cn[i][2];
		nodeP = cn[i][3];
		nodeQ = cn[i][4];
		ds->addRect4Elastic(i+1, nodeI, nodeJ, nodeP, nodeQ, E, nu, t);
	}

	int loadId = 1;
	int nP = 3;
	double time[3] {0,1,2};
	double p[3] {0,1,1};
	double P = -100.0;

	ds->addLoad(loadId,time,p,nP,0.0,P);

	ds->addDofLoad(ds->Nodes.at(21)->dofZ->id, loadId);
	ds->addDofLoad(ds->Nodes.at(63)->dofZ->id, loadId);

	ds->assembleMatrix();
	ds->solveLinearStaticResponse();

	ds->Nodes.at(21)->dofX->printResponse();
	ds->Nodes.at(21)->dofZ->printResponse();

	ds->Nodes.at(63)->dofX->printResponse();
	ds->Nodes.at(63)->dofZ->printResponse();

	char gmshFile[] = "data/plane_stress_rect4elastic.msh";
	ds->exportGmsh(gmshFile);
	ds->exportResponseGmsh(gmshFile);
}

void example_plate4elastic()
{
	DynamicSystem *ds = new DynamicSystem(0.05);

	double mass = 0.0;
	double E = 32.5;
	double nu = 0.2;
	double t = 100.0;

	int nnd = 25;
	int ne = 16;

	double crds[25][3] {
	{   1,    0.,    0.},
	{   2,  750.,    0.},
	{   3, 1500.,    0.},
	{   4, 2250.,    0.},
	{   5, 3000.,    0.},
	{   6,    0.,  750.},
	{   7,  750.,  750.},
	{   8, 1500.,  750.},
	{   9, 2250.,  750.},
	{  10, 3000.,  750.},
	{  11,    0., 1500.},
	{  12,  750., 1500.},
	{  13, 1500., 1500.},
	{  14, 2250., 1500.},
	{  15, 3000., 1500.},
	{  16,    0., 2250.},
	{  17,  750., 2250.},
	{  18, 1500., 2250.},
	{  19, 2250., 2250.},
	{  20, 3000., 2250.},
	{  21,    0., 3000.},
	{  22,  750., 3000.},
	{  23, 1500., 3000.},
	{  24, 2250., 3000.},
	{  25, 3000., 3000.},
	};

	double x = 0.0, y = 0.0;
	for (auto i = 0; i < nnd; i++)
	{
		ds->addDOF(3 * i + 1, RODS::Direction::Z, mass);
		ds->addDOF(3 * i + 2, RODS::Direction::RX, mass);
		ds->addDOF(3 * i + 3, RODS::Direction::RY, mass);

		x = crds[i][1];
		y = crds[i][2];
		ds->addNodePlate2D(i + 1, x, y, 3*i+1, 3*i+2, 3*i+3);
	}

	int fn[16] {1,  2,  3,  4,  5,  6, 10, 11, 15, 16, 20, 21, 22, 23, 24, 25};

	for (auto i = 0; i < 16; i++)
	{
		ds->fixNode(fn[i], RODS::Direction::Z);
	}

	int cn[16][5] {
	{ 1,  1,  2,  7,  6},
	{ 2,  2,  3,  8,  7},
	{ 3,  3,  4,  9,  8},
	{ 4,  4,  5, 10,  9},
	{ 5,  6,  7, 12, 11},
	{ 6,  7,  8, 13, 12},
	{ 7,  8,  9, 14, 13},
	{ 8,  9, 10, 15, 14},
	{ 9, 11, 12, 17, 16},
	{10, 12, 13, 18, 17},
	{11, 13, 14, 19, 18},
	{12, 14, 15, 20, 19},
	{13, 16, 17, 22, 21},
	{14, 17, 18, 23, 22},
	{15, 18, 19, 24, 23},
	{16, 19, 20, 25, 24},
	};

	int nodeI = 0, nodeJ = 0, nodeP = 0, nodeQ = 0;
	for (int i = 0; i < ne; i++)
	{
		nodeI = cn[i][1];
		nodeJ = cn[i][2];
		nodeP = cn[i][3];
		nodeQ = cn[i][4];
		ds->addPlate4Elastic(i+1, nodeI, nodeJ, nodeP, nodeQ, E, nu, t);
	}

	int loadId = 1;
	int nP = 3;
	double time[3] {0,1,2};
	double p[3] {0,1,1};
	double P = -1000.0;

	ds->addLoad(loadId,time,p,nP,0.0,P);

	ds->addDofLoad(ds->Nodes.at(13)->dofZ->id, loadId);

	ds->assembleMatrix();

	ds->printInfo();
	ds->K.print();

	ds->solveLinearStaticResponse();

	ds->Nodes.at(13)->dofZ->printResponse();

	// char gmshFile[] = "data/plane_stress_rect4elastic.msh";
	// ds->exportGmsh(gmshFile);
	// ds->exportResponseGmsh(gmshFile);
}

//void example_shell_rectshell4elastic()
//{
//	DynamicSystem *ds = new DynamicSystem(0.05);
//
//	double mass = 0.0;
//	double E = 32.5;
//	double nu = 0.2;
//	double t = 200.0;
//
//	const int nnd = 132;
//	const int ne = 120;
//
//	double crds[nnd][4] {
//		{  1,    0.,    0., 9000.},
//		{  2,    0., 3000., 9000.},
//		{  3,    0., 3000.,    0.},
//		{  4,    0.,    0.,    0.},
//		{  5, 3000., 3000., 9000.},
//		{  6, 3000., 3000.,    0.},
//		{  7, 3000.,    0., 9000.},
//		{  8, 3000.,    0.,    0.},
//		{  9,    0., 1000., 9000.},
//		{ 10,    0., 2000., 9000.},
//		{ 11,    0., 3000., 8100.},
//		{ 12,    0., 3000., 7200.},
//		{ 13,    0., 3000., 6300.},
//		{ 14,    0., 3000., 5400.},
//		{ 15,    0., 3000., 4500.},
//		{ 16,    0., 3000., 3600.},
//		{ 17,    0., 3000., 2700.},
//		{ 18,    0., 3000., 1800.},
//		{ 19,    0., 3000.,  900.},
//		{ 20,    0., 2000.,    0.},
//		{ 21,    0., 1000.,    0.},
//		{ 22,    0.,    0.,  900.},
//		{ 23,    0.,    0., 1800.},
//		{ 24,    0.,    0., 2700.},
//		{ 25,    0.,    0., 3600.},
//		{ 26,    0.,    0., 4500.},
//		{ 27,    0.,    0., 5400.},
//		{ 28,    0.,    0., 6300.},
//		{ 29,    0.,    0., 7200.},
//		{ 30,    0.,    0., 8100.},
//		{ 31, 1000., 3000., 9000.},
//		{ 32, 2000., 3000., 9000.},
//		{ 33, 3000., 3000., 8100.},
//		{ 34, 3000., 3000., 7200.},
//		{ 35, 3000., 3000., 6300.},
//		{ 36, 3000., 3000., 5400.},
//		{ 37, 3000., 3000., 4500.},
//		{ 38, 3000., 3000., 3600.},
//		{ 39, 3000., 3000., 2700.},
//		{ 40, 3000., 3000., 1800.},
//		{ 41, 3000., 3000.,  900.},
//		{ 42, 2000., 3000.,    0.},
//		{ 43, 1000., 3000.,    0.},
//		{ 44, 3000., 2000., 9000.},
//		{ 45, 3000., 1000., 9000.},
//		{ 46, 3000.,    0., 8100.},
//		{ 47, 3000.,    0., 7200.},
//		{ 48, 3000.,    0., 6300.},
//		{ 49, 3000.,    0., 5400.},
//		{ 50, 3000.,    0., 4500.},
//		{ 51, 3000.,    0., 3600.},
//		{ 52, 3000.,    0., 2700.},
//		{ 53, 3000.,    0., 1800.},
//		{ 54, 3000.,    0.,  900.},
//		{ 55, 3000., 1000.,    0.},
//		{ 56, 3000., 2000.,    0.},
//		{ 57, 2000.,    0., 9000.},
//		{ 58, 1000.,    0., 9000.},
//		{ 59, 1000.,    0.,    0.},
//		{ 60, 2000.,    0.,    0.},
//		{ 61,    0., 1000., 8100.},
//		{ 62,    0., 2000., 8100.},
//		{ 63,    0., 1000., 7200.},
//		{ 64,    0., 2000., 7200.},
//		{ 65,    0., 1000., 6300.},
//		{ 66,    0., 2000., 6300.},
//		{ 67,    0., 1000., 5400.},
//		{ 68,    0., 2000., 5400.},
//		{ 69,    0., 1000., 4500.},
//		{ 70,    0., 2000., 4500.},
//		{ 71,    0., 1000., 3600.},
//		{ 72,    0., 2000., 3600.},
//		{ 73,    0., 1000., 2700.},
//		{ 74,    0., 2000., 2700.},
//		{ 75,    0., 1000., 1800.},
//		{ 76,    0., 2000., 1800.},
//		{ 77,    0., 1000.,  900.},
//		{ 78,    0., 2000.,  900.},
//		{ 79, 1000., 3000., 8100.},
//		{ 80, 2000., 3000., 8100.},
//		{ 81, 1000., 3000., 7200.},
//		{ 82, 2000., 3000., 7200.},
//		{ 83, 1000., 3000., 6300.},
//		{ 84, 2000., 3000., 6300.},
//		{ 85, 1000., 3000., 5400.},
//		{ 86, 2000., 3000., 5400.},
//		{ 87, 1000., 3000., 4500.},
//		{ 88, 2000., 3000., 4500.},
//		{ 89, 1000., 3000., 3600.},
//		{ 90, 2000., 3000., 3600.},
//		{ 91, 1000., 3000., 2700.},
//		{ 92, 2000., 3000., 2700.},
//		{ 93, 1000., 3000., 1800.},
//		{ 94, 2000., 3000., 1800.},
//		{ 95, 1000., 3000.,  900.},
//		{ 96, 2000., 3000.,  900.},
//		{ 97, 3000., 2000., 8100.},
//		{ 98, 3000., 1000., 8100.},
//		{ 99, 3000., 2000., 7200.},
//		{100, 3000., 1000., 7200.},
//		{101, 3000., 2000., 6300.},
//		{102, 3000., 1000., 6300.},
//		{103, 3000., 2000., 5400.},
//		{104, 3000., 1000., 5400.},
//		{105, 3000., 2000., 4500.},
//		{106, 3000., 1000., 4500.},
//		{107, 3000., 2000., 3600.},
//		{108, 3000., 1000., 3600.},
//		{109, 3000., 2000., 2700.},
//		{110, 3000., 1000., 2700.},
//		{111, 3000., 2000., 1800.},
//		{112, 3000., 1000., 1800.},
//		{113, 3000., 2000.,  900.},
//		{114, 3000., 1000.,  900.},
//		{115, 2000.,    0., 8100.},
//		{116, 1000.,    0., 8100.},
//		{117, 2000.,    0., 7200.},
//		{118, 1000.,    0., 7200.},
//		{119, 2000.,    0., 6300.},
//		{120, 1000.,    0., 6300.},
//		{121, 2000.,    0., 5400.},
//		{122, 1000.,    0., 5400.},
//		{123, 2000.,    0., 4500.},
//		{124, 1000.,    0., 4500.},
//		{125, 2000.,    0., 3600.},
//		{126, 1000.,    0., 3600.},
//		{127, 2000.,    0., 2700.},
//		{128, 1000.,    0., 2700.},
//		{129, 2000.,    0., 1800.},
//		{130, 1000.,    0., 1800.},
//		{131, 2000.,    0.,  900.},
//		{132, 1000.,    0.,  900.},
//	};
//
//	double x = 0.0, y = 0.0, z = 0.0;
//	for (auto i = 0; i < nnd; i++)
//	{
//		ds->addDOF(6*i + 1, RODS::Direction::X, mass);
//		ds->addDOF(6*i + 2, RODS::Direction::Y, mass);
//		ds->addDOF(6*i + 3, RODS::Direction::Z, mass);
//		ds->addDOF(6*i + 4, RODS::Direction::RX, mass);
//		ds->addDOF(6*i + 5, RODS::Direction::RY, mass);
//		ds->addDOF(6*i + 6, RODS::Direction::RZ, mass);
//
//		x = crds[i][1];
//		y = crds[i][2];
//		z = crds[i][3];
//		ds->addNode(i + 1, x, y, z, 6*i+1, 6*i+2, 6*i+3, 6*i+4, 6*i+5, 6*i+6);
//	}
//
//	int fn[12] {3,  4,  6,  8, 20, 21, 42, 43, 55, 56, 59, 60};
//
//	for (auto i : fn)
//	{
//		ds->fixNode(i);
//	}
//
//	int cn[ne][5] {
//		{ 1,  1,  9, 61, 30},
//		{ 2,  9, 10, 62, 61},
//		{ 3, 10,  2, 11, 62},
//		{ 4, 30, 61, 63, 29},
//		{ 5, 61, 62, 64, 63},
//		{ 6, 62, 11, 12, 64},
//		{ 7, 29, 63, 65, 28},
//		{ 8, 63, 64, 66, 65},
//		{ 9, 64, 12, 13, 66},
//		{10, 28, 65, 67, 27},
//		{11, 65, 66, 68, 67},
//		{12, 66, 13, 14, 68},
//		{13, 27, 67, 69, 26},
//		{14, 67, 68, 70, 69},
//		{15, 68, 14, 15, 70},
//		{16, 26, 69, 71, 25},
//		{17, 69, 70, 72, 71},
//		{18, 70, 15, 16, 72},
//		{19, 25, 71, 73, 24},
//		{20, 71, 72, 74, 73},
//		{21, 72, 16, 17, 74},
//		{22, 24, 73, 75, 23},
//		{23, 73, 74, 76, 75},
//		{24, 74, 17, 18, 76},
//		{25, 23, 75, 77, 22},
//		{26, 75, 76, 78, 77},
//		{27, 76, 18, 19, 78},
//		{28, 22, 77, 21,  4},
//		{29, 77, 78, 20, 21},
//		{30, 78, 19,  3, 20},
//		{31,  2, 31, 79, 11},
//		{32, 31, 32, 80, 79},
//		{33, 32,  5, 33, 80},
//		{34, 11, 79, 81, 12},
//		{35, 79, 80, 82, 81},
//		{36, 80, 33, 34, 82},
//		{37, 12, 81, 83, 13},
//		{38, 81, 82, 84, 83},
//		{39, 82, 34, 35, 84},
//		{40, 13, 83, 85, 14},
//		{41, 83, 84, 86, 85},
//		{42, 84, 35, 36, 86},
//		{43, 14, 85, 87, 15},
//		{44, 85, 86, 88, 87},
//		{45, 86, 36, 37, 88},
//		{46, 15, 87, 89, 16},
//		{47, 87, 88, 90, 89},
//		{48, 88, 37, 38, 90},
//		{49, 16, 89, 91, 17},
//		{50, 89, 90, 92, 91},
//		{51, 90, 38, 39, 92},
//		{52, 17, 91, 93, 18},
//		{53, 91, 92, 94, 93},
//		{54, 92, 39, 40, 94},
//		{55, 18, 93, 95, 19},
//		{56, 93, 94, 96, 95},
//		{57, 94, 40, 41, 96},
//		{58, 19, 95, 43,  3},
//		{59, 95, 96, 42, 43},
//		{60, 96, 41,  6, 42},
//		{61,   5,  44,  97,  33},
//		{62,  44,  45,  98,  97},
//		{63,  45,   7,  46,  98},
//		{64,  33,  97,  99,  34},
//		{65,  97,  98, 100,  99},
//		{66,  98,  46,  47, 100},
//		{67,  34,  99, 101,  35},
//		{68,  99, 100, 102, 101},
//		{69, 100,  47,  48, 102},
//		{70,  35, 101, 103,  36},
//		{71, 101, 102, 104, 103},
//		{72, 102,  48,  49, 104},
//		{73,  36, 103, 105,  37},
//		{74, 103, 104, 106, 105},
//		{75, 104,  49,  50, 106},
//		{76,  37, 105, 107,  38},
//		{77, 105, 106, 108, 107},
//		{78, 106,  50,  51, 108},
//		{79,  38, 107, 109,  39},
//		{80, 107, 108, 110, 109},
//		{81, 108,  51,  52, 110},
//		{82,  39, 109, 111,  40},
//		{83, 109, 110, 112, 111},
//		{84, 110,  52,  53, 112},
//		{85,  40, 111, 113,  41},
//		{86, 111, 112, 114, 113},
//		{87, 112,  53,  54, 114},
//		{88,  41, 113,  56,   6},
//		{89, 113, 114,  55,  56},
//		{90, 114,  54,   8,  55},
//		{ 91,   7,  57, 115,  46},
//		{ 92,  57,  58, 116, 115},
//		{ 93,  58,   1,  30, 116},
//		{ 94,  46, 115, 117,  47},
//		{ 95, 115, 116, 118, 117},
//		{ 96, 116,  30,  29, 118},
//		{ 97,  47, 117, 119,  48},
//		{ 98, 117, 118, 120, 119},
//		{ 99, 118,  29,  28, 120},
//		{100,  48, 119, 121,  49},
//		{101, 119, 120, 122, 121},
//		{102, 120,  28,  27, 122},
//		{103,  49, 121, 123,  50},
//		{104, 121, 122, 124, 123},
//		{105, 122,  27,  26, 124},
//		{106,  50, 123, 125,  51},
//		{107, 123, 124, 126, 125},
//		{108, 124,  26,  25, 126},
//		{109,  51, 125, 127,  52},
//		{110, 125, 126, 128, 127},
//		{111, 126,  25,  24, 128},
//		{112,  52, 127, 129,  53},
//		{113, 127, 128, 130, 129},
//		{114, 128,  24,  23, 130},
//		{115,  53, 129, 131,  54},
//		{116, 129, 130, 132, 131},
//		{117, 130,  23,  22, 132},
//		{118,  54, 131,  60,   8},
//		{119, 131, 132,  59,  60},
//		{120, 132,  22,   4,  59},
//	};
//
//	int nodeI = 0, nodeJ = 0, nodeP = 0, nodeQ = 0;
//	for (int i = 0; i < ne; i++)
//	{
//		nodeI = cn[i][1];
//		nodeJ = cn[i][2];
//		nodeP = cn[i][3];
//		nodeQ = cn[i][4];
//		ds->addRectShell4Elastic(i+1, nodeI, nodeJ, nodeP, nodeQ, E, nu, t);
//	}
//
//	int loadId = 1;
//	int nP = 3;
//	double time[3] {0,1,2};
//	double p[3] {0,1,1};
//	double P = 1000.0;
//	ds->addLoad(loadId,time,p,nP,0.0,P);
//
//	int ln[4] {1, 2, 5, 7};
//
//	for (auto i : ln)
//	{
//		ds->addDofLoad(ds->Nodes.at(i)->dofY->id, loadId);
//	}
//
//	ds->assembleMatrix();
//	ds->printInfo();
//
//	ds->solveLinearStaticResponse();
//	
//	ds->Nodes.at(1)->dofX->printResponse();
//	ds->Nodes.at(1)->dofY->printResponse();
//	ds->Nodes.at(1)->dofZ->printResponse();
//	ds->Nodes.at(1)->dofRX->printResponse();
//	ds->Nodes.at(1)->dofRY->printResponse();
//	ds->Nodes.at(1)->dofRZ->printResponse();
//
//	 char gmshFile[] = "data/shell_rectshell4elastic.msh";
//	 ds->exportGmsh(gmshFile);
//	 ds->exportResponseGmsh(gmshFile);
//}

//
//void example_nonlinear_spring()
//{
//	double E = 2.06e2, fy = 0.4;
//	double alpha = 0.02;
//	double L = 1000.0;
//
//	double A = 2148.84937505542;
//	auto ds = new DynamicSystem();
//	
//	ds->addDOF(1,RODS::Direction::X,0.0,FIXED);
//	ds->addDOF(2,RODS::Direction::X,0.0);
//
//	ds->addSpringBilinear(1,1,2,E*A/L,fy*A/(E*A/L),alpha);
//
//	int loadId = 1;
//	int nP = 3;
//	double t[3] {0,1,2};
//	double p[3] {0,1,1};
//	double G = fy*A*1.05;
//	ds->addLoad(loadId,t,p,nP,0.0,G);
//	ds->addDofLoad(2, loadId);
//
//	ds->assembleMatrix();
//	
//	ds->solveNonlinearStaticResponse(10);
//
//	ds->Elements.at(1)->printResponse();
//	//ds->printInfo();
//}
//
//void example_nonlinear_truss()
//{
//	double E = 2.06e2, fy = 0.4;
//	double eps_y = fy/E;
//	double alpha = 0.02;
//
//	double Ec = 3.0e4, fc = 30.0;
//	double eps_c = 0.002, eps_u = 0.0033;
//	double sigma_cr = 0.4*fc, sigma_u = 0.85*fc;
//	ConcreteTrilinear *mat2 = new ConcreteTrilinear(2, Ec, fc, eps_c, sigma_cr, sigma_u, eps_u);
//
//	double A = 2148.84937505542;
//	auto ds = new DynamicSystem();
//	int matId = 1;
//	int fiberId = 1;
//	int secId = 1;
//	ds->addMaterialElastoplastic(matId, E, fy, alpha);
//	ds->addFiber(fiberId, matId, A,0.0);
//	ds->addSectionTruss(secId, &fiberId, 1);
//
//	ds->addDOF(1,RODS::Direction::X,0.0,FIXED);
//	ds->addDOF(2,RODS::Direction::Z,0.0,FIXED);
//	ds->addDOF(3,RODS::Direction::X,0.0,FIXED);
//	ds->addDOF(4,RODS::Direction::Z,0.0);
//	ds->addDOF(5,RODS::Direction::X,0.0,FIXED);
//	ds->addDOF(6,RODS::Direction::Z,0.0);
//	
//	double L = 1000.0;
//	ds->addNode(1,0.0,0.0,1,2,-1);
//	ds->addNode(11,0.0,0.5*L,5,6,-1);
//	ds->addNode(2,0.0,L,3,4,-1);
//
//	int loadId = 1;
//	int nP = 3;
//	double t[3] {0,1,2};
//	double p[3] {0,1,1};
//	double G = -fy*A*1.2;
//	ds->addLoad(loadId,t,p,nP,0.0,G);
//	ds->addDofLoad(4, loadId);
//
//	ds->addTruss2D(1,1,11,secId);
//	ds->addTruss2D(2,11,2,secId);
//
//	ds->assembleMatrix();
//
//	int nre = 1;
//	int *eleIds = new int[nre] { 1 };
//	char eleOutput[] = "data/truss.dat";
//	ds->addElementRecorder(1, eleIds, nre, RODS::Response::ALL, eleOutput);
//
//	ds->solveNonlinearStaticResponse(10);
//
//	ds->Elements.at(1)->printResponse();
//	//ds->printInfo();
//}
//
//void example_nonlinear_cantilever()
//{
//	auto ds = new DynamicSystem();
//	int matConcId = 1;
//	int matBarId = 2;
//	int secId = 1;
//
//	double E = 2.0e2, fy = 0.582;
//	double alpha = 0.0;
//
//	double Ec = 3.0e1, fc = 0.0307;
//	double eps_c = 0.0066, eps_u = 0.015;
//	double sigma_cr = 0.4*fc, sigma_u = 0.85*fc;
//	
//	ds->addMaterialConcreteTrilinear(matConcId, Ec, fc, eps_c, sigma_cr, sigma_u, eps_u);
//	ds->addMaterialSteelBilinear(matBarId, E, fy, alpha, 0.5);
//	//ds->addMaterialElastoplastic(matBarId, E, fy, alpha);
//
//	double d = 8.0;
//	double b = 200.0, h = 200.0, as = 15, As = 3*PI*d*d/4.0;
//	int nLayer = 10;
//	double A = b*h/nLayer, y = h/nLayer*0.5;
//	int fiberId = 1;
//	int *fiberIds = new int[nLayer+2];
//
//	for (int i = 0; i < nLayer; ++i)
//	{
//		ds->addFiber(fiberId, matConcId, A, y);
//		fiberIds[fiberId-1] = fiberId;
//		y += h/nLayer;
//		fiberId += 1;
//	}
//
//	fiberIds[fiberId-1] = fiberId;
//	ds->addFiber(fiberId, matBarId, As, as);
//	fiberIds[fiberId] = fiberId+1;
//	ds->addFiber(fiberId+1, matBarId, As, h-as);
//	
//	ds->addSectionFrame2D(secId, fiberIds, nLayer+2);
//
//	ds->addDOF(1,RODS::Direction::X,0.0,FIXED);
//	ds->addDOF(2,RODS::Direction::Z,0.0,FIXED);
//	ds->addDOF(3,RODS::Direction::RY,0.0,FIXED);
//	
//	ds->addDOF(4,RODS::Direction::X,0.0);
//	ds->addDOF(5,RODS::Direction::Z,0.0);
//	ds->addDOF(6,RODS::Direction::RY,0.0);
//
//	ds->addDOF(7,RODS::Direction::X,0.0);
//	ds->addDOF(8,RODS::Direction::Z,0.0);
//	ds->addDOF(9,RODS::Direction::RY,0.0);
//
//	double L = 750.0;
//	ds->addNode(1,0.0,0.0,1,2,3);
//	ds->addNode(2,0.0,0.5*h,7,8,9);
//	ds->addNode(4,0.0,L,4,5,6);
//
//	int loadId = 1;
//	int nP = 3;
//	double t[3] {0,1,100};
//	double p[3] {0,1,1};
//	double G = -140.0;
//	
//	ds->addLoad(loadId,t,p,nP,0.0,G);
//	ds->addDofLoad(5, loadId);
//
//	ds->addFramePDelta2D(1,1,2,secId,2);
//	ds->addFramePDelta2D(2,2,4,secId,2);
//
//	double EA = Ec*b*h;
//	double EI = Ec*b*h*h*h/12.0;
//	//ds->addFrameElastic2D(1,1,2,EA,EI);
//	//ds->addFrameElastic2D(2,2,4,EA,EI);
//
//	ds->assembleMatrix();
//
//	int nrd = 1;
//	int *dofIds = new int[nrd] {
//		ds->Nodes.at(4)->dofX->id
//	};
//	char dispOutput[] = "data/disp_cantilever.dat";
//	ds->addDOFRecorder(1, dofIds, nrd, RODS::Response::DISP, dispOutput);
//
//	int nre = 1;
//	int *eleIds = new int[nre] { 1 };
//	char eleOutput[] = "data/cantilever.dat";
//	ds->addElementRecorder(1, eleIds, nre, RODS::Response::ALL, eleOutput);
//
//	ds->solveNonlinearStaticResponse(10);
//	ds->setLoadConst();
//
//	ds->dsp.print();
//
//	int dispLoadId = 12;
//	int nPd = 25;
//	double td[25] {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24};
//	double pd[25] {0,5,-5,10,-10,15,-15,20,-20,25,-25,30,-30,35,-35,40,-40,45,-45,50,-50,55,-55,60,-60};
//	
//	double scale = 1.0;
//	
//	ds->addLoad(dispLoadId, td, pd, nPd, 0.0, scale);
//
//	int lateralLoadId = 2;
//	ds->addLoad(lateralLoadId, t, p, nP, 0.0, 1.0);
//	ds->addDofLoad(4, lateralLoadId);
//	
//	ds->setDispControlDof(4, dispLoadId);
//
//	double loadedTime = 8.5;
//	double dt = 0.001;
//	int nsub = loadedTime/dt;
//	ds->solveNonlinearStaticResponseDispControlDelta(loadedTime, nsub);
//
//	//ds->printInfo();
//}

int main()
{
	//example_sdof();
	//example_sdof_inerter_system();
	//example_sdof_inerter_system_nl();
	//example_sdof_bl();
	//example_shear_building();
	//example_shear_building_spis2();
	//test_material();
	//example_truss();
	//example_truss_RODS();
	//example_frame();
	example_frame3D();
	//example_cantilever();
	//example_wall();
	//example_wall_RODS();
	//example_nonlinear_spring();
	//example_nonlinear_truss();
	//example_nonlinear_cantilever();
	//example_plane_stress_tri3elastic();
	//example_plane_stress_rect4elastic();
	//example_plate4elastic();
	//example_shell_rectshell4elastic();
	return 0;
}