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

using namespace std;

void example_sdof()
{
	double m = 800.0;
	double k = m*400.0;
	double zeta = 0.02;
	double c = 2.0*zeta*sqrt(m*k);

	DynamicSystem *ds = new DynamicSystem();

	ds->addDOF(0, m, FIXED);
	ds->addDOF(1, m);

	ds->addSpring(0, 0, 1, k);
	ds->addDashpot(1, 0, 1, c);

	ds->assembleMatrix();

	ds->solveEigen();
	ds->P.print("Natural Periods:");

	//ds->solveStochasticSeismicResponse();
	//ds->dsp.print("StochasticSeismicResponse:");
	//cout << "AnalyticalSolution: " << sqrt(PI / 4 / ds->omg(0) / zeta) / ds->omg(0) << endl;

	double dt = 0.01;
	int nsteps = 1000;
	vec t = linspace(0.0, dt*(nsteps - 1), nsteps);
	double Omg = 1 * ds->omg(0);
	vec ag = arma::sin(Omg*t);
	ds->addWave(1, dt, ag);

	char eq[] = "data/EQ-S-1.txt";
	ds->addWave(2, 0.005, eq);

	int nrd = 2;
	int *dofIds = new int[nrd] { 0, 1 };
	char dispOutput[] = "data/disp0.dat";
	ds->addDOFRecorder(0, dofIds, nrd, RODS::Response::DISP, dispOutput);

	int nre = 2;
	int *eleIds = new int[nre] { 0, 1 };
	char eleOutput[] = "data/force0.dat";
	ds->addElementRecorder(0, eleIds, nre, RODS::Response::FORCE, eleOutput);

	int ts = 2;
	ds->setDynamicSolver(RODS::DynamicSolver::StateSpace_NL);
	ds->activeGroundMotion(RODS::Direction::X, ts, 4000.0);
	ds->solveSeismicResponse(30);
	//system("python post.py");
}

void example_sdof_inerter_system()
{
	double zeta = 0.02;
	double m = 800.0;
	double k = m*400.0;
	double c = 2.0*zeta*sqrt(m*k);

	DynamicSystem *ds = new DynamicSystem();

	ds->addDOF(1, m, FIXED);
	ds->addDOF(2, m);

	ds->addSpring(1, 1, 2, k);
	ds->addDashpot(2, 1, 2, c);

	double mu = 0.1;
	double kp = mu / (1.0 - mu);
	double xi = mu / 2.0*sqrt(kp / 2.0);
	double m_in = mu * m;
	double k_s = kp * k;
	double c_d = 2.0*xi*sqrt(m*k);

	ds->addDOF(11, 0.0);
	ds->addInerter(11, 1, 11, m_in);
	ds->addDashpot(12, 1, 11, c_d);
	ds->addSpring(13, 11, 2, k_s);

	//ds->addSPIS2(3, 1, 2, 11, m_in, c_d, k_s);

	//ds->addTVMD(3, 1, 2, m_in, c_d, k_s);

	ds->assembleMatrix();

	ds->solveEigen();
	ds->P.print("Natural Periods:");

	char eq[] = "data/EQ-S-1.txt";
	double dt = 0.005;
	ds->addWave(1, dt, eq);

	int nrd = 1;
	int *dofIds = new int[nrd] { 2 };
	//char dispOutput[] = "data/disp0.dat";
	char dispOutput[] = "data/disp.dat";
	ds->addDOFRecorder(0, dofIds, nrd, RODS::Response::DISP, dispOutput);

	int ts = 1;
	ds->setDynamicSolver(RODS::DynamicSolver::StateSpace_NL);
	ds->activeGroundMotion(RODS::Direction::X, ts, 4000.0);
	ds->solveSeismicResponse(30);
}

