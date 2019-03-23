// DDSYS.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "dof.h"
#include "spring.h"
#include "dsystem.h"

constexpr bool FIXED = true;
constexpr bool RELATIVE = true;
constexpr bool NORM = true;

using namespace std;

void example_shear_building()
{
	double m = 1.0;
	double k = 2025.0;
	double c = 1.0;
	double zeta = 0.0;
	int ndof = 3;
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

	for (int i = 1; i <= ndof; i++)
	{
		ds->addDashpot(i, i - 1, i, c);
	}

	cout << "Numbers of equations: " << ds->eqnCount << endl;

	ds->buildInherentDampingMatrix();
	ds->assembleDampingMatrix();
	ds->C.print("Damping Matrix:");

	mat M_d = ds->Phi.t()*ds->M*ds->Phi;
	mat K_d = ds->Phi.t()*ds->K*ds->Phi;
	mat C_d = ds->Phi.t()*ds->C*ds->Phi;

	ds->solveComplexEigen();
	ds->solveStochasticResponse();

	ds->dsp.print();
}

void example_shear_building_spis2()
{
	double m = 1.0;
	double k = 2025.0;
	double c = 1.0;
	double zeta = 0.0;
	int ndof = 3;
	dsystem *ds = new dsystem(zeta);

	double mu = 0.05;
	double xi = 0.005;
	double kp = 0.05;

	double m_in = mu*m;
	double c_d = 2.0*xi*sqrt(m*k);
	double k_s = kp*k;

	ds->addDof(0, m, FIXED);
	for (int i = 1; i <= ndof; i++)
	{
		ds->addDof(i*10, m);
		ds->addSpring(i, (i-1)*10, i*10, k);
		ds->addDof(i*10+1, RELATIVE);
		ds->addSPIS2(i, (i-1)*10, i*10, i*10+1, m_in, c_d, k_s);
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

	//ds->solveStochasticResponse();
	//ds->dsp.print();

	ds->addTimeseries(1, 0.005, "D:\\Analysis\\DSR\\test\\EQ-S-1.txt");
	cout << ds->tss[1]->nsteps;
}

int main()
{
	//example_shear_building();
	example_shear_building_spis2();
	return 0;
}