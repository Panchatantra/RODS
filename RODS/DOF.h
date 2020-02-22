#pragma once
#include "Basis.h"
#include "Load.h"
#include <vector>

using namespace std;

constexpr bool FIXED = true;

/// Direction of DOF in the global coordinate
enum class Direction {
	X, 		///< Direction X
	Y, 		///< Direction Y
	Z, 		///< Direction Z
	RX, 	///< Direction RX
	RY, 	///< Direction RY
	RZ 		///< Direction RZ
	};

/**
 * @brief      The Degree-Of-Freedom Class.
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
	DOF(const int id, Direction dir);

	/**
	 * @brief      Constructs a new instance.
	 *
	 * @param[in]  id     The identifier
	 * @param[in]  dir    The Direction
	 * @param[in]  m      The mass or inertance or moment of inertia
	 * @param[in]  fixed  If the DOF is fixed
	 */
	DOF(const int id, Direction dir, const double m, const bool fixed = false);
	~DOF();

	/**
	 * @brief      Sets the mass.
	 *
	 * @param[in]  m     The mass or inertance or moment of inertia
	 */
	void setMass(const double m);

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

    int eqnId; ///< The identifier of the DOF in system equations
	double mass; ///< mass
	Direction dir; ///< Direction
	vector<Load *> loads; ///< loads
	bool isFixed; ///< if the DOF is fixed

	double dsp; ///< displacement of the DOF
	double vel; ///< velocity of the DOF
	double acc; ///< acceleration of the DOF

	static double g; ///< gravity acceleration
};