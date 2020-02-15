#pragma once

#include <map>
#include <armadillo>
#include "Basis.h"
#include "dof.h"
#include "node.h"
#include "line.h"
#include "element/spring.h"
#include "element/springBilinear.h"
#include "element/springBoucWen.h"
#include "element/dashpot.h"
#include "element/dashpotExp.h"
#include "element/dashpotMaxwell.h"
#include "element/inerter.h"
#include "element/slider.h"
#include "element/spis2.h"
#include "element/TVMD.h"
#include "element/springNonlinear.h"
#include "element/spring2D.h"
#include "element/springBoucWen2D.h"
#include "element/dashpot2D.h"
#include "element/dashpotExp2D.h"
#include "element/inerter2D.h"
#include "element/dashpotMaxwell2D.h"
#include "element/trussElastic.h"
#include "element/beamElastic.h"
#include "element/FrameElastic2D.h"
#include "element/Quad4Elastic.h"
#include "element/Truss2D.h"
#include "element/Frame2D.h"
#include "TimeSeries.h"
#include "recorder/recorder.h"
#include "recorder/dofRecorder.h"
#include "recorder/elementRecorder.h"
#include "material/material.h"
#include "material/material1D.h"
#include "section/Section.h"
#include "section/Fiber.h"

using namespace arma;

constexpr double PI = 3.14159265;
constexpr bool NORM = true;

enum dsolver
{
	Newmark, Newmark_NL, StateSpace, StateSpace_NL
};

class DynamicSystem : public Basis
{
public:
	DynamicSystem(const double z=0.0);
	~DynamicSystem();

	void addNode(node *nd);
	void addNode(const int id, const double x, const int dofId);
	void addNode(const int id, const double x, const double z, const int dofXId, const int dofZId, const int dofRYId);
	void addNode(const int id, const double x, const double y, const double z);

	void addNodeWithDof(const int id, const double x, const int dofId);

	void addLine(line *l);
	void addLine(const int id, const int ni, const int nj);

	void fixDof(const int id);
	void fixNode(const int id);

	void addDofLoad(const int id, const double load);

	void exportGmsh(char * fileName);

    void addDof(dof *d);
	void addDof(const int id, const double m, const bool fixed=false);
	void addDof(const int id, direction dir, const double m, const bool fixed=false);

	void setMass(const int id, const double m);
	void setNodeMass(const int id, const double m);

	void mapDofNode(dof *d, node *nd);
	void mapDofNode(const int id_d, const int id_nd);

	bool addMaterial1D(material1D *mtrl);
	bool addMaterialElastic(const int id, const double E0);
	bool addMaterialElastoplastic(const int id, const double E0, const double fy, const double alpha=0.02);
	bool addMaterialSteelBilinear(const int id, const double E0, const double fy, const double alpha=0.02, const double beta=0.5);
	bool addMaterialConcreteTrilinear(const int id, const double E0, const double fc, const double epsilon_c,
		const double sigma_cr, const double sigma_u, const double epsilon_u);
	bool addMaterialSMABilinear(const int id, const double E0, const double fy, const double alpha, const double sigma_shift);

	bool addFiber(const int id, const int matId, const double A, const double y, const double z=0.0);
	bool addSectionTruss(const int id, int *fiberIds, const int nFibers);
	bool addSectionFrame2D(const int id, int *fiberIds, const int nFibers);

	bool addElement(element *e);

