#pragma once

#include "DOF.h"
#include "ROD1D.h"
#include <armadillo>

using namespace arma;

/**
 * @brief      The spring in X direction.
 */
class Spring : public ROD1D
{
public:
	Spring();
	Spring(const int id, DOF *i, DOF *j, const double k);
	~Spring();

	void buildMatrix();
	void assembleStiffnessMatrix(mat &K);
	void getResponse(const bool update = false);

	double k;
	double u, f;

	mat::fixed<2,2> K;

	friend void to_json(nlohmann::json& nlohmann_json_j, const Spring& nlohmann_json_t) {
		nlohmann_json_j["id"] = nlohmann_json_t.id;
		nlohmann_json_j["IdDofI"] = nlohmann_json_t.dofI->id;
		nlohmann_json_j["IdDofJ"] = nlohmann_json_t.dofJ->id;
		nlohmann_json_j["k"] = nlohmann_json_t.k;
	}
	friend void from_json(const nlohmann::json& nlohmann_json_j, Spring& nlohmann_json_t) {
		nlohmann_json_j.at("id").get_to(nlohmann_json_t.id);
		nlohmann_json_j.at("IdDofI").get_to(nlohmann_json_t.IdDofI);
		nlohmann_json_j.at("IdDofJ").get_to(nlohmann_json_t.IdDofJ);
		nlohmann_json_j.at("k").get_to(nlohmann_json_t.k);
	};
};

