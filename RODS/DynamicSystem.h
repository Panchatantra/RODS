#pragma once

#include <map>
#include <armadillo>
#include "Basis.h"
#include "DOF.h"
#include "Node.h"
#include "Line.h"
#include "element/Spring.h"
#include "element/SpringBilinear.h"
#include "element/SpringBoucWen.h"
#include "element/Dashpot.h"
#include "element/DashpotExp.h"
#include "element/DashpotMaxwell.h"
#include "element/Inerter.h"
#include "element/Slider.h"
#include "element/SPIS2.h"
#include "element/TVMD.h"
#include "element/SpringNonlinear.h"
#include "element/Spring2D.h"
#include "element/SpringBoucWen2D.h"
#include "element/Dashpot2D.h"
#include "element/DashpotExp2D.h"
#include "element/Inerter2D.h"
#include "element/DashpotMaxwell2D.h"
#include "element/TrussElastic.h"
#include "element/BeamElastic.h"
#include "element/FrameElastic2D.h"
#include "element/Quad4Elastic.h"
#include "element/Truss2D.h"
#include "element/Frame2D.h"
#include "element/FramePDelta2D.h"
#include "TimeSeries.h"
#include "recorder/Recorder.h"
#include "recorder/dofRecorder.h"
#include "recorder/elementRecorder.h"
#include "material/Material.h"
#include "material/Material1D.h"
#include "section/Section.h"
#include "section/Fiber.h"
#include "section/SectionTruss.h"
#include "section/SectionFrame2D.h"

using namespace arma;

constexpr double PI = 3.14159265;
constexpr bool NORM = true;

enum dsolver
{
	Newmark, Newmark_NL, StateSpace, StateSpace_NL
};

/**
 * @brief      The Dynamic System Class.
 */
class DynamicSystem : public Basis
{
public:
	/**
	 * @brief      Constructs a new instance.
	 *
	 * @param[in]  z     The inherent damping ratio
	 */
	DynamicSystem(const double z=0.0);
	~DynamicSystem();

	void addNode(Node *nd);

	/**
	 * @brief      Adds a 1D Node into the system.
	 *
	 * @param[in]  id     The identifier
	 * @param[in]  x      The X coordinate
	 * @param[in]  dofId  The DOF identifier
	 */
	void addNode(const int id, const double x, const int dofId);

	/**
	 * @brief      Adds a 2D Node into the system.
	 *
	 * @param[in]  id       The identifier
	 * @param[in]  x        The X coordinate
	 * @param[in]  z        The Z coordinate
	 * @param[in]  dofXId   The DOF X identifier
	 * @param[in]  dofZId   The DOF Z identifier
	 * @param[in]  dofRYId  The DOF RY identifier
	 */
	void addNode(const int id, const double x, const double z, const int dofXId, const int dofZId, const int dofRYId);

	void addNode(const int id, const double x, const double y, const double z);

	void addNodeWithDof(const int id, const double x, const int dofId);

	void addLine(Line *l);
	void addLine(const int id, const int ni, const int nj);

	/**
	 * @brief      Fix a DOF.
	 *
	 * @param[in]  id    The DOF identifier
	 */
	void fixDof(const int id);

	/**
	 * @brief      Fix all the DOFs of a Node.
	 *
	 * @param[in]  id    The Node identifier
	 */
	void fixNode(const int id);

	/**
	 * @brief      Adds a load into the system.
	 *
	 * @param[in]  id          The Load identifier
	 * @param      t           The loading moments
	 * @param      p           The loading values related to the moments
	 * @param[in]  nP          The number of loading moments
	 * @param[in]  arriveTime  The arrive time of the load, default to 0.0
	 * @param[in]  scale       The scale factor of the load, default to 1.0
	 */
	void addLoad(const int id, double *t, double *p, const int nP, const double arriveTime=0.0, const double scale=1.0);

	/**
	 * @brief      Adds a Load to a DOF.
	 *
	 * @param[in]  dofId   The DOF identifier
	 * @param[in]  loadId  The Load identifier
	 */
	void addDofLoad(const int dofId, const int loadId);

	/**
	 * @brief      Export the FEM model as GMSH format.
	 *
	 * @param      fileName  The exported file name
	 */
	void exportGmsh(char * fileName);

    void addDof(DOF *d);
	void addDof(const int id, const double m, const bool fixed=false);
	void addDof(const int id, Direction dir, const double m, const bool fixed=false);

	void setMass(const int id, const double m);
	void setNodeMass(const int id, const double m);

	void mapDofNode(DOF *d, Node *nd);
	void mapDofNode(const int id_d, const int id_nd);

