#pragma once

#include "ROD2D.h"
#include <armadillo>

using namespace arma;

/**
 * @brief      The linear dashpot in X-Z plane.
 */
class Dashpot2D : public ROD2D
{
public:
	Dashpot2D();
	Dashpot2D(const int id, Node * nodeI, Node * nodeJ, double c, RODS::LocalAxis axis);
	~Dashpot2D();

	virtual void buildMatrix();
	virtual void assembleDampingMatrix(mat &C);
	virtual void getResponse(const bool update = false);

	double c;
	double ue, f;

	rowvec::fixed<4> T;
	mat::fixed<4, 4> C;

	friend void to_json(json& jsonObj, const Dashpot2D& Dashpot2DObj) {
		jsonObj["id"] = Dashpot2DObj.id;
		jsonObj["IdNodeI"] = Dashpot2DObj.nodeI->id;
		jsonObj["IdNodeJ"] = Dashpot2DObj.nodeJ->id;
		jsonObj["c"] = Dashpot2DObj.c;
		jsonObj["axis"] = Dashpot2DObj.axis;
	}
	friend void from_json(const json& jsonObj, Dashpot2D& Dashpot2DObj) {
		jsonObj.at("id").get_to(Dashpot2DObj.id);
		jsonObj.at("IdNodeI").get_to(Dashpot2DObj.IdNodeI);
		jsonObj.at("IdNodeJ").get_to(Dashpot2DObj.IdNodeJ);
		jsonObj.at("c").get_to(Dashpot2DObj.c);
		jsonObj.at("axis").get_to(Dashpot2DObj.axis);
	};
};

