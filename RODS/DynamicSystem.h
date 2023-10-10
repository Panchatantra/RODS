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
#include "element/TrussElastic2D.h"
#include "element/BeamElastic2D.h"
#include "element/FrameElastic2D.h"
#include "element/FrameElastic3D.h"
#include "element/Quad4Elastic.h"
#include "element/Tri3Elastic.h"
#include "element/Rect4Elastic.h"
#include "element/Plate4Elastic.h"
#include "element/RectShell4Elastic.h"
#include "element/Truss2D.h"
#include "element/Frame2D.h"
#include "element/FramePDelta2D.h"
#include "Wave.h"
#include "recorder/Recorder.h"
#include "recorder/DOFRecorder.h"
#include "recorder/ElementRecorder.h"
#include "material/Material1D.h"
#include "section/Section.h"
#include "section/Fiber.h"
#include "section/SectionTruss.h"
#include "section/SectionFrame2D.h"
#include "element/TrussElastic3D.h"
#include "element/Spring3D.h"
#include "element/Dashpot3D.h"
#include "element/Inerter3D.h"
#include "element/TVMD2D.h"
#include "element/TVMD3D.h"
#include "element/SpringBilinear2D.h"
#include "element/SpringBilinear3D.h"
#include "element/SpringNonlinear2D.h"
#include "element/SpringNonlinear3D.h"
#include "element/DashpotMaxwell3D.h"

#include "json.hpp"
using json = nlohmann::json;

using namespace arma;

constexpr double PI = 3.14159265;
constexpr bool NORM = true;

namespace RODS {
	/// The dynamic solver
	enum class DynamicSolver
	{
		Newmark, ///< Linear Newmark-β solver
		Newmark_NL, ///< Nonlinear Newmark-β solver with Newton Iteration
		StateSpace, ///< Linear state space solver
		StateSpace_NL ///< Nonlinear state space solver
	};
}

/**
 * @brief      The dynamic system.
 */
class DynamicSystem : public Basis
{
public:
	/**
	 * @brief      Construct function of DynamicSystem.
	 *
	 * @param[in]  z     The inherent damping ratio
	 */
	DynamicSystem(const double z=0.05);
	~DynamicSystem();

	/**
	 * @brief Load model from a json file.
	 *
	 * @param fileName name of the json file
	 */
	void loadFromJSON(const char * fileName);
	void loadFromJSON() {loadFromJSON((workDir+"/"+name+".json").c_str());}

	/**
	 * @brief Save model to a json file.
	 *
	 * @param fileName name of the json file
	 */
	void saveToJSON(const char * fileName);
	void saveToJSON() {saveToJSON((workDir+"/"+name+".json").c_str());}

	/**
	 * @brief Add a Point into the system.
	 *
	 * @param p The pointer to a Point
	 */
	void addPoint(Point *p);

	/**
	 * @brief Add a Point into the system.
	 *
	 * @param pointId The identifier
	 * @param x The X coordinate
	 * @param y The Y coordinate, default value 0.0
	 * @param z The Z coordinate, default value 0.0
	 */
	void addPoint(const int pointId, const double x, const double y=0.0, const double z=0.0);


	/**
	 * @brief Add a Node into the system.
	 *
	 * @param nd The pointer to a Node
	 */
	void addNode(Node *nd);
	void removeNode(const int id);

	/**
	 * @brief      Add a 1D Node into the system.
	 *
	 * @param[in]  nodeId     The identifier
	 * @param[in]  x      The X coordinate
	 * @param[in]  dofId  The DOF identifier
	 */
	void addNode(const int nodeId, const double x, const int dofId);
	void addNode(const int nodeId, const int pointId, const int dofId);
	void addNode(const int nodeId, const double x);

	void addNode(const int nodeId, const int pointId,
				const RODS::Dimension dim);

	/**
	 * @brief      Add a 2D Node into the system.
	 *
	 * @param[in]  nodeId       The identifier
	 * @param[in]  x        The X coordinate
	 * @param[in]  z        The Z coordinate
	 * @param[in]  dofXId   The DOF X identifier
	 * @param[in]  dofZId   The DOF Z identifier
	 * @param[in]  dofRYId  The DOF RY identifier, set a negative integer to deactive the rotational DOF
	 */
	void addNode(const int nodeId, const double x, const double z,
				const int dofXId, const int dofZId, const int dofRYId);
	void addNode(const int nodeId, const int pointId,
				const int dofXId, const int dofZId, const int dofRYId);
	void addNode(const int nodeId, const double x, const double z,
				bool with_rotate_dof = true);

	/**
	 * @brief      Add a 3D Node into the system.
	 *
	 * @param[in]  nodeId       The identifier
	 * @param[in]  x        The X coordinate
	 * @param[in]  y        The Y coordinate
	 * @param[in]  z        The Z coordinate
	 * @param[in]  dofXId   The DOF X identifier
	 * @param[in]  dofYId   The DOF Y identifier
	 * @param[in]  dofZId   The DOF Z identifier
	 * @param[in]  dofRXId  The DOF RX identifier, set a negative integer to deactive the rotational DOF
	 * @param[in]  dofRYId  The DOF RY identifier, set a negative integer to deactive the rotational DOF
	 * @param[in]  dofRZId  The DOF RZ identifier, set a negative integer to deactive the rotational DOF
	 */
	void addNode(const int nodeId, const double x, const double y, const double z,
				const int dofXId,  const int dofYId,  const int dofZId,
				const int dofRXId, const int dofRYId, const int dofRZId);
	void addNode(const int nodeId, const int pointId,
				const int dofXId,  const int dofYId,  const int dofZId,
				const int dofRXId, const int dofRYId, const int dofRZId);
	void addNode(const int nodeId, const double x, const double y, const double z,
				bool with_rotate_dof = true);

