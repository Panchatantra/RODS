#pragma once

#include <armadillo>
#include "Basis.h"

using namespace arma;

/**
 * @brief      The wave.
 */
class Wave : public Basis
{
public:
	Wave(const int id, const double dt);
	Wave(const int id, const double dt, const vec &s);
	Wave(const int id, const double dt, char *fileName);
	~Wave();

	/**
	 * @brief      Loads data from a file
	 *
	 * @param      fileName  The file name to read data
	 */
	void load(char *fileName);

	int nsteps;
	double dt;
	vec series;
};