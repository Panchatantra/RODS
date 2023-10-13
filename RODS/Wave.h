#pragma once

#include <armadillo>
#include "Basis.h"
#include <string>

#include "json.hpp"
using json = nlohmann::json;

using namespace arma;

/**
 * @brief      The wave.
 */
class Wave : public Basis
{
public:
	Wave();
	Wave(const int id, const double dt);
	Wave(const int id, const double dt, const vec &s);
	Wave(const int id, const double dt, const char *filePathName);
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
	std::string filePathName;

	friend void to_json(json& jsonObj, const Wave& WaveObj) {
		jsonObj["id"] = WaveObj.id;
		jsonObj["filePathName"] = WaveObj.filePathName;
		jsonObj["dt"] = WaveObj.dt;
	}

	friend void from_json(const json& jsonObj, Wave& WaveObj) {
		jsonObj.at("id").get_to(WaveObj.id);
		jsonObj.at("filePathName").get_to(WaveObj.filePathName);
		jsonObj.at("dt").get_to(WaveObj.dt);
	};
};