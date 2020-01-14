#pragma once

#include <string>
#include <map>
#include <armadillo>
#include "dof.h"
#include "node.h"
#include "spring.h"
#include "springBilinear.h"
#include "springBoucWen.h"
#include "dashpot.h"
#include "dashpotExp.h"
#include "inerter.h"
#include "spis2.h"
#include "timeseries.h"
#include "dofRecorder.h"

using namespace arma;

constexpr double PI = 3.14159265;

class dsystem
{
public:
	dsystem(const double z=0.0);
	~dsystem();

	void addNode(node *nd);
	void addNode(const int id, const double x=0.0, const double y=0.0, const double z=0.0);

    void addDof(dof *d);
	void addDof(const int id, const double m=0.0, const bool fixed=false);
	void addDof(const int id, direction dir, const double m=0.0, const bool fixed=false);
    
	void mapDofNode(dof *d, node *nd);
	void mapDofNode(const int id_d, const int id_nd);

	void addSpring(spring *s);
	void addSpring(const int n, const int ni, const int nj, const double k);
	void addSpringBL(springBilinear *s);
	void addSpringBL(const int n, const int ni, const int nj, const double k0, const double uy, const double alpha=0.0);
	void addSpringBW(springBoucWen *s);
	void addSpringBW(const int n, const int ni, const int nj, const double k0, const double uy, const double alpha=0.0);
	void addDashpot(dashpot *d);
	void addDashpot(const int n, const int ni, const int nj, const double c);
	void addDashpotExp(dashpotExp *d);
	void addDashpotExp(const int n, const int ni, const int nj, const double c, const double alpha);
	void addInerter(inerter *in);
	void addInerter(const int n, const int ni, const int nj, const double m);
	void addSPIS2(spis2 *s);
	void addSPIS2(const int n, const int ni, const int nj, const int nin, const double m, const double c, const double k);

	void addTimeseries(timeseries *ts);
	void addTimeseries(const int n, const double dt, const vec &s);
	void addTimeseries(const int n, const double dt, const char* fileName);

	void addDofRecorder(dofRecorder *dr);
	void addDofRecorder(const int id, int *dofIds, const int n, char * fileName);

	void buildDofEqnMap();
	void assembleMassMatrix();
	void assembleStiffnessMatrix();
	void buildInherentDampingMatrix(const int n = 0);
	void buildRayleighDampingMatrix(const double omg1, const double omg2);
	void buildRayleighDampingMatrix(const int md1, const int md2);
	void assembleDampingMatrix();
	void solveEigen();
	void solveComplexEigen();
	void solveStochasticSeismicResponse(const double f_h=50.0, const int nf=10000, const char method='c');
	void solveTimeDomainSeismicResponse(const int tsn, const double s=1.0, const int nsub=1);
	void solveTimeDomainSeismicResponseStateSpace(const int tsn, const double s=1.0, const int nsub=1);
	void solveTimeDomainSeismicResponseStateSpaceNL(const int tsn, const double s=1.0, const int nsub=1);
	void solveTimeDomainSeismicResponseRK4(const int tsn, const double s = 1.0, const int nsub = 1);
	void setDofResponse();
	void assembleNonlinearForceVector(const bool update=false);
	void initRecorders();
	void recordResponse();
	void saveResponse();
	
	std::map<int, node *> nodes;
	std::map<int, dof *> dofs;
	std::map<int, int> dofMapEqn;
	std::map<int, int> eqnMapDof;
	std::map<int, int> dofMapNode;
	std::map<int, spring *> springs;
	std::map<int, springBilinear *> springBLs;
	std::map<int, springBoucWen *> springBWs;
	std::map<int, dashpot *> dashpots;
	std::map<int, dashpotExp *> dashpotExps;
	std::map<int, inerter *> inerters;
	std::map<int, spis2 *> spis2s;

	std::map<int, timeseries *> tss;
	std::map<int, dofRecorder *> drs;

	double zeta;
	int eqnCount;

	bool eigenVectorNormed;

	mat Mp, K, C, M;
	mat Phi;
	vec omg, P;
	vec E;
	vec dsp, vel, acc;
	mat u, v, a;

	mat K0;
	vec q;

	double dt, ctime;
	int nsteps, cstep;
};