	/**
	* @brief      Add a 2D Node (for 2D plate elements) into the system.
	*
	* @param[in]  nodeId       The identifier
	* @param[in]  x        The X coordinate
	* @param[in]  y        The Y coordinate
	* @param[in]  dofZId   The DOF Z identifier
	* @param[in]  dofRXId  The DOF RX identifier
	* @param[in]  dofRYId  The DOF RY identifier
	*/
	void addNodePlate2D(const int nodeId, const double x, const double y, const int dofZId, const int dofRXId, const int dofRYId);


	void addLine(Line *l);
	void addLine(const int id, const int ni, const int nj);

	/**
	 * @brief      Fix a DOF.
	 *
	 * @param[in]  dofId    The DOF identifier
	 */
	void fixDOF(const int dofId);

	/**
	 * @brief      Free a DOF.
	 *
	 * @param[in]  dofId    The DOF identifier
	 */
	void freeDOF(const int dofId);

	/**
	 * @brief      Fix all the DOFs of a Node.
	 *
	 * @param[in]  nodeId    The Node identifier
	 */
	void fixNode(const int nodeId);

	/**
	 * @brief Free all the DOFs of a Node.
	 * 
	 * @param nodeId    The Node identifier
	 */
	void freeNode(const int nodeId);

	/**
	 * @brief      Fix a DOF of a Node.
	 *
	 * @param[in]  nodeId  The node identifier
	 * @param[in]  dir     The Direction to be fixed
	 */
	void fixNode(const int nodeId, RODS::Direction dir);

	/**
	 * @brief      Free a DOF of a Node.
	 * 
	 * @param nodeId  The node identifier
	 * @param dir     The Direction to be free
	 */
	void freeNode(const int nodeId, RODS::Direction dir);

	/**
	 * @brief      Add a load into the system.
	 *
	 * @param[in]  id          The Load identifier
	 * @param      t           The loading moments
	 * @param      p           The loading values related to the moments
	 * @param[in]  nP          The number of loading moments
	 * @param[in]  arriveTime  The arrive time of the load, default to 0.0
	 * @param[in]  scale       The scale factor of the load, default to 1.0
	 */
	void addLoad(const int id, double *t, double *p, const int nP, const double arriveTime=0.0, const double scale=1.0);
	void addLoad(const int id, const double arriveTime=0.0, const double scale=1.0);
	void addLoadValue(const int id, const double time, const double value);

	/**
	 * @brief      Add a Load to a DOF.
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

	void exportModalGmsh(char * fileName, const int order=1);

	void exportResponseGmsh(char * fileName);

	void exportResponseGmsh();

	void setResponseGmsh(char * fileName, const int interval);

	void getDofModalResponse(double *res, const int order=1);

	void getNodeModalResponse(double *res, const int order=1);

    /**
     * @brief Add a DOF into the system.
     * 
     * @param d The pointer to the DOF
     */
	void addDOF(DOF *d);

    /**
     * @brief      Add a DOF (X Direction) into the system.
     *
     * @param[in]  id     The identifier
     * @param[in]  m      The mass of the DOF
     * @param[in]  fixed  Indicates if the DOF is fixed
     */
	void addDOF(const int id, const double m=0.0, const bool fixed=false);

	/**
	 * @brief      Add a DOF into the system.
	 *
	 * @param[in]  id     The identifier
	 * @param[in]  dir    The Direction
	 * @param[in]  m      The mass of the DOF
	 * @param[in]  fixed  Indicates if the DOF is fixed
	 */
	void addDOF(const int id, RODS::Direction dir, const double m=0.0, const bool fixed=false);

	void removeDOF(const int id);
	int getNextDofId();

	/**
	 * @brief      Sets the mass of a DOF.
	 *
	 * @param[in]  id    The identifier of the DOF
	 * @param[in]  m     The mass
	 */
	void setMass(const int id, const double m);

	/**
	 * @brief      Sets the mass of all DOFs of a node.
	 *
	 * @param[in]  id    The identifier of the Node
	 * @param[in]  m     The mass
	 */
	void setNodeMass(const int id, const double m);

	/**
	 * @brief      Sets the mass and moment of inertia of all DOFs of a node.
	 *
	 * @param[in]  id    The identifier of the Node
	 * @param[in]  m     The mass
	 * @param[in]  J     The moment of inertia
	 */
	void setNodeMass(const int id, const double m, const double J);

	void mapDofNode(DOF *d, Node *nd);
	void mapDofNode(const int id_d, const int id_nd);

	bool checkDuplicateMaterial1D(const int matId);
	void addMaterialElastic(const int id, const double E0);

	/**
	 * @brief      Add a elastoplastic material.
	 *
	 * @param[in]  id     The identifier
	 * @param[in]  E0     The initial tangent module
	 * @param[in]  fy     The yield strength
	 * @param[in]  alpha  The post-yield tangent module ratio
	 */
	void addMaterialElastoplastic(const int id, const double E0, const double fy, const double alpha=0.02);

	/**
	 * @brief      Add a bilinear steel material.
	 *
	 * @param[in]  id     The identifier
	 * @param[in]  E0     The initial tangent module
	 * @param[in]  fy     The yield strength
	 * @param[in]  alpha  The post-yield tangent module ratio
	 * @param[in]  beta   The recovery stress ratio
	 */
	void addMaterialSteelBilinear(const int id, const double E0, const double fy, const double alpha=0.02, const double beta=0.5);

