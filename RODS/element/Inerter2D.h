#pragma once

#include "DOF.h"
#include "ROD2D.h"
#include <armadillo>

using namespace arma;

/**
 * @brief      The inerter in X-Z plane.
 */
class Inerter2D : public ROD2D
{
public:
	Inerter2D();
	Inerter2D(const int id, Node * nodeI, Node * nodeJ, double m, RODS::LocalAxis axis);
	~Inerter2D();

	virtual void buildMatrix();
	virtual void assembleMassMatrix(mat &M);
	virtual void getResponse(const bool update = false);

	double m;

	rowvec::fixed<4> T;
	mat::fixed<4, 4> M;

	double ue, f;

	friend void to_json(json& jsonObj, const Inerter2D& Inerter2DObj) {
		jsonObj["id"] = Inerter2DObj.id;
		jsonObj["IdNodeI"] = Inerter2DObj.nodeI->id;
		jsonObj["IdNodeJ"] = Inerter2DObj.nodeJ->id;
		jsonObj["m"] = Inerter2DObj.m;
		jsonObj["axis"] = Inerter2DObj.axis;
	}
	friend void from_json(const json& jsonObj, Inerter2D& Inerter2DObj) {
		jsonObj.at("id").get_to(Inerter2DObj.id);
		jsonObj.at("IdNodeI").get_to(Inerter2DObj.IdNodeI);
		jsonObj.at("IdNodeJ").get_to(Inerter2DObj.IdNodeJ);
		jsonObj.at("m").get_to(Inerter2DObj.m);
		jsonObj.at("axis").get_to(Inerter2DObj.axis);
	};
};

