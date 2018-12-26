#pragma once

#include <string>
#include <map>
#include <armadillo>
#include "dof.h"
#include "spring.h"
#include "dashpot.h"
#include "inerter.h"
#include "spis2.h"

using namespace arma;

constexpr double PI = 3.14159265;

class dsystem
{
public:
	dsystem(const double z = 0.05);
	~dsystem();

    void addDof(dof *d);
	void addDof(const int n, const double m, const bool fixed = false);
    void addSpring(spring *s);
	void addSpring(const int n, const int ni, const int nj, const double k);
	void addDashpot(dashpot *d);
	void addDashpot(const int n, const int ni, const int nj, const double c);
	void addInerter(inerter *in);
	void addInerter(const int n, const int ni, const int nj, const double m);
	void addSPIS2(spis2 *s);
	void addSPIS2(const int n, const int ni, const int nj, const int nin, const double m, const double c, const double k);

	void buildDofEqnMap();
	void assembleMassMatrix();
	void assembleStiffnessMatrix();
	void buildInherentDampingMatrix(const int n = 0);
	void buildRayleighDampingMatrix(const double omg1, const double omg2);
	void buildRayleighDampingMatrix(const int md1, const int md2);
	void assembleDampingMatrix();
	void buildGroundMotionVector();
	void solveEigen(bool norm = true);
	void solveComplexEigen();
	void solveStochasticResponse(const double f_h=20.0, const int nf=1000, const char method='c');
	
	std::map<int, dof *> dofs;
	std::map<int, int> dofMapEqn;
	std::map<int, int> eqnMapDof;
	std::map<int, spring *> springs;
	std::map<int, dashpot *> dashpots;
	std::map<int, inerter *> inerters;
	std::map<int, spis2 *> spis2s;

	double zeta;
	int eqnCount;

	bool eigenVectorNormed;

	mat M, K, C, Mi;
	mat Phi;
	vec omg, P;
	vec E;	

};