	/**
	 * @brief      Add a trilinear concrete material.
	 *
	 * @param[in]  id         The identifier
	 * @param[in]  E0         The initial tangent module
	 * @param[in]  fc         The compressive strength
	 * @param[in]  epsilon_c  The corresponding strain of compressive strength
	 * @param[in]  sigma_cr   The compressive crack stress
	 * @param[in]  sigma_u    The compressive crush stress
	 * @param[in]  epsilon_u  The compressive crush strain
	 */
	void addMaterialConcreteTrilinear(const int id, const double E0, const double fc, const double epsilon_c,
		const double sigma_cr, const double sigma_u, const double epsilon_u);

	/**
	 * @brief      Add a bilinear SMA material.
	 *
	 * @param[in]  id           The identifier
	 * @param[in]  E0           The initial tangent module
	 * @param[in]  fy           The yield strength
	 * @param[in]  alpha        The post-yield tangent module ratio
	 * @param[in]  sigma_shift  The shift stress during unloading
	 */
	void addMaterialSMABilinear(const int id, const double E0, const double fy, const double alpha, const double sigma_shift);


	void addMaterialCyclicHardenTrilinear(const int id, const double E, const double sigma1, const double alpha1,
											const double sigma2, const double alpha2);

	/**
	 * @brief      Add a fiber.
	 *
	 * @param[in]  id     The identifier
	 * @param[in]  matId  The material identifier
	 * @param[in]  A      The area
	 * @param[in]  y      The y coordinate
	 * @param[in]  z      The z coordinate
	 */
	void addFiber(const int id, const int matId, const double A, const double y, const double z=0.0);

	/**
	 * @brief      Add a fiber-based truss section.
	 *
	 * @param[in]  id        The identifier
	 * @param      fiberIds  The fiber identifiers
	 * @param[in]  nFibers   The number of fibers
	 */
	void addSectionTruss(const int id, int *fiberIds, const int nFibers);

	/**
	 * @brief      Add a fiber-based frame section.
	 *
	 * @param[in]  id        The identifier
	 * @param      fiberIds  The fiber identifiers
	 * @param[in]  nFibers   The number of fibers
	 */
	void addSectionFrame2D(const int id, int *fiberIds, const int nFibers);

	bool addElement(Element *e);

	bool checkDuplicateElement(const int eleId);

	/**
	 * @brief      Add a spring.
	 *
	 * @param[in]  id    The identifier
	 * @param[in]  ni    The identifier of DOF i
	 * @param[in]  nj    The identifier of DOF j
	 * @param[in]  k     The stiffness
	 */
	void addSpring(const int id, const int ni, const int nj, const double k);
	void removeSpring(const int id);
	/**
	 * @brief      Add a bilinear spring.
	 *
	 * @param[in]  id     The identifier
	 * @param[in]  ni     The identifier of DOF i
	 * @param[in]  nj     The identifier of DOF j
	 * @param[in]  k0     The initial stiffness
	 * @param[in]  uy     The yield deformation
	 * @param[in]  alpha  The post-yield stiffness ratio
	 */
	void addSpringBilinear(const int id, const int ni, const int nj, const double k0, const double uy, const double alpha=0.0);

	/**
	 * @brief      Add a nonlinear spring related to a nonlinear Material1D.
	 *
	 * @param[in]  id     The identifier
	 * @param[in]  ni     The identifier of DOF i
	 * @param[in]  nj     The identifier of DOF j
	 * @param[in]  matId  The Material1D identifier
	 */
	void addSpringNonlinear(const int id, const int ni, const int nj, const int matId);
	void addSpringNonlinear2D(const int id, const int ni, const int nj, const int matId, RODS::LocalAxis U=RODS::LocalAxis::U1);
	void addSpringNonlinear3D(const int id, const int ni, const int nj, const int matId, RODS::LocalAxis U=RODS::LocalAxis::U1);


	/**
	 * @brief      Add a Bouc-Wen spring.
	 *
	 * @param[in]  id     The identifier
	 * @param[in]  ni     The identifier of DOF i
	 * @param[in]  nj     The identifier of DOF j
	 * @param[in]  k0     The initial stiffness
	 * @param[in]  uy     The yield deformation
	 * @param[in]  alpha  The post-yield stiffness ratio
	 * @param[in]  beta   The beta
	 * @param[in]  n      The n
	 */
	void addSpringBoucWen(const int id, const int ni, const int nj, const double k0, const double uy, const double alpha=0.0, const double beta = 0.5, const double n=20);

	/**
	 * @brief      Add a dashpot.
	 *
	 * @param[in]  id    The identifier
	 * @param[in]  ni    The identifier of DOF i
	 * @param[in]  nj    The identifier of DOF j
	 * @param[in]  c     The damping coefficient
	 */
	void addDashpot(const int id, const int ni, const int nj, const double c);
	void removeDashpot(const int id);
	/**
	 * @brief      Add a nonlinear dashpot.
	 *
	 * @param[in]  id     The identifier
	 * @param[in]  ni     The identifier of DOF i
	 * @param[in]  nj     The identifier of DOF j
	 * @param[in]  c      The damping coefficient
	 * @param[in]  alpha  The damping exponent
	 */
	void addDashpotExp(const int id, const int ni, const int nj, const double c, const double alpha = 0.1);

	void addDashpotMaxwell(const int id, const int ni, const int nj, const double k, const double c, const double alpha = 1.0);

	void addInerter(const int id, const int i, const int j, const double m);
	void removeInerter(const int id);

	void addSlider(const int id, const int i, const int j, const double muN);

	void addSPIS2(const int id, const int i, const int j, const int in, const double m, const double c, const double k);

