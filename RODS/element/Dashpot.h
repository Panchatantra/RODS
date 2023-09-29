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

	friend void to_json(json& jsonObj, const Dashpot& DashpotObj) {
		jsonObj["id"] = DashpotObj.id;
		jsonObj["IdDofI"] = DashpotObj.dofI->id;
		jsonObj["IdDofJ"] = DashpotObj.dofJ->id;
		jsonObj["c"] = DashpotObj.c;
	}
	friend void from_json(const json& jsonObj, Dashpot& DashpotObj) {
		jsonObj.at("id").get_to(DashpotObj.id);
		jsonObj.at("IdDofI").get_to(DashpotObj.IdDofI);
		jsonObj.at("IdDofJ").get_to(DashpotObj.IdDofJ);
		jsonObj.at("c").get_to(DashpotObj.c);
	};
};