	bool addMaterial1D(Material1D *mt);
	bool addMaterialElastic(const int id, const double E0);
	bool addMaterialElastoplastic(const int id, const double E0, const double fy, const double alpha=0.02);
	bool addMaterialSteelBilinear(const int id, const double E0, const double fy, const double alpha=0.02, const double beta=0.5);
	bool addMaterialConcreteTrilinear(const int id, const double E0, const double fc, const double epsilon_c,
		const double sigma_cr, const double sigma_u, const double epsilon_u);
	bool addMaterialSMABilinear(const int id, const double E0, const double fy, const double alpha, const double sigma_shift);

	bool addFiber(const int id, const int matId, const double A, const double y, const double z=0.0);
	bool addSectionTruss(const int id, int *fiberIds, const int nFibers);
	bool addSectionFrame2D(const int id, int *fiberIds, const int nFibers);

	bool addElement(Element *e);

	void addSpring(Spring *s);
	void addSpring(const int id, const int ni, const int nj, const double k);
	void addSpringBilinear(SpringBilinear *s);
	void addSpringBilinear(const int id, const int ni, const int nj, const double k0, const double uy, const double alpha=0.0);
	void addSpringNonlinear(SpringNonlinear *s);
	void addSpringNonlinear(const int id, const int ni, const int nj, const int matId);
	void addSpringBoucWen(SpringBoucWen *s);
	void addSpringBoucWen(const int id, const int ni, const int nj, const double k0, const double uy, const double alpha=0.0, const double beta = 0.5, const double n=20);
	void addDashpot(Dashpot *d);
	void addDashpot(const int id, const int ni, const int nj, const double c);
	void addDashpotExp(DashpotExp *d);
	void addDashpotExp(const int id, const int ni, const int nj, const double c, const double alpha = 0.1);
	void addDashpotMaxwell(DashpotMaxwell *d);
	void addDashpotMaxwell(const int id, const int ni, const int nj, const double k, const double c, const double alpha = 1.0);
	void addInerter(Inerter *in);
	void addInerter(const int id, const int ni, const int nj, const double m);
	void addSlider(Slider *s);
	void addSlider(const int id, const int ni, const int nj, const double muN);
	void addSPIS2(SPIS2 *s);
	void addSPIS2(const int id, const int ni, const int nj, const int nin, const double m, const double c, const double k);
	void addTVMD(TVMD *d);
	void addTVMD(const int id, const int ni, const int nj, const double m, const double c, const double k);

	void addSpring2D(Spring2D *s);
	void addSpring2D(const int id, const int ni, const int nj, const double k, ELE::localAxis U=ELE::U1);
	void addSpringBoucWen2D(SpringBoucWen2D *s);
	void addSpringBoucWen2D(const int id, const int ni, const int nj, const double k0, const double uy, const double alpha=0.0, const double beta=0.5, const double n=20, ELE::localAxis U = ELE::U1);
	void addDashpot2D(Dashpot2D *s);
	void addDashpot2D(const int id, const int ni, const int nj, const double c, ELE::localAxis U=ELE::U1);
	void addInerter2D(Inerter2D *s);
	void addInerter2D(const int id, const int ni, const int nj, const double m, ELE::localAxis U=ELE::U1);
	void addDashpotExp2D(DashpotExp2D *s);
	void addDashpotExp2D(const int id, const int ni, const int nj, const double c, const double alpha, ELE::localAxis U = ELE::U1);
	void addDashpotMaxwell2D(DashpotMaxwell2D *s);
	void addDashpotMaxwell2D(const int id, const int ni, const int nj, const double k, const double c, const double alpha, ELE::localAxis U = ELE::U1);

	void addTrussElastic(TrussElastic *truss);
	void addTrussElastic(const int id, const int ni, const int nj, const double EA);
	void addBeamElastic(BeamElastic *beam);
	void addBeamElastic(const int id, const int ni, const int nj, const double EI);
	void addFrameElastic(FrameElastic2D *frame);
	void addFrameElastic(const int id, const int ni, const int nj, const double EA, const double EI);
	void addQuad4Elastic(Quad4Elastic *quad);
	void addQuad4Elastic(const int id, const int nodeI, const int nodeJ,
						const int nodeP, const int nodeQ,
						const double E, const double nu, const double t);

	void addTruss2D(const int id, const int ni, const int nj, const int secId);
	void addFrame2D(const int id, const int ni, const int nj, const int secId, const int nIntP=5);
	void addFramePDelta2D(const int id, const int ni, const int nj, const int secId, const int nIntP=5);

	void addTimeSeries(TimeSeries *ts);
	void addTimeSeries(const int id, const double dt, const vec &s);
	void addTimeSeries(const int id, const double dt, char * fileName);

