#pragma once

#include "ROD2D.h"
#include "Node.h"

/**
 * @brief      The spring in X-Z plane.
 */
class Spring2D :
	public ROD2D
{
public:
	Spring2D();
	Spring2D(const int id, Node * nodeI, Node * nodeJ, const double k, RODS::LocalAxis axis);
	~Spring2D();

	virtual void buildMatrix();
	virtual void getResponse();
	virtual void assembleStiffnessMatrix(mat &K);

	vec::fixed<4> u;

	double k;
	double ue, f;

	rowvec::fixed<4> T;
	mat::fixed<4, 4> K;

	friend void to_json(json& jsonObj, const Spring2D& Spring2DObj) {
		jsonObj["id"] = Spring2DObj.id;
		jsonObj["IdNodeI"] = Spring2DObj.nodeI->id;
		jsonObj["IdNodeJ"] = Spring2DObj.nodeJ->id;
		jsonObj["k"] = Spring2DObj.k;
		jsonObj["axis"] = Spring2DObj.axis;
	}
	friend void from_json(const json& jsonObj, Spring2D& Spring2DObj) {
		jsonObj.at("id").get_to(Spring2DObj.id);
		jsonObj.at("IdNodeI").get_to(Spring2DObj.IdNodeI);
		jsonObj.at("IdNodeJ").get_to(Spring2DObj.IdNodeJ);
		jsonObj.at("k").get_to(Spring2DObj.k);
		jsonObj.at("axis").get_to(Spring2DObj.axis);
	};
};