void example_sdof_inerter_system_nl()
{
	double zeta = 0.02;
	double m = 800.0;
	double k = m*400.0;
	double c = 2.0*zeta*sqrt(m*k);

	double uy = 10.0;
	double alpha = 0.1;

	DynamicSystem *ds0 = new DynamicSystem();
	DynamicSystem *ds = new DynamicSystem();

	ds0->addDOF(1, m, FIXED);
	ds0->addDOF(2, m);

	ds->addDOF(1, m, FIXED);
	ds->addDOF(2, m);

	//ds0->addSpring(1, 1, 2, k);
	//ds0->addSpringBilinear(1, 1, 2, k, uy, alpha);
	ds0->addMaterialSteelBilinear(1, k, k*uy, alpha, 0.0);
	ds0->addSpringNonlinear(1, 1, 2, 1);
	ds0->addDashpot(2, 1, 2, c);

	//ds->addSpring(1, 1, 2, k);
	//ds->addSpringBilinear(1, 1, 2, k, uy, alpha);
	ds->addMaterialSteelBilinear(1, k, k*uy, alpha, 0.0);
	ds->addSpringNonlinear(1, 1, 2, 1);
	ds->addDashpot(2, 1, 2, c);

	double mu = 0.1;
	double kp = mu / (1.0 - mu);
	double xi = mu / 2.0*sqrt(kp / 2.0);
	double m_in = mu * m;
	double k_s = kp * k;
	double c_d = 2.0*xi*sqrt(m*k);

	ds->addDOF(11, 0.0);
	ds->addInerter(11, 1, 11, m_in);
	ds->addDashpot(12, 1, 11, c_d);
	//ds->addSpring(13, 11, 2, k_s);
	//ds->addSpringBilinear(13, 11, 2, k_s, 1.4*uy, alpha);
	ds->addMaterialSMABilinear(2, k_s, 1.5*k_s*uy, alpha, 0.5*k_s*uy);
	ds->addSpringNonlinear(13, 11, 2, 2);

	//ds->addSPIS2(3, 1, 2, 11, m_in, c_d, k_s);

	//ds->addTVMD(3, 1, 2, m_in, c_d, k_s);

	ds0->assembleMatrix();
	ds->assembleMatrix();

	ds->solveEigen();
	ds->P.print("Natural Periods:");

	char eq[] = "data/EQ-S-1.txt";
	double dt = 0.005;
	ds0->addWave(1, dt, eq);
	ds0->setDynamicSolver(RODS::DynamicSolver::StateSpace_NL);
	ds0->activeGroundMotion(RODS::Direction::X, 1, 4000.0);

	ds->addWave(1, dt, eq);
	ds->setDynamicSolver(RODS::DynamicSolver::StateSpace_NL);
	ds->activeGroundMotion(RODS::Direction::X, 1, 4000.0);

	int nrd = 1;
	int *dofIds = new int[nrd] { 2 };
	char dispOutput0[] = "data/disp0_nl.dat";
	//char dispOutput[] = "data/disp_nl.dat";
	char dispOutput[] = "data/disp_nl_nl.dat";
	ds0->addDOFRecorder(1, dofIds, nrd, RODS::Response::DISP, dispOutput0);
	ds->addDOFRecorder(1, dofIds, nrd, RODS::Response::DISP, dispOutput);

	int nre = 2;
	int *eleIds = new int[nre] { 1, 13 };
	char eleOutput[] = "data/def_force_nl.dat";
	ds->addElementRecorder(1, eleIds, nre, RODS::Response::ALL, eleOutput);

	ds0->solveSeismicResponse(30);
	ds->solveSeismicResponse(30);
}

