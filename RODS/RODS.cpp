// DDSYS.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "dof.h"
#include "dsystem.h"
#include "elastoplastic.h"
#include "steelBilinear.h"
#include "concreteTrilinear.h"
#include "SMABilinear.h"

#ifdef __GNUC__
#define DLLEXPORT
#else
#define DLLEXPORT __declspec(dllexport)
#endif

constexpr bool FIXED = true;
constexpr bool NORM = true;

using namespace std;

void example_sdof()
{
	double m = 1.0;
	double k = 100.0;
	double zeta = 0.02;
	double c = 2.0*zeta*sqrt(m*k);

	dsystem *ds = new dsystem();

	ds->addDof(0, m, FIXED);
	ds->addDof(1, m);

	ds->addSpring(0, 0, 1, k);
	ds->addDashpot(1, 0, 1, c);

	ds->assembleMatrix();

	ds->solveEigen();
	ds->P.print("Natural Periods:");

	ds->solveStochasticSeismicResponse();
	ds->dsp.print("StochasticSeismicResponse:");
	cout << "AnalyticalSolution: " << sqrt(PI / 4 / ds->omg(0) / zeta) / ds->omg(0) << endl;

	double dt = 0.01;
	int nsteps = 1000;
	vec t = linspace(0.0, dt*(nsteps - 1), nsteps);
	double Omg = 1 * ds->omg(0);
	vec ag = arma::sin(Omg*t);
	ds->addTimeseries(1, dt, ag);

	char eq[] = "EQ-S-1.txt";
	ds->addTimeseries(2, 0.005, eq);

	int nrd = 2;
	int *dofIds = new int[nrd] { 0, 1 };
	char dispOutput[] = "disp.csv";
	ds->addDofRecorder(0, dofIds, nrd, DISP, dispOutput);

	int nre = 2;
	int *eleIds = new int[nre] { 0, 1 };
	char eleOutput[] = "force.csv";
	ds->addElementRecorder(0, eleIds, nre, FORCE, eleOutput);

	int ts = 2;
	ds->setDynamicSolver(StateSpace);
	ds->solveTimeDomainSeismicResponse(ts, 1.0, 1);
	system("python post.py");
}

void example_sdof_bl()
{
	double m = 1.0;
	double k = 100.0;
	double zeta = 0.02;
	double c = 2.0*zeta*sqrt(m*k);

	double uy = 0.01;
	double alpha = 0.1;

	dsystem *ds = new dsystem();

	ds->addDof(0, m, FIXED);
	ds->addDof(1, m);

	ds->addMaterialElastoplastic(0, k, k*uy, alpha);
	ds->addMaterialSMABilinear(1, k, k*uy, alpha, 0.5*k*uy);
	ds->addSpringNL(0, 0, 1, 1);

	//ds->addSpring(0, 0, 1, k);
	//ds->addSpring(10, 0, 1, k);
	//ds->addSlider(0, 0, 1, 0.1);
	//ds->addSpringBL(0, 0, 1, k, uy, alpha);
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
	ds->addTimeseries(1, dt, ag);

	char eq[] = "EQ-S-1.txt";
	ds->addTimeseries(2, 0.005, eq);

	int nrd = 2;
	int *dofIds = new int[nrd] { 0, 1 };
	char dispOutput[] = "disp.csv";
	ds->addDofRecorder(0, dofIds, nrd, DISP, dispOutput);

	int nre = 2;
	int *eleIds = new int[nre] { 0, 1 };
	char eleOutput[] = "force.csv";
	ds->addElementRecorder(0, eleIds, nre, FORCE, eleOutput);

	int ts = 2;
	ds->setDynamicSolver(StateSpace_NL);
	ds->solveTimeDomainSeismicResponse(ts, 1.0, 1);

	system("python post.py");
}