	void addTVMD(const int id, const int ni, const int nj, const double m, const double c, const double k);
	void addTVMD2D(const int id, const int ni, const int nj, const double m, const double c, const double k, RODS::LocalAxis U=RODS::LocalAxis::U1);
	void addTVMD3D(const int id, const int ni, const int nj, const double m, const double c, const double k, RODS::LocalAxis U=RODS::LocalAxis::U1);

	/**
	 * @brief      Add a Spring2D.
	 *
	 * @param[in]  id    The identifier
	 * @param[in]  ni    The identifier of Node i
	 * @param[in]  nj    The identifier of Node j
	 * @param[in]  k     The stiffness
	 * @param[in]  U     The RODS::LocalAxis
	 */
	void addSpring2D(const int id, const int ni, const int nj, const double k, RODS::LocalAxis U=RODS::LocalAxis::U1);

	/**
	* @brief      Add a Spring3D.
	*
	* @param[in]  id    The identifier
	* @param[in]  ni    The identifier of Node i
	* @param[in]  nj    The identifier of Node j
	* @param[in]  k     The stiffness
	* @param[in]  U     The RODS::LocalAxis
	*/
	void addSpring3D(const int id, const int ni, const int nj, const double k, RODS::LocalAxis U=RODS::LocalAxis::U1);

	/**
	 * @brief      Add a SpringBoucWen2D.
	 *
	 * @param[in]  id     The identifier
	 * @param[in]  ni     The identifier of Node i
	 * @param[in]  nj     The identifier of Node j
	 * @param[in]  k0     The initial stiffness
	 * @param[in]  uy     The yield deformation
	 * @param[in]  alpha  The post-yield stiffness ratio
	 * @param[in]  beta   The beta
	 * @param[in]  n      The n
	 * @param[in]  U      The RODS::LocalAxis
	 */
	void addSpringBoucWen2D(const int id, const int ni, const int nj, const double k0, const double uy, const double alpha=0.0, const double beta=0.5, const double n=20, RODS::LocalAxis U = RODS::LocalAxis::U1);

	/**
	 * @brief      Add a Dashpot2D.
	 *
	 * @param[in]  id    The identifier
	 * @param[in]  ni    The identifier of Node i
	 * @param[in]  nj    The identifier of Node j
	 * @param[in]  c     The damping coefficient
	 * @param[in]  U     The RODS::LocalAxis
	 */
	void addDashpot2D(const int id, const int ni, const int nj, const double c, RODS::LocalAxis U=RODS::LocalAxis::U1);

	/**
	 * @brief      Add a Inerter2D.
	 *
	 * @param[in]  id    The identifier
	 * @param[in]  ni    The identifier of Node i
	 * @param[in]  nj    The identifier of Node j
	 * @param[in]  m     The inertance
	 * @param[in]  U     The RODS::LocalAxis
	 */
	void addInerter2D(const int id, const int ni, const int nj, const double m, RODS::LocalAxis U=RODS::LocalAxis::U1);

	void addSpringBilinear2D(const int id, const int ni, const int nj, const double k0, const double uy, const double alpha=0.0, RODS::LocalAxis U=RODS::LocalAxis::U1);

	void addSpringBilinear3D(const int id, const int ni, const int nj, const double k0, const double uy, const double alpha=0.0, RODS::LocalAxis U=RODS::LocalAxis::U1);

	/**
	* @brief      Add a Dashpot3D.
	*
	* @param[in]  id    The identifier
	* @param[in]  ni    The identifier of Node i
	* @param[in]  nj    The identifier of Node j
	* @param[in]  c     The damping coefficient
	* @param[in]  U     The RODS::LocalAxis
	*/
	void addDashpot3D(const int id, const int ni, const int nj, const double c, RODS::LocalAxis U=RODS::LocalAxis::U1);

	/**
	* @brief      Add a Inerter3D.
	*
	* @param[in]  id    The identifier
	* @param[in]  ni    The identifier of Node i
	* @param[in]  nj    The identifier of Node j
	* @param[in]  m     The inertance
	* @param[in]  U     The RODS::LocalAxis
	*/
	void addInerter3D(const int id, const int ni, const int nj, const double m, RODS::LocalAxis U=RODS::LocalAxis::U1);


	/**
	 * @brief      Add a DashpotExp2D.
	 *
	 * @param[in]  id     The identifier
	 * @param[in]  ni     The identifier of Node i
	 * @param[in]  nj     The identifier of Node j
	 * @param[in]  c      The damping coefficient
	 * @param[in]  alpha  The damping exponent
	 * @param[in]  U      The RODS::LocalAxis
	 */
	void addDashpotExp2D(const int id, const int ni, const int nj, const double c, const double alpha, RODS::LocalAxis U = RODS::LocalAxis::U1);

	/**
	 * @brief      Add a DashpotMaxwell2D.
	 *
	 * @param[in]  id     The identifier
	 * @param[in]  ni     The identifier of Node i
	 * @param[in]  nj     The identifier of Node j
	 * @param[in]  k      The stiffness
	 * @param[in]  c      The damping coefficient
	 * @param[in]  alpha  The damping exponent
	 * @param[in]  U      The RODS::LocalAxis
	 */
	void addDashpotMaxwell2D(const int id, const int ni, const int nj, const double k, const double c, const double alpha, RODS::LocalAxis U = RODS::LocalAxis::U1);
	void addDashpotMaxwell3D(const int id, const int ni, const int nj, const double k, const double c, const double alpha, RODS::LocalAxis U = RODS::LocalAxis::U1);

	/**
	 * @brief      Add a TrussElastic2D element.
	 *
	 * @param[in]  id    The identifier
	 * @param[in]  ni    The identifier of Node i
	 * @param[in]  nj    The identifier of Node j
	 * @param[in]  EA    E*A of the truss section
	 */
	void addTrussElastic2D(const int id, const int ni, const int nj, const double EA);

