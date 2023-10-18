#pragma once

#include "ROD3D.h"

/**
 * @brief      The 3D spring.
 */
class Spring3D :
	public ROD3D
{
public:
	Spring3D();
	Spring3D(const int id, Node * nodeI, Node * nodeJ, const double k, RODS::LocalAxis axis);
	~Spring3D();

	virtual void buildMatrix();
	virtual void getResponse();
	virtual void assembleStiffnessMatrix(mat &K);

	double k;
	double ue, f;

	rowvec::fixed<6> T;
	mat::fixed<6, 6> K;

	friend void to_json(json& jsonObj, const Spring3D& Spring3DObj) {
		jsonObj["id"] = Spring3DObj.id;
		jsonObj["IdNodeI"] = Spring3DObj.nodeI->id;
		jsonObj["IdNodeJ"] = Spring3DObj.nodeJ->id;
		jsonObj["k"] = Spring3DObj.k;
		jsonObj["axis"] = Spring3DObj.axis;
	}
	friend void from_json(const json& jsonObj, Spring3D& Spring3DObj) {
		jsonObj.at("id").get_to(Spring3DObj.id);
		jsonObj.at("IdNodeI").get_to(Spring3DObj.IdNodeI);
		jsonObj.at("IdNodeJ").get_to(Spring3DObj.IdNodeJ);
		jsonObj.at("k").get_to(Spring3DObj.k);
		jsonObj.at("axis").get_to(Spring3DObj.axis);
	};
};

