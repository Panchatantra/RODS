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

	friend void to_json(json& jsonObj, const Spring& SpringObj) {
		jsonObj["id"] = SpringObj.id;
		jsonObj["IdDofI"] = SpringObj.dofI->id;
		jsonObj["IdDofJ"] = SpringObj.dofJ->id;
		jsonObj["k"] = SpringObj.k;
	}
	friend void from_json(const json& jsonObj, Spring& SpringObj) {
		jsonObj.at("id").get_to(SpringObj.id);
		jsonObj.at("IdDofI").get_to(SpringObj.IdDofI);
		jsonObj.at("IdDofJ").get_to(SpringObj.IdDofJ);
		jsonObj.at("k").get_to(SpringObj.k);
	};
};