	/**
	* @brief      Add a TrussElastic3D element.
	*
	* @param[in]  id    The identifier
	* @param[in]  ni    The identifier of Node i
	* @param[in]  nj    The identifier of Node j
	* @param[in]  EA    E*A of the truss section
	*/
	void addTrussElastic3D(const int id, const int ni, const int nj, const double EA);

	/**
	 * @brief      Add a BeamElastic2D element.
	 *
	 * @param[in]  id    The identifier
	 * @param[in]  ni    The identifier of Node i
	 * @param[in]  nj    The identifier of Node j
	 * @param[in]  EI    E*I of the beam section
	 */
	void addBeamElastic2D(const int id, const int ni, const int nj, const double EI);

	/**
	 * @brief      Add a FrameElastic2D element.
	 *
	 * @param[in]  id    The identifier
	 * @param[in]  ni    The identifier of Node i
	 * @param[in]  nj    The identifier of Node j
	 * @param[in]  EA    E*A of the frame section
	 * @param[in]  EI    E*I of the frame section
	 */
	void addFrameElastic2D(const int id, const int ni, const int nj, const double EA, const double EI);

	/**
	* @brief      Add a FrameElastic3D element.
	*
	* @param[in]  id    The identifier
	* @param[in]  ni    The identifier of Node i
	* @param[in]  nj    The identifier of Node j
	* @param[in]  EA    E*A of the frame section
	* @param[in]  EIy    E*Iy of the frame section
	* @param[in]  EIz    E*Iz of the frame section
	* @param[in]  GIp    G*Ip of the frame section
	*/
	void addFrameElastic3D(const int id, const int ni, const int nj, const double EA,
						const double EIy, const double EIz, const double GIp);

	/**
	 * @brief      Add a Tri3Elastic element.
	 *
	 * @param[in]  id     The identifier
	 * @param[in]  nodeI  The identifier of node I
	 * @param[in]  nodeJ  The identifier of node J
	 * @param[in]  nodeP  The identifier of node P
	 * @param[in]  E      The elastic module
	 * @param[in]  nu     The Poisson ratio
	 * @param[in]  t      The thickness
	 */
	void addTri3Elastic(const int id, const int nodeI, const int nodeJ,
						const int nodeP,
						const double E, const double nu, const double t);

	/**
	* @brief      Add a Rect4Elastic element.
	*
	* @param[in]  id     The identifier
	* @param[in]  nodeI  The identifier of node I
	* @param[in]  nodeJ  The identifier of node J
	* @param[in]  nodeP  The identifier of node P
	* @param[in]  nodeQ  The identifier of node Q
	* @param[in]  E      The elastic module
	* @param[in]  nu     The Poisson ratio
	* @param[in]  t      The thickness
	*/
	void addRect4Elastic(const int id, const int nodeI, const int nodeJ,
		const int nodeP, const int nodeQ,
		const double E, const double nu, const double t);

	/**
	* @brief      Add a Plate4Elastic element.
	*
	* @param[in]  id     The identifier
	* @param[in]  nodeI  The identifier of node I
	* @param[in]  nodeJ  The identifier of node J
	* @param[in]  nodeP  The identifier of node P
	* @param[in]  nodeQ  The identifier of node Q
	* @param[in]  E      The elastic module
	* @param[in]  nu     The Poisson ratio
	* @param[in]  t      The thickness
	*/
	void addPlate4Elastic(const int id, const int nodeI, const int nodeJ,
		const int nodeP, const int nodeQ,
		const double E, const double nu, const double t);

	/**
	 * @brief      Add a Quad4Elastic element.
	 *
	 * @param[in]  id     The identifier
	 * @param[in]  nodeI  The identifier of node I
	 * @param[in]  nodeJ  The identifier of node J
	 * @param[in]  nodeP  The identifier of node P
	 * @param[in]  nodeQ  The identifier of node Q
	 * @param[in]  E      The elastic module
	 * @param[in]  nu     The Poisson ratio
	 * @param[in]  t      The thickness
	 */
	void addQuad4Elastic(const int id, const int nodeI, const int nodeJ,
						const int nodeP, const int nodeQ,
						const double E, const double nu, const double t);

	/**
	* @brief      Add a RectShell4Elastic element.
	*
	* @param[in]  id     The identifier
	* @param[in]  nodeI  The identifier of node I
	* @param[in]  nodeJ  The identifier of node J
	* @param[in]  nodeP  The identifier of node P
	* @param[in]  nodeQ  The identifier of node Q
	* @param[in]  E      The elastic module
	* @param[in]  nu     The Poisson ratio
	* @param[in]  t      The thickness
	*/
	void addRectShell4Elastic(const int id, const int nodeI, const int nodeJ,
		const int nodeP, const int nodeQ,
		const double E, const double nu, const double t);

	/**
	 * @brief      Add a nonlinear Truss2D element.
	 *
	 * @param[in]  id     The identifier
	 * @param[in]  ni     The identifier of Node i
	 * @param[in]  nj     The identifier of Node j
	 * @param[in]  secId  The identifier of SectionTruss
	 */
	void addTruss2D(const int id, const int ni, const int nj, const int secId);

	/**
	 * @brief      Add a nonlinear Frame2D element.
	 *
	 * @param[in]  id     The identifier
	 * @param[in]  ni     The identifier of Node i
	 * @param[in]  nj     The identifier of Node j
	 * @param[in]  secId  The identifier of SectionFrame2D
	 * @param[in]  nIntP  The number of Gauss integration points
	 */
	void addFrame2D(const int id, const int ni, const int nj, const int secId, const int nIntP=2);