void example_sdof_bl()
{
	double m = 1.0;
	double k = 100.0;
	double zeta = 0.02;
	double c = 2.0*zeta*sqrt(m*k);

	double uy = 0.01;
	double alpha = 0.1;

	DynamicSystem *ds = new DynamicSystem();

	ds->addDOF(0, m, FIXED);
	ds->addDOF(1, m);

	ds->addMaterialElastoplastic(0, k, k*uy, alpha);
	ds->addMaterialSMABilinear(1, k, k*uy, alpha, 0.5*k*uy);
	ds->addSpringNonlinear(0, 0, 1, 0);

	//ds->addSpring(0, 0, 1, k);
	//ds->addSpring(10, 0, 1, k);
	//ds->addSlider(0, 0, 1, 0.1);
	//ds->addSpringBilinear(0, 0, 1, k, uy, alpha);
	//ds->addSpringBW(0, 0, 1, k, uy, alpha);
	ds->addDashpot(1, 0, 1, c);
	//ds->addDashpotExp(1, 0, 1, c, 0.2);

	ds->assembleMatrix();
	ds->solveEigen();

	double dt = 0.01;
	int nsteps = 1000;
	vec t = linspace(0.0, dt*(nsteps - 1), nsteps);
	double Omg = 1 * ds->omg(0);
	vec ag = arma::sin(Omg*t);
	ds->addWave(1, dt, ag);

	char eq[] = "EQ-S-1.txt";
	ds->addWave(2, 0.005, eq);

	int nrd = 2;
	int *dofIds = new int[nrd] { 0, 1 };
	char dispOutput[] = "disp.dat";
	ds->addDOFRecorder(0, dofIds, nrd, RODS::Response::DISP, dispOutput);

	int nre = 2;
	int *eleIds = new int[nre] { 0, 1 };
	char eleOutput[] = "force.dat";
	ds->addElementRecorder(0, eleIds, nre, RODS::Response::FORCE, eleOutput);

	int ts = 2;
	ds->setDynamicSolver(RODS::DynamicSolver::StateSpace_NL);
	ds->activeGroundMotion(RODS::Direction::X, ts, 700.0);
	ds->solveSeismicResponse(30);

	system("python post.py");
}

void example_shear_building()
{
	double m = 1.0;
	double k = 100.0;
	double c = 1.0;
	double zeta = 0.0;
	int ndof = 1;
	auto *ds = new DynamicSystem(zeta);

	ds->addDOF(0, m, FIXED);
	for (int i = 1; i <= ndof; i++)
	{
		ds->addDOF(i, m);
		ds->addSpring(i, i - 1, i, k);
	}

	ds->assembleMassMatrix();
	ds->M.print("Mass Matrix:");

	ds->assembleStiffnessMatrix();
	ds->K.print("Stiffness Matrix:");

	ds->solveEigen();
	ds->P.print("Natural Periods:");
	ds->omg.print("Natural Circular Frequencies:");

	zeta = 0.02;
	c = 2.0*zeta*k / ds->omg(0);
	cout << "Damping Coefficient: " << c << endl;

	for (int i = 1; i <= ndof; i++)
	{
		ds->addDashpot(i, i - 1, i, c);
	}

	cout << "Numbers of equations: " << ds->eqnCount << endl;

	ds->buildInherentDampingMatrix();
	ds->assembleDampingMatrix();
	ds->C.print("Damping Matrix:");

	//ds->solveComplexEigen();
	ds->solveStochasticSeismicResponse();
	ds->dsp.print("StochasticSeismicResponse:");
	cout << "AnalyticalSolution: " << sqrt(PI / 4 / ds->omg(0) / zeta) / ds->omg(0) << endl;

	double dt = 0.02;
	int nsteps = 1000;
	vec t = linspace(0.0, dt*(nsteps - 1), nsteps);
	double Omg = 0.9*ds->omg(0);
	vec ag = arma::sin(Omg*t);
	ds->addWave(1, dt, ag);
	char eq[] = "EQ-S-1.txt";
	ds->addWave(2, 0.005, eq);

	int ts = 2;
	ds->setDynamicSolver(RODS::DynamicSolver::StateSpace);
	ds->activeGroundMotion(RODS::Direction::X, ts, 700.0);
	ds->solveSeismicResponse(30);
}

