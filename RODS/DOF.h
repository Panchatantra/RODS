#pragma once
#include "Basis.h"
#include "Load.h"
#include "json.hpp"
#include <vector>

using namespace std;

constexpr bool FIXED = true;

/**
 * The RODS namespace
 */
namespace RODS
{
	/// Direction of DOF in the global coordinate
	enum class Direction {
		X, 		///< Direction X
		Y, 		///< Direction Y
		Z, 		///< Direction Z
		RX, 	///< Direction RX
		RY, 	///< Direction RY
		RZ 		///< Direction RZ
	};
}

/**
 * @brief      The Degree-Of-Freedom.
 * @details    DOF is the basic unit for analysis in RODS.
 */
class DOF : public Basis
{
public:
	/**
	 * @brief      Constructs a new instance.
	 *
	 * @param[in]  id    The identifier
	 * @param[in]  dir   The Direction
	 */
	DOF(const int id, RODS::Direction dir);

	/**
	 * @brief      Constructs a new instance.
	 *
	 * @param[in]  id     The identifier
	 * @param[in]  dir    The Direction
	 * @param[in]  m      The mass or inertance or moment of inertia
	 * @param[in]  fixed  If the DOF is fixed
	 */
	DOF(const int id, RODS::Direction dir, const double m, const bool fixed = false);
	~DOF();

	/**
	 * @brief      Sets the mass.
	 *
	 * @param[in]  m     The mass or inertance or moment of inertia
	 */
	void setMass(const double m);

	void setFixed(const bool isFixed = true);

	/**
	 * @brief      Sets the response.
	 *
	 * @param[in]  dsp   The displacement
	 * @param[in]  vel   The velocity
	 * @param[in]  acc   The acceleration
	 */
	void setResponse(const double dsp, const double vel, const double acc);

	/**
	 * @brief      Sets the response.
	 *
	 * @param[in]  dsp   The displacement
	 */
	void setResponse(const double dsp);

	/**
	 * @brief      Adds a load.
	 *
	 * @param      load  The load
	 */
	void addLoad(Load* load);

	/**
	 * @brief      Gets the load value.
	 *
	 * @param[in]  time       The time
	 * @param[in]  withConst  If considering constant loads
	 *
	 * @return     The load value.
	 */
	double getLoad(const double time, const bool withConst=true);

	void printResponse();

	void setNodeId(const int id) {nodeId = id;}
	void setPointId(const int id) {pointId = id;}

	int nodeId; ///< The identifier of the associated node
	int pointId; ///< The identifier of the associated node
    int eqnId; ///< The identifier of the DOF in system equations
	double mass; ///< mass
	RODS::Direction dir; ///< Direction
	vector<Load *> loads; ///< loads
	bool isFixed; ///< if the DOF is fixed

	double dsp; ///< displacement of the DOF
	double vel; ///< velocity of the DOF
	double acc; ///< acceleration of the DOF

	static double g; ///< gravity acceleration

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(DOF, id, dir, mass, isFixed);
};