	void addSpring(spring *s);
	void addSpring(const int id, const int ni, const int nj, const double k);
	void addSpringBilinear(springBilinear *s);
	void addSpringBilinear(const int id, const int ni, const int nj, const double k0, const double uy, const double alpha=0.0);
	void addSpringNonlinear(springNonlinear *s);
	void addSpringNonlinear(const int id, const int ni, const int nj, const int matId);
	void addSpringBoucWen(springBoucWen *s);
	void addSpringBoucWen(const int id, const int ni, const int nj, const double k0, const double uy, const double alpha=0.0, const double beta = 0.5, const double n=20);
	void addDashpot(dashpot *d);
	void addDashpot(const int id, const int ni, const int nj, const double c);
	void addDashpotExp(dashpotExp *d);
	void addDashpotExp(const int id, const int ni, const int nj, const double c, const double alpha = 0.1);
	void addDashpotMaxwell(dashpotMaxwell *d);
	void addDashpotMaxwell(const int id, const int ni, const int nj, const double k, const double c, const double alpha = 1.0);
	void addInerter(inerter *in);
	void addInerter(const int id, const int ni, const int nj, const double m);
	void addSlider(slider *s);
	void addSlider(const int id, const int ni, const int nj, const double muN);
	void addSPIS2(spis2 *s);
	void addSPIS2(const int id, const int ni, const int nj, const int nin, const double m, const double c, const double k);
	void addTVMD(TVMD *d);
	void addTVMD(const int id, const int ni, const int nj, const double m, const double c, const double k);

	void addSpring2D(spring2D *s);
	void addSpring2D(const int id, const int ni, const int nj, const double k, ELE::localAxis U=ELE::U1);
	void addSpringBoucWen2D(springBoucWen2D *s);
	void addSpringBoucWen2D(const int id, const int ni, const int nj, const double k0, const double uy, const double alpha=0.0, const double beta=0.5, const double n=20, ELE::localAxis U = ELE::U1);
	void addDashpot2D(dashpot2D *s);
	void addDashpot2D(const int id, const int ni, const int nj, const double c, ELE::localAxis U=ELE::U1);
	void addInerter2D(inerter2D *s);
	void addInerter2D(const int id, const int ni, const int nj, const double m, ELE::localAxis U=ELE::U1);
	void addDashpotExp2D(dashpotExp2D *s);
	void addDashpotExp2D(const int id, const int ni, const int nj, const double c, const double alpha, ELE::localAxis U = ELE::U1);
	void addDashpotMaxwell2D(dashpotMaxwell2D *s);
	void addDashpotMaxwell2D(const int id, const int ni, const int nj, const double k, const double c, const double alpha, ELE::localAxis U = ELE::U1);

	void addTrussElastic(trussElastic *truss);
	void addTrussElastic(const int id, const int ni, const int nj, const double EA);
	void addBeamElastic(beamElastic *beam);
	void addBeamElastic(const int id, const int ni, const int nj, const double EI);
	void addFrameElastic(FrameElastic2D *frame);
	void addFrameElastic(const int id, const int ni, const int nj, const double EA, const double EI);
	void addQuad4Elastic(Quad4Elastic *quad);
	void addQuad4Elastic(const int id, const int nodeI, const int nodeJ,
						const int nodeP, const int nodeQ,
						const double E, const double nu, const double t);

	void addTimeseries(TimeSeries *ts);
	void addTimeseries(const int id, const double dt, const vec &s);
	void addTimeseries(const int id, const double dt, char * fileName);

	void addDofRecorder(dofRecorder *dr);
	void addDofRecorder(const int id, int *dofIds, const int n, response rtype, char * fileName);
	void addElementRecorder(elementRecorder *er);
	void addElementRecorder(const int id, int *eleIds, const int n, response rtype, char * fileName);
	void addSpringRecorder(const int id, int *eleIds, const int n, response rtype, char *  fileName);
	void addDashpotRecorder(const int id, int *eleIds, const int n, response rtype, char * fileName);
	void addInerterRecorder(const int id, int *eleIds, const int n, response rtype, char * fileName);

	void setRayleighDamping(const double omg1, const double omg2);
	void activeGroundMotion(direction dir);
	void buildDofEqnMap();
	void assembleMatrix();
	void assembleMassMatrix();
	void applyRestraint();
	void applyLoad();
	void addGravity();
	void assembleStiffnessMatrix();
	void setNumModesInherentDamping(const int n);
	void buildInherentDampingMatrix();
	void buildRayleighDampingMatrix(const double omg1, const double omg2);
	void buildRayleighDampingMatrix(const int md1, const int md2);
	void assembleDampingMatrix();

	void solveEigen();
	void solveComplexEigen();
	void solveStochasticSeismicResponse(const double f_h=50.0, const int nf=10000, const char method='c');

