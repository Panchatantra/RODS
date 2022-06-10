#pragma once

#include "DOF.h"
#include "ROD1D.h"
#include <armadillo>

using namespace arma;

/**
 * @brief      The slider in X direction.
 */
class Slider : public ROD1D
{
public:
	/**
	 * @brief      Constructs a new instance.
	 *
	 * @param[in]  id    The identifier
	 * @param      i     The DOF i
	 * @param      j     The DOF j
	 * @param[in]  muN   The friction force
	 */
	Slider(const int id, DOF *i, DOF *j, const double muN);
	~Slider();

	void getResponse(const bool update=false);
	void assembleNonlinearForceVector(vec &q);

	double muN; ///< The friction force
	double u; 	///< The deformation
	double f; 	///< The force
	double v; 	///< The velocity

	vec::fixed<2> q; ///< The nonlinear force vector
};