void example_shear_building()
{
	double m = 1.0;
	double k = 100.0;
	double c = 1.0;
	double zeta = 0.0;
	int ndof = 1;
	dsystem *ds = new dsystem(zeta);

	ds->addDof(0, m, FIXED);
	for (int i = 1; i <= ndof; i++)
	{
		ds->addDof(i, m);
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
	ds->addTimeseries(1, dt, ag);
	char eq[] = "EQ-S-1.txt";
	ds->addTimeseries(2, 0.005, eq);

	int ts = 2;
	ds->solveTimeDomainSeismicResponseStateSpace(ts, 1, 10);
	rowvec tt = ds->tss[ts]->time.t();
	tt.save("t.csv", csv_ascii);
	ds->u.save("dsp.csv", csv_ascii);
}

void example_shear_building_spis2()
{
	double m = 1.0;
	double k = 2025.0;
	double zeta = 0.0;
	int ndof = 3;
	dsystem *ds = new dsystem(zeta);

	double mu = 0.05;
	double xi = 0.005;
	double kp = 0.05;

	double m_in = mu * m;
	double c_d = 2.0*xi*sqrt(m*k);
	double k_s = kp * k;

	ds->addDof(0, m, FIXED);
	for (int i = 1; i <= ndof; i++)
	{
		ds->addDof(i * 10, m);
		ds->addSpring(i, (i - 1) * 10, i * 10, k);
		ds->addDof(i * 10 + 1, 0.0);
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
	material1D *tmat;
	elastoplastic *mat0 = new elastoplastic(0, E, fy, alpha);
	steelBilinear *mat1 = new steelBilinear(1, E, fy, alpha);

	double Ec = 3.0e4, fc = 30.0;
	double eps_c = 0.002, eps_u = 0.0033;
	double sigma_cr = 0.4*fc, sigma_u = 0.85*fc;
	concreteTrilinear *mat2 = new concreteTrilinear(2, Ec, fc, eps_c, sigma_cr, sigma_u, eps_u);

	double sigma_shift = 0.5*fy;
	SMABilinear *mat3 = new SMABilinear(3, E, fy, alpha, sigma_shift);

	tmat = mat3->copy();
	int n = 1001;
	mat s = zeros<mat>(n, 2);
	vec t = linspace(0, 10, n);
	s.col(0) = eps_y * t%arma::sin(2.0*PI / 1.0*t);

	for (size_t i = 0; i < n; i++)
	{
		tmat->setStrain(&s(i, 0));
		tmat->getResponse(true);
		s(i, 1) = tmat->sigma;
	}

	s.save("mat.txt", raw_ascii);
	system("gnuplot -e \"set term pdf; set output \\\"mat.pdf\\\"; plot \\\"mat.txt\\\" using 1:2 with line\" ");
}

int main()
{
	//example_sdof();
	example_sdof_bl();
	//example_shear_building();
	//example_shear_building_spis2();
	//test_material();
	return 0;
}

extern "C" {
	dsystem *ds = new dsystem();

	DLLEXPORT void set_damping_ratio(const double z) {
		ds->zeta = z;
	}

	DLLEXPORT size_t add_dof(const int id, const double m) {
		ds->addDof(id, m);
		return ds->dofs.size();
	}

	DLLEXPORT size_t add_dof_fixed(const int id, const double m) {
		ds->addDof(id, m, FIXED);
		return ds->dofs.size();
	}

	DLLEXPORT size_t add_spring(const int id, const int i, const int j, const double k) {
		ds->addSpring(id, i, j, k);
		return ds->springs.size();
	}

	DLLEXPORT size_t add_dashpot(const int id, const int i, const int j, const double c) {
		ds->addDashpot(id, i, j, c);
		return ds->dashpots.size();
	}

	DLLEXPORT size_t add_timeseries(const int id, const double dt, char* fileName) {
		ds->addTimeseries(id, dt, fileName);
		return ds->tss.size();
	}

	DLLEXPORT size_t assemble_matrix() {
		ds->assembleMatrix();
		return ds->eqnCount;
	}

	DLLEXPORT size_t add_dof_recorder(const int id, int *dofIds, const int n, const int rtype, char * fileName) {
		ds->addDofRecorder(id, dofIds, n, response(rtype), fileName);
		return ds->drs.size();
	}

	DLLEXPORT size_t add_ele_recorder(const int id, int *eleIds, const int n, const int rtype, char * fileName) {
		ds->addElementRecorder(id, eleIds, n, response(rtype), fileName);
		return ds->ers.size();
	}

	DLLEXPORT size_t set_dynamic_solver(const int s) {
		ds->setDynamicSolver(dsolver(s));
		return ds->dynamicSolver;
	}

	DLLEXPORT size_t solve_seismic_response(const int tsId, const double s = 1.0, const int nsub = 1) {
		ds->solveTimeDomainSeismicResponse(tsId, s, nsub);
		return ds->cstep;
	}
}