void example_shear_building_spis2()
{
	double m = 1.0;
	double k = 2025.0;
	double zeta = 0.0;
	int ndof = 3;
	DynamicSystem *ds = new DynamicSystem(zeta);

	double mu = 0.05;
	double xi = 0.005;
	double kp = 0.05;

	double m_in = mu * m;
	double c_d = 2.0*xi*sqrt(m*k);
	double k_s = kp * k;

	ds->addDOF(0, m, FIXED);
	for (int i = 1; i <= ndof; i++)
	{
		ds->addDOF(i * 10, m);
		ds->addSpring(i, (i - 1) * 10, i * 10, k);
		ds->addDOF(i * 10 + 1, 0.0);
		ds->addSPIS2(i, (i - 1) * 10, i * 10, i * 10 + 1, m_in, c_d, k_s);
	}

	ds->assembleMassMatrix();
	ds->M.print("Mass Matrix:");

	ds->assembleStiffnessMatrix();
	ds->K.print("Stiffness Matrix:");

	ds->solveEigen();
	ds->P.print("Natural Periods:");
	ds->omg.print("Natural Circular Frequencies:");
	ds->Phi.print("Eigen Vectors:");

	//mat M_d = ds->Phi.t()*ds->M*ds->Phi;
	//M_d.print();

	cout << "Numbers of equations: " << ds->eqnCount << endl;

	ds->buildInherentDampingMatrix();
	ds->assembleDampingMatrix();
	ds->C.print("Damping Matrix:");

	//ds->solveStochasticSeismicResponse();
	//ds->dsp.print();
}

void test_material()
{
	double E = 2.0e5, fy = 400.0;
	double eps_y = fy / E;
	double alpha = 0.02;
	Material1D *tmat;
	Elastoplastic *mat0 = new Elastoplastic(0, E, fy, alpha);
	SteelBilinear *mat1 = new SteelBilinear(1, E, fy, alpha);

	double Ec = 3.0e4, fc = 30.0;
	double eps_c = 0.002, eps_u = 0.0033;
	double sigma_cr = 0.4*fc, sigma_u = 0.85*fc;
	ConcreteTrilinear *mat2 = new ConcreteTrilinear(2, Ec, fc, eps_c, sigma_cr, sigma_u, eps_u);

	double sigma_shift = 0.5*fy;
	SMABilinear *mat3 = new SMABilinear(3, E, fy, alpha, sigma_shift);

	auto *mat4 = new CyclicHardenTrilinear(4, E, fy, 0.2, 1.5*fy, 0.02);

	tmat = mat4->copy();
	size_t n = 1001;
	mat s = zeros<mat>(n, 2);
	vec t = linspace(0, 5, n);
	s.col(0) = eps_y * t%arma::sin(2.0*PI / 0.5*t);
	//s.col(0) = eps_y *5.0* arma::sin(2.0*PI / 5.0*t);

	for (size_t i = 0; i < n; i++)
	{
		tmat->setStrain(&s(i, 0));
		tmat->getResponse(true);
		s(i, 1) = tmat->sigma;
	}

	s.save("data/mat.txt", raw_ascii);
	system(R"(gnuplot -e "set term pdf; set output 'data/mat.pdf'; plot 'data/mat.txt' using 1:2 with line")");
}

void example_truss()
{
	DynamicSystem *ds = new DynamicSystem();

	double mass = 0.01;
	double E = 206.0;
	double A = 2148.84937505542;
	double EA = E * A;

	int nnd = 7;
	int ne = 11;

	double nodeCoord[7][2]{ {-4500 , 0},
	{-1500 , 0},
	{1500  , 0},
	{4500  , 0},
	{-3000 , 3000},
	{0     , 3000},
	{3000  , 3000} };

	double x = 0, z = 0;

	for (int i = 0; i < nnd; i++)
	{
		ds->addDOF(2 * i + 1, RODS::Direction::X, mass);
		ds->addDOF(2 * i + 2, RODS::Direction::Z, mass);

		x = nodeCoord[i][0];
		z = nodeCoord[i][1];
		ds->addNode(i + 1, x, z, 2 * i + 1, 2 * i + 2, -1);
	}

	ds->fixNode(1);
	ds->fixNode(4);

	int elementConnect[11][2]{ {1, 2},
	{2, 3},
	{3, 4},
	{5, 6},
	{6, 7},
	{1, 5},
	{2, 6},
	{3, 7},
	{2, 5},
	{3, 6},
	{4, 7} };

	int ni = 0, nj = 0;
	for (int i = 0; i < ne; i++)
	{
		ni = elementConnect[i][0];
		nj = elementConnect[i][1];
		ds->addTrussElastic2D(i + 1, ni, nj, EA);
	}

	ds->assembleMatrix();
	ds->solveEigen();

	char gmshFile[] = "truss.msh";
	ds->exportGmsh(gmshFile);

	ds->printInfo();
}