	void addDofRecorder(dofRecorder *dr);
	void addDofRecorder(const int id, int *dofIds, const int n, response rtype, char * fileName);
	void addElementRecorder(elementRecorder *er);
	void addElementRecorder(const int id, int *eleIds, const int n, response rtype, char * fileName);
	void addSpringRecorder(const int id, int *eleIds, const int n, response rtype, char *  fileName);
	void addDashpotRecorder(const int id, int *eleIds, const int n, response rtype, char * fileName);
	void addInerterRecorder(const int id, int *eleIds, const int n, response rtype, char * fileName);

	void setRayleighDamping(const double omg1, const double omg2);
	void activeGroundMotion(Direction dir);
	void buildDofEqnMap();

	/**
	 * @brief      Assembles the system matrices
	 * @note       This function must be called before analysis.
	 */
	void assembleMatrix();

	void assembleMassMatrix();
	void applyRestraint();
	void applyLoad();

	/**
	 * @brief      Sets all the loads constant or not.
	 *
	 * @param[in]  isConst  Indicates if set the loads constant, default to be true
	 */
	void setLoadConst(const bool isConst=true);

	void assembleStiffnessMatrix();
	void reassembleStiffnessMatrix();

	/**
	 * @brief      Sets the number of modes for calculate inherent damping.
	 *
	 * @param[in]  n     number of modes
	 */
	void setNumModesInherentDamping(const int n);
	void buildInherentDampingMatrix();
	void buildRayleighDampingMatrix(const double omg1, const double omg2);
	void buildRayleighDampingMatrix(const int md1, const int md2);
	void assembleDampingMatrix();

	/**
	 * @brief      Solves for eigen values and vectors
	 */
	void solveEigen();
	void solveComplexEigen();

	/**
	 * @brief      Solves the stochastic seismic response
	 *
	 * @param[in]  f_h     The highest frequency for numerical integration
	 * @param[in]  nf      The number of frequencies for numerical integration
	 * @param[in]  method  The solution method: 'c' for complex superposition, 'd' for direct solution
	 * @note       A unit white-noise exitation is assumed at present.
	 */
	void solveStochasticSeismicResponse(const double f_h=50.0, const int nf=10000, const char method='c');

	void solveLinearStaticResponse();
	void solveNonlinearStaticResponse(const int nsub=10);
	void solveNonlinearStaticResponse(const double endTime, const int nsub=10);
	void setDispControlDof(const int dofId, const int loadId);
	void solveNonlinearStaticResponseDispControl(const double loadedTime, const int nsub=10);
	void solveNonlinearStaticResponseDispControlDelta(const double loadedTime, const int nsub=10);

	void setDynamicSolver(dsolver s) { this->dynamicSolver = s; }
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

	/**
	 * @brief      Initializes the recorders.
	 */
	void initRecorders();

	/**
	 * @brief      Records the responses.
	 */
	void recordResponse();

	/**
	 * @brief      Saves the responses.
	 */
	void saveResponse();

	/**
	 * @brief      Prints the basic information of the system.
	 */
	void printInfo();

	/// The map from a DOF identifier to its equation number
	std::map<int, int> dofMapEqn;
	/// The map from a equation number to its DOF identifier
	std::map<int, int> eqnMapDof;
	/// The map from a DOF identifier to its master Node identifier
	std::map<int, int> dofMapNode;

	std::map<int, Node *> Nodes; 			///< Nodes
	std::map<int, Line *> Lines; 			///< Lines
	std::map<int, DOF *> DOFs; 				///< DOFs
	std::map<int, Element *> Elements; 		///< Elements
	std::map<int, Element2D *> Element2Ds; 	///< Element2Ds
	std::map<int, Plane2D *> Plane2Ds; 		///< Plane2Ds

	/// The elements for assembling #Mp
	std::map<int, Element *> physicalMassElements;
	/// The elements for assembling #M other than #Mp
	std::map<int, Element *> inertialMassElements;
	/// The elements for assembling #K0 and #K
	std::map<int, Element *> linearElasticElements;
	/// The elements for assembling #C
	std::map<int, Element *> linearDampingElements;
	/// The elements for assembling #q only
	std::map<int, Element *> nonlinearElements;
	/// The elements for assembling #q and #K
	std::map<int, Element *> nonlinearTangentElements;
	/// The elements for assembling #q and #K0
	std::map<int, Element *> nonlinearSecantElements;
	/// The elements for assembling #q, #K0 and #K
	std::map<int, Element *> nonlinearInitialTangentElements;