	/**
	 * @brief      Add a nonlinear FramePDelta2D element.
	 *
	 * @param[in]  id     The identifier
	 * @param[in]  ni     The identifier of Node i
	 * @param[in]  nj     The identifier of Node j
	 * @param[in]  secId  The identifier of SectionFrame2D
	 * @param[in]  nIntP  The number of Gauss integration points
	 */
	void addFramePDelta2D(const int id, const int ni, const int nj, const int secId, const int nIntP=2);

	/**
	 * @brief      Add a wave from a armadillo vector.
	 *
	 * @param[in]  id    The identifier
	 * @param[in]  dt    The time interval
	 * @param[in]  s     The time series
	 */
	void addWave(const int id, const double dt, const vec &s);

	/**
	 * @brief      Add a wave from a file (one column data file).
	 *
	 * @param[in]  id        The identifier
	 * @param[in]  dt        The time interval
	 * @param      fileName  The file name
	 */
	void addWave(const int id, const double dt, const char * fileName);

	/**
	 * @brief      Add a DOF Recorder.
	 *
	 * @param[in]  id        The identifier
	 * @param      dofIds    The DOF identifiers
	 * @param[in]  n         The number of DOFs
	 * @param[in]  rType     The Response type
	 * @param      fileName  The record file name
	 */
	void addDOFRecorder(const int id, int *dofIds, const int n, RODS::Response rType, const char * fileName);

	/**
	* @brief      Add a DOF Recorder.
	*
	* @param[in]  id        The identifier
	* @param      dofIds    The DOF identifiers
	* @param[in]  n         The number of DOFs
	* @param[in]  rType     The Response type
	* @param      fileName  The record file name
	*/
	void addDOFRecorder(const int id, RODS::Response rType, const char * fileName);

	void addDOFToRecorder(const int dofId, const int rId);

	/**
	 * @brief      Add a Element Recorder.
	 *
	 * @param[in]  id        The identifier
	 * @param      dofIds    The Element identifiers
	 * @param[in]  n         The number of Elements
	 * @param[in]  rType     The Response type
	 * @param      fileName  The record file name
	 */
	void addElementRecorder(const int id, int *eleIds, const int n, RODS::Response rType, const char * fileName);

	void addElementRecorder(const int id, RODS::Response rType, const char * fileName);

	void addElementToRecorder(const int eleId, const int rId);

	void setDofRecorderFileName(const int id, char * fileName);
	void setElementRecorderFileName(const int id, char * fileName);

	/**
	 * @brief      Sets the circular frequencies for Rayleigh damping.
	 *
	 * @param[in]  omg1  The first circular frequency for Rayleigh damping
	 * @param[in]  omg2  The second circular frequency for Rayleigh damping
	 */
	void setRayleighDamping(const double omg1, const double omg2);

	/**
	 * @brief      Sets the orders of modes for Rayleigh damping.
	 *
	 * @param[in]  i  The first order of mode for Rayleigh damping
	 * @param[in]  j  The second order of mode for Rayleigh damping
	 */
	void setRayleighDamping(const int i, const int j);

	/**
	 * @brief      Actives Ground Motion
	 *
	 * @param[in]  dir        The Direction of Ground Motion
	 * @param[in]  waveId     The wave identifier
	 * @param[in]  waveScale  The wave scale factor
	 */
	void activeGroundMotion(RODS::Direction dir, const int waveId, const double waveScale);

	void buildDofEqnMap();

	/**
	 * @brief      Assembles the system matrices and applies restraints and static loads
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

	void recordAllDofResponse(const int id);
	void recordAllEleResponse(const int id);

	/**
	 * @brief      Solves the stochastic seismic response
	 *
	 * @param[in]  f_h     The highest frequency for numerical integration
	 * @param[in]  nf      The number of frequencies for numerical integration
	 * @param[in]  method  The solution method: 'c' for complex superposition, 'd' for direct solution
	 * @note       A unit white-noise exitation is assumed at present.
	 */
	void solveStochasticSeismicResponse(const double f_h=50.0, const int nf=10000, const char method='c');

	/**
	 * @brief      Sets the converge parameters.
	 *
	 * @param[in]  tol      ///< The tolerance for convergence check
	 * @param[in]  maxIter  ///< The maximum iterations before converged
	 */
	void setConvergeParameter(const double tol, const int maxIter);

	/**
	 * @brief      Solves linear static response.
	 */
	void solveLinearStaticResponse();

	/**
	 * @brief      Solves nonlinear static response.
	 *
	 * @param[in]  nsub  The number of substeps
	 * @note        The loaded time is 1.0
	 */
	void solveNonlinearStaticResponse(const int nsub=10);

	/**
	 * @brief      Solves nonlinear static response.
	 *
	 * @param[in]  loadedTime  The loaded time
	 * @param[in]  nsub     The number of substeps
	 */
	void solveNonlinearStaticResponse(const double loadedTime, const int nsub=10);

	/**
	 * @brief      Sets the displacement control DOF and the corresponding load pattern.
	 *
	 * @param[in]  dofId   The DOF identifier
	 * @param[in]  loadId  The load pattern identifier
	 */
	void setDispControlDof(const int dofId, const int loadId);

	/**
	 * @brief      Solves nonlinear static response using displacement control loading strategy.
	 *
	 * @param[in]  loadedTime  The loaded time
	 * @param[in]  nsub        The number of substeps
	 */
	void solveNonlinearStaticResponseDispControl(const double loadedTime, const int nsub=10);

	/**
	 * @brief      Solves nonlinear static response using displacement control loading strategy.
	 *
	 * @param[in]  loadedTime  The loaded time
	 * @param[in]  nsub        The number of substeps
	 * @note       This function has a better converge performance than #solveNonlinearStaticResponseDispControl.
	 */
	void solveNonlinearStaticResponseDispControlDelta(const double loadedTime, const int nsub=10);