void example_frame()
{
	DynamicSystem *ds = new DynamicSystem(0.05);

	double mass = 0.005;
	double E = 32.5;
	double A_b = 80000.0, A_c = 160000.0;
	double I_b = 1066666666.66667, I_c = 2133333333.33333;
	double EA_b = E * A_b, EA_c = E * A_c;
	double EI_b = E * I_b, EI_c = E * I_c;

	int nnd = 12;
	int ne = 15;

	double nodeCoord[12][2]{ {-6000, 0},
	{-6000, 3000},
	{-6000, 6000},
	{-6000, 9000},
	{0    , 0},
	{0    , 3000},
	{0    , 6000},
	{0    , 9000},
	{6000 , 0},
	{6000 , 3000},
	{6000 , 6000},
	{6000 , 9000} };

	double x = 0, z = 0;
	for (int i = 0; i < nnd; i++)
	{
		ds->addDOF(3 * i + 1, RODS::Direction::X, mass);
		ds->addDOF(3 * i + 2, RODS::Direction::Z, mass);
		ds->addDOF(3 * i + 3, RODS::Direction::RY, mass);

		x = nodeCoord[i][0];
		z = nodeCoord[i][1];
		ds->addNode(i + 1, x, z, 3 * i + 1, 3 * i + 2, 3 * i + 3);
	}

	ds->fixNode(1);
	ds->fixNode(5);
	ds->fixNode(9);

	int elementConnect[15][3]{ {1, 2, 1},
	{2, 3, 1},
	{3, 4, 1},
	{5, 6, 1},
	{6, 7, 1},
	{7, 8, 1},
	{9, 10, 1},
	{10, 11, 1},
	{11, 12, 1},
	{2, 6, 2},
	{3, 7, 2},
	{4, 8, 2},
	{6, 10, 2},
	{7, 11, 2},
	{8, 12, 2} };

	int ni = 0, nj = 0, st = 0;
	for (int i = 0; i < ne; i++)
	{
		ni = elementConnect[i][0];
		nj = elementConnect[i][1];
		st = elementConnect[i][2];
		if (st == 1)
		{
			ds->addFrameElastic2D(i + 1, ni, nj, EA_c, EI_c);
		}
		else
		{
			ds->addFrameElastic2D(i + 1, ni, nj, EA_b, EI_b);
		}
	}

	double c = 5.0;
	double k = 100.0;
	double alpha = 0.15;
	
	//ds->addDashpot2D(101, 6, 11, c);
	//ds->addDashpot2D(102, 6, 11, c);
	//ds->addDashpot2D(103, 8, 11, c);
	
	//ds->addDashpotExp2D(101, 6, 9, c, alpha);
	//ds->addDashpotExp2D(102, 6, 11, c, alpha);
	//ds->addDashpotExp2D(103, 8, 11, c, alpha);

	//ds->addDashpotMaxwell2D(101, 6, 9, k, c, alpha);
	//ds->addDashpotMaxwell2D(102, 6, 11, k, c, alpha);
	//ds->addDashpotMaxwell2D(103, 8, 11, k, c, alpha);

	double k0 = 16.0, uy = 0.5, alfa = 0.02;
	//ds->addSpringBoucWen(101, ds->Nodes.at(6)->dofX->id, ds->Nodes.at(9)->dofX->id, k0, uy, alfa);
	//ds->addSpringBoucWen(102, ds->Nodes.at(6)->dofX->id, ds->Nodes.at(11)->dofX->id, k0, uy, alfa);
	//ds->addSpringBoucWen(103, ds->Nodes.at(8)->dofX->id, ds->Nodes.at(11)->dofX->id, k0, uy, alfa);

	ds->addSpringBoucWen2D(101, 6, 9, k0, uy, alfa);
	ds->addSpringBoucWen2D(102, 6, 11, k0, uy, alfa);
	ds->addSpringBoucWen2D(103, 8, 11, k0, uy, alfa);

	ds->setRayleighDamping(2.0*PI / 0.36, 2.0*PI / 0.1);

	ds->assembleMatrix();
	ds->solveEigen();

	ds->printInfo();

	char gmshFile[] = "data/frame.msh";
	ds->exportGmsh(gmshFile);

	int nrd = 1;
	int *dofIds = new int[nrd] {
		ds->Nodes.at(4)->dofX->id
	};
	char dispOutput[] = "data/disp_frame_damped.dat";
	ds->addDOFRecorder(0, dofIds, nrd, RODS::Response::DISP, dispOutput);

	int nre = 1;
	int *eleIds = new int[nre] { 102 };
	char eleOutput[] = "data/damper.dat";
	ds->addElementRecorder(0, eleIds, nre, RODS::Response::ALL, eleOutput);

	int eqId = 1;
	double dt = 0.005;
	char eqFile[] = "data/EQ-S-1.txt";
	ds->addWave(eqId, dt, eqFile);

	ds->setDynamicSolver(RODS::DynamicSolver::StateSpace_NL);
	ds->activeGroundMotion(RODS::Direction::X, eqId, 700.0);
	ds->solveSeismicResponse(30);
}