	std::map<int, Spring *> Springs; ///< Springs in the system
	std::map<int, SpringBilinear *> SpringBilinears; ///< SpringBilinears in the system
	std::map<int, SpringNonlinear *> SpringNonlinears; ///< SpringNonlinears in the system
	std::map<int, SpringBoucWen *> SpringBoucWens; ///< SpringBoucWens in the system
	std::map<int, Dashpot *> Dashpots; ///< Dashpots in the system
	std::map<int, DashpotExp *> DashpotExps; ///< DashpotExps in the system
	std::map<int, DashpotMaxwell *> DashpotMaxwells; ///< DashpotMaxwells in the system
	std::map<int, Inerter *> Inerters; ///< Inerters in the system
	std::map<int, SPIS2 *> SPIS2s; ///< SPIS2s in the system
	std::map<int, TVMD *> TVMDs; ///< TVMDs in the system
	std::map<int, Slider *> Sliders; ///< Sliders in the system

	std::map<int, Spring2D *> Spring2Ds; ///< Spring2Ds in the system
	std::map<int, SpringBoucWen2D *> SpringBoucWen2Ds; ///< SpringBoucWen2Ds in the system
	std::map<int, Dashpot2D *> Dashpot2Ds; ///< Dashpot2Ds in the system
	std::map<int, DashpotMaxwell2D *> DashpotMaxwell2Ds; ///< DashpotMaxwell2Ds in the system
	std::map<int, DashpotExp2D *> DashpotExp2Ds; ///< DashpotExp2Ds in the system
	std::map<int, Inerter2D *> Inerter2Ds; ///< Inerter2Ds in the system

	std::map<int, TrussElastic *> TrussElastics; ///< TrussElastics in the system
	std::map<int, Truss2D *> Truss2Ds; ///< Truss2Ds in the system
	std::map<int, Frame2D *> Frame2Ds; ///< Frame2Ds in the system
	std::map<int, FramePDelta2D *> FramePDelta2Ds; ///< FramePDelta2Ds in the system
	std::map<int, BeamElastic *> BeamElastics; ///< BeamElastics in the system
	std::map<int, FrameElastic2D *> FrameElastic2Ds; ///< FrameElastic2Ds in the system
	std::map<int, Quad4Elastic *> Quad4Elastics; ///< Quad4Elastics in the system

	std::map<int, Material1D *> Material1Ds; ///< Material1Ds in the system
	std::map<int, Fiber *> Fibers; ///< Fibers in the system
	std::map<int, Section *> Sections; ///< Sections in the system
	std::map<int, SectionTruss *> SectionTrusss; ///< SectionTrusss in the system
	std::map<int, SectionFrame2D *> SectionFrame2Ds; ///< SectionFrame2Ds in the system

	std::map<int, TimeSeries *> Waves; ///< Waves in the system
	std::map<int, Load *> Loads; ///< Loads in the system
	std::map<int, Recorder *> DOFRecorders; ///< DOFRecorders in the system
	std::map<int, Recorder *> ElementRecorders; ///< ElementRecorders in the system

	mat Mp; 		///< The physical mass matrix
	mat K0; 		///< The initial stiffness matrix
	mat K; 			///< The tangent stiffness matrix
	mat C; 			///< The damping matrix
	mat M; 			///< The inertial mass matrix
	mat Phi; 		///< The matrix for storing eigen vectors
	vec omg; 		///< The natural circular frequencies
	vec P; 			///< The natural periods
	vec E; 			///< The ground motion reference vector
	vec Q; 			///< The static load vector
	vec Q0; 		///< The constant static load vector
	vec dsp;		///< The displacement vector
	vec vel;		///< The velocity vector
	vec acc;		///< The acceleration vector
	vec q;			///< The nonlinear force vector
	uvec fixedIds;	///< The identifiers of fixed DOFs
	vec dsp0;		///< The backup of displacement vector

    double zeta;				///< The inherent damping ratio
    int eqnCount;				///< The number of equations
    int fixedDofCount;			///< The number of fixed DOFs
    bool eigenVectorNormed;		///< If the eigen vectors are normed

	dsolver dynamicSolver;		///< The dynamic solver
	double dt;					///< The analysis time interval
	double ctime;				///< The current analysis time
	int nsteps;					///< The total analysis steps
	int cstep;					///< The current analysis step

	bool useRayleighDamping;	///< If use Rayleigh damping
	double RayleighOmg1;		///< The first circular frequency for Rayleigh damping
	double RayleighOmg2;		///< The second circular frequency for Rayleigh damping
	int NumModesInherentDamping;///< The number of modes to calculate inherent damping

	int dispControlDOFId;		///< The identifier of displacement control DOF
	int dispControlLoadId;		///< The identifier of load pattern for displacement control DOF
	int dispControlEqn;			///< The equation number of displacement control DOF
};
