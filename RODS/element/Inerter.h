#pragma once

#include "DOF.h"
#include "ROD1D.h"
#include <armadillo>

using namespace arma;

/**
 * @brief      The inerter in X direction.
 */
class Inerter : public ROD1D
{
public:
	Inerter();
	Inerter(const int id, DOF *i, DOF *j, const double m);
	~Inerter();

	void buildMatrix();
	void assembleMassMatrix(mat & M);
	void getResponse(const bool update = false);

	double m;

	mat::fixed<2,2> M;

	double u, f;

	friend void to_json(json& jsonObj, const Inerter& InerterObj) {
		jsonObj["id"] = InerterObj.id;
		jsonObj["IdDofI"] = InerterObj.dofI->id;
		jsonObj["IdDofJ"] = InerterObj.dofJ->id;
		jsonObj["m"] = InerterObj.m;
	}
	friend void from_json(const json& jsonObj, Inerter& InerterObj) {
		jsonObj.at("id").get_to(InerterObj.id);
		jsonObj.at("IdDofI").get_to(InerterObj.IdDofI);
		jsonObj.at("IdDofJ").get_to(InerterObj.IdDofJ);
		jsonObj.at("m").get_to(InerterObj.m);
	};
};