void example_frame3D()
{
	DynamicSystem *ds = new DynamicSystem(0.05);

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

	ds->setRayleighDamping(2.0*PI / 0.36, 2.0*PI / 0.1);

	ds->assembleMatrix();
	ds->solveEigen();

	ds->printInfo();

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

void example_cantilever()
{
	DynamicSystem *ds = new DynamicSystem();

	double mass = 0.005;
	double E = 32.5;
	double A_b = 80000.0, A_c = 160000.0;
	double I_b = 1066666666.66667, I_c = 2133333333.33333;
	double EA_b = E * A_b, EA_c = E * A_c;
	double EI_b = E * I_b, EI_c = E * I_c;

	int nnd = 2;
	int ne = 1;

	double nodeCoord[2][2]{ {-6000, 0},
	{0    , 0}
	};

	double x = 0, z = 0;
	for (int i = 0; i < nnd; i++)
	{
		ds->addDOF(3 * i + 1, RODS::Direction::X, mass);
		ds->addDOF(3 * i + 2, RODS::Direction::Z, mass);
		ds->addDOF(3 * i + 3, RODS::Direction::RY, mass*1.0e-1);

		x = nodeCoord[i][0];
		z = nodeCoord[i][1];
		ds->addNode(i + 1, x, z, 3 * i + 1, 3 * i + 2, 3 * i + 3);
	}

	ds->fixNode(1);

	int elementConnect[1][3]{ {1, 2, 1} };

	int ni = 0, nj = 0, st = 0;
	for (int i = 0; i < ne; i++)
	{
		ni = elementConnect[i][0];
		nj = elementConnect[i][1];
		st = elementConnect[i][2];
		if (st == 1)
		{
			ds->addFrameElastic2D(i + 1, ni, nj, EA_b, EI_b);
			//ds->addBeamElastic2D(i + 1, ni, nj, EI_b);
		}
		else
		{
			ds->addFrameElastic2D(i + 1, ni, nj, EA_c, EI_c);
			//ds->addBeamElastic2D(i + 1, ni, nj, EI_c);
		}
	}

	ds->assembleMatrix();
	ds->solveEigen();
	cout << ds->eqnCount << endl;

	ds->M.print("Mass Matrix:");
	ds->K.print("Stiffness Matrix:");
	//ds->K0.print("Stiffness Matrix:");
	ds->P.print("Natural Periods:");
	//ds->Phi.print("Modes:");

	//char gmshFile[] = "frame.msh";
	//ds->exportGmsh(gmshFile);
}

void example_wall()
{
	DynamicSystem *ds = new DynamicSystem(0.05);

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
	{1500, 6000}};

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
}

