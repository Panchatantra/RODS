#pragma once

#include "ROD2D.h"

/**
 * @brief      The elastic frame element in X-Z plane.
 */
class FrameElastic2D :
	public ROD2D
{
public:
	FrameElastic2D();
	FrameElastic2D(const int id, Node * nodeI, Node * nodeJ, const double EA, const double EI);
	~FrameElastic2D();

	void buildMatrix();
	void getResponse(const bool update = false);
	void assembleStiffnessMatrix(mat &K);

	double EA, EI;
	double *ue, *f;

	mat::fixed<3, 3> k;
	mat::fixed<3, 6> T;
	mat::fixed<6, 6> K;
	vec::fixed<6> u;

	friend void to_json(json& jsonObj, const FrameElastic2D& FrameElastic2DObj) {
		jsonObj["id"] = FrameElastic2DObj.id;
		jsonObj["IdNodeI"] = FrameElastic2DObj.nodeI->id;
		jsonObj["IdNodeJ"] = FrameElastic2DObj.nodeJ->id;
		jsonObj["EA"] = FrameElastic2DObj.EA;
		jsonObj["EI"] = FrameElastic2DObj.EI;
	}
	friend void from_json(const json& jsonObj, FrameElastic2D& FrameElastic2DObj) {
		jsonObj.at("id").get_to(FrameElastic2DObj.id);
		jsonObj.at("IdNodeI").get_to(FrameElastic2DObj.IdNodeI);
		jsonObj.at("IdNodeJ").get_to(FrameElastic2DObj.IdNodeJ);
		jsonObj.at("EA").get_to(FrameElastic2DObj.EA);
		jsonObj.at("EI").get_to(FrameElastic2DObj.EI);
	};
};