	/**
	 * @brief      Sets the dynamic solver.
	 *
	 * @param[in]  s     The solver
	 */
	void setDynamicSolver(RODS::DynamicSolver s) { this->dynamicSolver = s; }

	/**
	 * @brief      Solves seismic response.
	 *
	 * @param[in]  nsub  The number of substeps
	 */
	void solveSeismicResponse(const int nsub=1);
	void solveSeismicResponseNewmark(const int nsub=1);
	void solveSeismicResponseNewmarkNL(const int nsub=1);
	void solveSeismicResponseStateSpace(const int nsub=1);
	void solveSeismicResponseStateSpaceNL(const int nsub=1);
	void solveSeismicResponseRK4(const int nsub=1);

	void solveSeismicResponseNewmarkMD(const int nsub=1);
	void solveSeismicResponseNewmarkNLMD(const int nsub=1);
	void solveSeismicResponseStateSpaceMD(const int nsub=1);
	void solveSeismicResponseStateSpaceNLMD(const int nsub=1);

	void setDofResponse();
	void setDofStaticResponse();

	/**
	 * @brief      Gets the element responses.
	 */
	void getElementResponse();

	/**
	 * @brief      Assembles nonlinear force vector.
	 *
	 * @param[in]  update  If update the history variables
	 */
	void assembleNonlinearForceVector(const bool update=false);

	/**
	 * @brief      Initializes the recorders.
	 */
	void initRecorders();

	/**
	 * @brief      Records the responses.
	 */
	void recordResponse();

	/**
	 * @brief      Saves the responses when analysis is finished.
	 */
	void saveResponse();

	/**
	 * @brief      Prints the basic information of the system.
	 */
	void printInfo();

	void setCurrentTime(double ctime);
	void setName(const char * name);
	void setWorkDir(const char * dir);

	void exportMassMatrix(const char* fileName) {M.save(fileName, raw_ascii);}
	void exportStiffMatrix(const char* fileName) {K.save(fileName, raw_ascii);}
	void exportDampMatrix(const char* fileName) {C.save(fileName, raw_ascii);}
	void exportPeriodVector(const char* fileName) {P.save(fileName, raw_ascii);}
	void exportModalMatrix(const char* fileName) {Phi.save(fileName, raw_ascii);}

	/// The map from a DOF identifier to its equation number
	std::map<int, int> dofMapEqn;
	/// The map from a equation number to its DOF identifier
	std::map<int, int> eqnMapDof;
	/// The map from a DOF identifier to its master Node identifier
	std::map<int, int> dofMapNode;

	std::map<int, Node *> Nodes; 			///< Nodes
	std::map<int, Point *> Points; 			///< Points
	std::map<int, Line *> Lines; 			///< Lines
	std::map<int, DOF *> DOFs; 				///< DOFs
	std::map<int, Element *> Elements; 		///< Elements
	std::map<int, ROD1D *> ROD1Ds; 			///< ROD1Ds
	std::map<int, ROD2D *> ROD2Ds; 			///< ROD2Ds
	std::map<int, Quad2D *> Quad2Ds; 		///< Quad2Ds
	std::map<int, Tri2D *> Tri2Ds;    		///< Tri2Ds
	std::map<int, ROD3D *> ROD3Ds; 			///< ROD3Ds

	std::map<int, DOF*> DOFXs; 				///< DOFs
	std::map<int, DOF*> DOFYs; 				///< DOFs
	std::map<int, DOF*> DOFZs; 				///< DOFs
	std::map<int, DOF*> DOFRXs; 			///< DOFs
	std::map<int, DOF*> DOFRYs; 			///< DOFs
	std::map<int, DOF*> DOFRZs; 			///< DOFs

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
	std::map<int, SpringBilinear2D *> SpringBilinear2Ds; ///< SpringBilinears in the system
	std::map<int, SpringBoucWen2D *> SpringBoucWen2Ds; ///< SpringBoucWen2Ds in the system
	std::map<int, Dashpot2D *> Dashpot2Ds; ///< Dashpot2Ds in the system
	std::map<int, DashpotMaxwell2D *> DashpotMaxwell2Ds; ///< DashpotMaxwell2Ds in the system
	std::map<int, DashpotExp2D *> DashpotExp2Ds; ///< DashpotExp2Ds in the system
	std::map<int, Inerter2D *> Inerter2Ds; ///< Inerter2Ds in the system
	std::map<int, TVMD2D *> TVMD2Ds; ///< TVMD2Ds in the system
	std::map<int, SpringNonlinear2D *> SpringNonlinear2Ds; ///< SpringNonlinear2Ds in the system

	std::map<int, Spring3D *> Spring3Ds; ///< Spring3Ds in the system
	std::map<int, Dashpot3D *> Dashpot3Ds; ///< Dashpot3Ds in the system
	std::map<int, Inerter3D *> Inerter3Ds; ///< Inerter3Ds in the system
	std::map<int, TVMD3D *> TVMD3Ds; ///< TVMD3Ds in the system
	std::map<int, DashpotMaxwell3D *> DashpotMaxwell3Ds; ///< DashpotMaxwell3Ds in the system
	std::map<int, SpringBilinear3D *> SpringBilinear3Ds; ///< SpringBilinear3Ds in the system
	std::map<int, SpringNonlinear3D *> SpringNonlinear3Ds; ///< SpringNonlinear3Ds in the system