void example_nonlinear_spring()
{
	double E = 2.06e2, fy = 0.4;
	double alpha = 0.02;
	double L = 1000.0;

	double A = 2148.84937505542;
	auto ds = new DynamicSystem();
	
	ds->addDOF(1,RODS::Direction::X,0.0,FIXED);
	ds->addDOF(2,RODS::Direction::X,0.0);

	ds->addSpringBilinear(1,1,2,E*A/L,fy*A/(E*A/L),alpha);

	int loadId = 1;
	int nP = 3;
	double t[3] {0,1,2};
	double p[3] {0,1,1};
	double G = fy*A*1.05;
	ds->addLoad(loadId,t,p,nP,0.0,G);
	ds->addDofLoad(2, loadId);

	ds->assembleMatrix();
	
	ds->solveNonlinearStaticResponse(10);

	ds->Elements.at(1)->printResponse();
	//ds->printInfo();
}

void example_nonlinear_truss()
{
	double E = 2.06e2, fy = 0.4;
	double eps_y = fy/E;
	double alpha = 0.02;

	double Ec = 3.0e4, fc = 30.0;
	double eps_c = 0.002, eps_u = 0.0033;
	double sigma_cr = 0.4*fc, sigma_u = 0.85*fc;
	ConcreteTrilinear *mat2 = new ConcreteTrilinear(2, Ec, fc, eps_c, sigma_cr, sigma_u, eps_u);

	double A = 2148.84937505542;
	auto ds = new DynamicSystem();
	int matId = 1;
	int fiberId = 1;
	int secId = 1;
	ds->addMaterialElastoplastic(matId, E, fy, alpha);
	ds->addFiber(fiberId, matId, A,0.0);
	ds->addSectionTruss(secId, &fiberId, 1);

	ds->addDOF(1,RODS::Direction::X,0.0,FIXED);
	ds->addDOF(2,RODS::Direction::Z,0.0,FIXED);
	ds->addDOF(3,RODS::Direction::X,0.0,FIXED);
	ds->addDOF(4,RODS::Direction::Z,0.0);
	ds->addDOF(5,RODS::Direction::X,0.0,FIXED);
	ds->addDOF(6,RODS::Direction::Z,0.0);
	
	double L = 1000.0;
	ds->addNode(1,0.0,0.0,1,2,-1);
	ds->addNode(11,0.0,0.5*L,5,6,-1);
	ds->addNode(2,0.0,L,3,4,-1);

	int loadId = 1;
	int nP = 3;
	double t[3] {0,1,2};
	double p[3] {0,1,1};
	double G = -fy*A*1.2;
	ds->addLoad(loadId,t,p,nP,0.0,G);
	ds->addDofLoad(4, loadId);

	ds->addTruss2D(1,1,11,secId);
	ds->addTruss2D(2,11,2,secId);

	ds->assembleMatrix();

	int nre = 1;
	int *eleIds = new int[nre] { 1 };
	char eleOutput[] = "data/truss.dat";
	ds->addElementRecorder(1, eleIds, nre, RODS::Response::ALL, eleOutput);

	ds->solveNonlinearStaticResponse(10);

	ds->Elements.at(1)->printResponse();
	//ds->printInfo();
}

