#pragma once

#include <armadillo>
#include "Basis.h"
#include <string>

using namespace arma;

/**
 * @brief      The wave.
 */
class Wave : public Basis
{
public:
	Wave(const int id, const double dt);
	Wave(const int id, const double dt, const vec &s);
	Wave(const int id, const double dt, char *filePathName);
	Wave(const int id, const double dt, std::string file_path_name);
	~Wave();

	/**
	 * @brief      Loads data from a file
	 *
	 * @param      filePathName  The file name to read data
	 */
	void load(char *filePathName);

	int nsteps;
	double dt;
	vec series;
	std::string name;
};