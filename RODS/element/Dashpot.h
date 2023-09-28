#pragma once

#include "DOF.h"
#include "ROD1D.h"
#include <armadillo>

using namespace arma;

/**
 * @brief      The linear dashpot in X direction.
 */
class Dashpot : public ROD1D
{
public:
	Dashpot();
	Dashpot(const int id, DOF *i, DOF *j, const double c);
	~Dashpot();

	void buildMatrix();
	void assembleDampingMatrix(mat & C);
	void getResponse(const bool update = false);

	double c;
	double u, f;

	mat::fixed<2, 2> C;

	friend void to_json(json& jsonObj, const Dashpot& json_t) {
		jsonObj["id"] = json_t.id;
		jsonObj["IdDofI"] = json_t.dofI->id;
		jsonObj["IdDofJ"] = json_t.dofJ->id;
		jsonObj["c"] = json_t.c;
	}
	friend void from_json(const json& jsonObj, Dashpot& json_t) {
		jsonObj.at("id").get_to(json_t.id);
		jsonObj.at("IdDofI").get_to(json_t.IdDofI);
		jsonObj.at("IdDofJ").get_to(json_t.IdDofJ);
		jsonObj.at("c").get_to(json_t.c);
	};
};