	void setDynamicSolver(dsolver s) { this->dynamicSolver = s; }
	void solveStaticResponse(const int nsub=1);
	void solveTimeDomainSeismicResponse(const int tsId, const double s=1.0, const int nsub=1);
	void solveTimeDomainSeismicResponseNMK(const int tsId, const double s=1.0, const int nsub=1);
	void solveTimeDomainSeismicResponseNMKNL(const int tsId, const double s=1.0, const int nsub=1, const double tol=1.0e-6, const int maxiter=20);
	void solveTimeDomainSeismicResponseStateSpace(const int tsId, const double s=1.0, const int nsub=1);
	void solveTimeDomainSeismicResponseStateSpaceNL(const int tsId, const double s=1.0, const int nsub=1);
	void solveTimeDomainSeismicResponseRK4(const int tsId, const double s = 1.0, const int nsub = 1);
	void setDofResponse();
	void getElementResponse();
	void assembleNonlinearForceVector(const bool update=false);

	//void solveNonlinearEquation();

	void initRecorders();
	void recordResponse();
	void saveResponse();

	void printInfo();

	std::map<int, int> dofMapEqn;
	std::map<int, int> eqnMapDof;
	std::map<int, int> dofMapNode;

	std::map<int, node *> nodes;
	std::map<int, line *> lines;
	std::map<int, dof *> dofs;
	std::map<int, element *> eles;
	std::map<int, element2D *> ele2Ds;
	std::map<int, Plane2D *> Plane2Ds;

	std::map<int, element *> physicalMassElements; // For Assembling Mp
	std::map<int, element *> inertialMassElements; // For Assembling M other than Mp
	std::map<int, element *> linearElasticElements; // For Assembling K0 and K
	std::map<int, element *> linearDampingElements; // For Assembling C
	std::map<int, element *> nonlinearElements;  // For Assembling q only
	std::map<int, element *> nonlinearTangentElements;  // For Assembling q and K
	std::map<int, element *> nonlinearSecantElements;  // For Assembling q and K0
	std::map<int, element *> nonlinearInitialTangentElements;  // For Assembling q, K0 and K

	std::map<int, spring *> springs;
	std::map<int, springBilinear *> springBLs;
	std::map<int, springNonlinear *> springNLs;
	std::map<int, springBoucWen *> springBWs;
	std::map<int, dashpot *> dashpots;
	std::map<int, dashpotExp *> dashpotExps;
	std::map<int, dashpotMaxwell *> dashpotMaxwells;
	std::map<int, inerter *> inerters;
	std::map<int, spis2 *> spis2s;
	std::map<int, TVMD *> TVMDs;
	std::map<int, slider *> sliders;

	std::map<int, spring2D *> spring2Ds;
	std::map<int, springBoucWen2D *> springBoucWen2Ds;
	std::map<int, dashpot2D *> dashpot2Ds;
	std::map<int, dashpotMaxwell2D *> dashpotMaxwell2Ds;
	std::map<int, dashpotExp2D *> dashpotExp2Ds;
	std::map<int, inerter2D *> inerter2Ds;

	std::map<int, trussElastic *> trussElastics;
	std::map<int, beamElastic *> beamElastics;
	std::map<int, FrameElastic2D *> FrameElastic2Ds;
	std::map<int, Quad4Elastic *> Quad4Elastics;

	std::map<int, material1D *> material1Ds;
	std::map<int, Fiber *> Fibers;
	std::map<int, Section *> Sections;

	std::map<int, TimeSeries *> tss;
	std::map<int, recorder *> drs;
	std::map<int, recorder *> ers;

	mat Mp, K, C, M;
	mat Phi;
	vec omg, P;
	vec E, Q;
	vec dsp, vel, acc;
	mat u, v, a;

	mat K0;
	vec q;

    double zeta;
    int eqnCount, fixedDofCount;
    bool eigenVectorNormed;

	dsolver dynamicSolver;
	double dt, ctime;
	int nsteps, cstep;

	bool useRayleighDamping;
	double RayleighOmg1, RayleighOmg2;
	int NumModesInherentDamping;
};