	std::map<int, TrussElastic2D *> TrussElastic2Ds; ///< TrussElastic2Ds in the system
	std::map<int, Truss2D *> Truss2Ds; ///< Truss2Ds in the system
	std::map<int, Frame2D *> Frame2Ds; ///< Frame2Ds in the system
	std::map<int, FramePDelta2D *> FramePDelta2Ds; ///< FramePDelta2Ds in the system
	std::map<int, BeamElastic2D *> BeamElastic2Ds; ///< BeamElastic2Ds in the system
	std::map<int, FrameElastic2D *> FrameElastic2Ds; ///< FrameElastic2Ds in the system
	std::map<int, FrameElastic3D *> FrameElastic3Ds; ///< FrameElastic3Ds in the system
	std::map<int, TrussElastic3D *> TrussElastic3Ds; ///< FrameElastic3Ds in the system
	std::map<int, Quad4Elastic *> Quad4Elastics; ///< Quad4Elastics in the system
	std::map<int, Tri3Elastic *> Tri3Elastics; ///< Tri3Elastics in the system
	std::map<int, Rect4Elastic *> Rect4Elastics; ///< Rect4Elastics in the system
	std::map<int, Plate4Elastic *> Plate4Elastics; ///< Plate4Elastics in the system
	std::map<int, RectShell4Elastic *> RectShell4Elastics; ///< RectShell4Elastics in the system

	std::map<int, Material1D *> Material1Ds; ///< Material1Ds in the system
	std::map<int, Fiber *> Fibers; ///< Fibers in the system
	std::map<int, Section *> Sections; ///< Sections in the system
	std::map<int, SectionTruss *> SectionTrusss; ///< SectionTrusss in the system
	std::map<int, SectionFrame2D *> SectionFrame2Ds; ///< SectionFrame2Ds in the system

	std::map<int, Wave *> Waves; ///< Waves in the system
	std::map<int, Load *> Loads; ///< Loads in the system
	std::map<int, DOFRecorder *> DOFRecorders; ///< DOFRecorders in the system
	std::map<int, ElementRecorder *> ElementRecorders; ///< ElementRecorders in the system

	mat Mp; 		///< The physical mass matrix
	mat K0; 		///< The initial stiffness matrix
	mat K; 			///< The tangent stiffness matrix
	mat C; 			///< The damping matrix
	mat M; 			///< The inertial mass matrix
	mat Phi; 		///< The matrix for storing eigen vectors
	vec omg; 		///< The natural circular frequencies
	vec P; 			///< The natural periods
	vec EX; 		///< The ground motion reference vector (X direction)
	vec EY; 		///< The ground motion reference vector (Y direction)
	vec EZ; 		///< The ground motion reference vector (Z direction)
	vec Q; 			///< The static load vector
	vec Q0; 		///< The constant static load vector
	vec dsp;		///< The displacement vector
	vec vel;		///< The velocity vector
	vec acc;		///< The acceleration vector
	vec q;			///< The nonlinear force vector
	uvec fixedIds;	///< The identifiers of fixed DOFs
	vec dsp0;		///< The backup of displacement vector
	vec p;			///< The dynamic load vector

	string name;    ///< name of the model
	string workDir;	///< the working directory

    double zeta;				///< The inherent damping ratio
    int eqnCount;				///< The number of equations
    size_t dofCount;			///< The number of DOFs
    int fixedDofCount;			///< The number of fixed DOFs
    bool eigenVectorNormed;		///< If the eigen vectors are normed

	RODS::DynamicSolver dynamicSolver;		///< The dynamic solver
	double dt;					///< The analysis time interval
	double ctime;				///< The current analysis time
	int nsteps;					///< The total analysis steps
	int cstep;					///< The current analysis step

	bool useRayleighDamping;	///< If use Rayleigh damping
	double RayleighOmg1;		///< The first circular frequency for Rayleigh damping
	double RayleighOmg2;		///< The second circular frequency for Rayleigh damping
	int NumModesInherentDamping;///< The number of modes to calculate inherent damping

	int XSeismicWaveId;			///< The identifier of seismic wave in X direction
	int YSeismicWaveId;			///< The identifier of seismic wave in Y direction
	int ZSeismicWaveId;			///< The identifier of seismic wave in Z direction

	double XSeismicWaveScale;	///< The scale factor of seismic wave in X direction
	double YSeismicWaveScale;	///< The scale factor of seismic wave in Y direction
	double ZSeismicWaveScale;	///< The scale factor of seismic wave in Z direction
	int NumDynamicSubSteps;		///< Number of substeps in an analysis step

	int dispControlDOFId;		///< The identifier of displacement control DOF
	int dispControlLoadId;		///< The identifier of load pattern for displacement control DOF
	int dispControlEqn;			///< The equation number of displacement control DOF

	double tol;  ///< The tolerance for convergence check
	size_t maxIter; ///< The maximum iterations before converged

	string gmshFileName;
	std::ofstream gmshFile;
	int exportGmshInterval;

	double xMax, xMin;
	double yMax, yMin;
	double zMax, zMin;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(DynamicSystem,
		name,
		workDir,
		zeta,
		eqnCount,
		fixedDofCount,
		eigenVectorNormed,
		dynamicSolver,
		dt,
		ctime,
		nsteps,
		cstep,
		useRayleighDamping,
		RayleighOmg1,
		RayleighOmg2,
		NumModesInherentDamping,
		XSeismicWaveId,
		YSeismicWaveId,
		ZSeismicWaveId,
		XSeismicWaveScale,
		YSeismicWaveScale,
		ZSeismicWaveScale,
		NumDynamicSubSteps,
		dispControlDOFId,
		dispControlLoadId,
		dispControlEqn,
		tol,
		maxIter,
		exportGmshInterval,
		xMax,
		yMax,
		zMax,
		xMin,
		yMin,
		zMin
		);
};