void example_nonlinear_cantilever()
{
	auto ds = new DynamicSystem();
	int matConcId = 1;
	int matBarId = 2;
	int secId = 1;

	double E = 2.0e2, fy = 0.582;
	double alpha = 0.0;

	double Ec = 3.0e1, fc = 0.0307;
	double eps_c = 0.0066, eps_u = 0.015;
	double sigma_cr = 0.4*fc, sigma_u = 0.85*fc;
	
	ds->addMaterialConcreteTrilinear(matConcId, Ec, fc, eps_c, sigma_cr, sigma_u, eps_u);
	ds->addMaterialSteelBilinear(matBarId, E, fy, alpha, 0.5);
	//ds->addMaterialElastoplastic(matBarId, E, fy, alpha);

	double d = 8.0;
	double b = 200.0, h = 200.0, as = 15, As = 3*PI*d*d/4.0;
	int nLayer = 10;
	double A = b*h/nLayer, y = h/nLayer*0.5;
	int fiberId = 1;
	int *fiberIds = new int[nLayer+2];

	for (int i = 0; i < nLayer; ++i)
	{
		ds->addFiber(fiberId, matConcId, A, y);
		fiberIds[fiberId-1] = fiberId;
		y += h/nLayer;
		fiberId += 1;
	}

	fiberIds[fiberId-1] = fiberId;
	ds->addFiber(fiberId, matBarId, As, as);
	fiberIds[fiberId] = fiberId+1;
	ds->addFiber(fiberId+1, matBarId, As, h-as);
	
	ds->addSectionFrame2D(secId, fiberIds, nLayer+2);

	ds->addDOF(1,RODS::Direction::X,0.0,FIXED);
	ds->addDOF(2,RODS::Direction::Z,0.0,FIXED);
	ds->addDOF(3,RODS::Direction::RY,0.0,FIXED);
	
	ds->addDOF(4,RODS::Direction::X,0.0);
	ds->addDOF(5,RODS::Direction::Z,0.0);
	ds->addDOF(6,RODS::Direction::RY,0.0);

	ds->addDOF(7,RODS::Direction::X,0.0);
	ds->addDOF(8,RODS::Direction::Z,0.0);
	ds->addDOF(9,RODS::Direction::RY,0.0);

	double L = 750.0;
	ds->addNode(1,0.0,0.0,1,2,3);
	ds->addNode(2,0.0,0.5*h,7,8,9);
	ds->addNode(4,0.0,L,4,5,6);

	int loadId = 1;
	int nP = 3;
	double t[3] {0,1,100};
	double p[3] {0,1,1};
	double G = -140.0;
	
	ds->addLoad(loadId,t,p,nP,0.0,G);
	ds->addDofLoad(5, loadId);

	ds->addFramePDelta2D(1,1,2,secId,2);
	ds->addFramePDelta2D(2,2,4,secId,2);

	double EA = Ec*b*h;
	double EI = Ec*b*h*h*h/12.0;
	//ds->addFrameElastic2D(1,1,2,EA,EI);
	//ds->addFrameElastic2D(2,2,4,EA,EI);

	ds->assembleMatrix();

	int nrd = 1;
	int *dofIds = new int[nrd] {
		ds->Nodes.at(4)->dofX->id
	};
	char dispOutput[] = "data/disp_cantilever.dat";
	ds->addDOFRecorder(1, dofIds, nrd, RODS::Response::DISP, dispOutput);

	int nre = 1;
	int *eleIds = new int[nre] { 1 };
	char eleOutput[] = "data/cantilever.dat";
	ds->addElementRecorder(1, eleIds, nre, RODS::Response::ALL, eleOutput);

	ds->solveNonlinearStaticResponse(10);
	ds->setLoadConst();

	ds->dsp.print();

	int dispLoadId = 12;
	int nPd = 25;
	double td[25] {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24};
	double pd[25] {0,5,-5,10,-10,15,-15,20,-20,25,-25,30,-30,35,-35,40,-40,45,-45,50,-50,55,-55,60,-60};
	
	double scale = 1.0;
	
	ds->addLoad(dispLoadId, td, pd, nPd, 0.0, scale);

	int lateralLoadId = 2;
	ds->addLoad(lateralLoadId, t, p, nP, 0.0, 1.0);
	ds->addDofLoad(4, lateralLoadId);
	
	ds->setDispControlDof(4, dispLoadId);

	double loadedTime = 8.5;
	double dt = 0.001;
	int nsub = loadedTime/dt;
	ds->solveNonlinearStaticResponseDispControlDelta(loadedTime, nsub);

	//ds->printInfo();
}

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
	//example_frame();
	example_frame3D();
	//example_cantilever();
	//example_wall();
	//example_nonlinear_spring();
	//example_nonlinear_truss();
	//example_nonlinear_cantilever();
	return 0;
}