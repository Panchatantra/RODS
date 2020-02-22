#pragma once

#include <armadillo>
#include "Basis.h"

using namespace arma;

namespace RODS
{
	/// Response types
	enum class Response {
		DISP, ///< DOF displacement
		VEL, ///< DOF velocity
		ACC, ///< DOF acceleration
		FORCE, ///< Element forces
		DEF, ///< Element deformations
		ALL ///< Element deformations and forces
	};
}

using namespace RODS;

/**
 * @brief      The response recorder.
 */
class Recorder : public Basis
{
public:

	/**
	 * @brief      Constructs a new instance.
	 *
	 * @param[in]  id        The identifier
	 * @param[in]  rtype     The Response type
	 * @param      fileName  The record file name
	 */
	Recorder(const int id, Response rtype, char * fileName);
	~Recorder();

	void init(const int nsteps);
	virtual void record(const int cstep, const double ctime) = 0;
	void save();

	/**
	 * @brief      Sets the record file name.
	 *
	 * @param      fileName  The record file name
	 */
	void setFileName(char *fileName);

	int n; 				///< The number of instances to record
	int nsteps;			///< The number of analysis steps
	Response rtype;		///< The RODS::Response Type
	char * fileName;	///< The record file name
	mat Res;			///< The matrix to record results
};