#pragma once

#include "ROD2D.h"

/**
 * @brief      The elastic truss element in X-Z plane.
 */
class TrussElastic2D :
	public ROD2D
{
public:
	TrussElastic2D();
	TrussElastic2D(const int id, Node * nodeI, Node * nodeJ, const double EA);
	~TrussElastic2D();

	void buildMatrix();
	void getResponse(const bool update = false);
	void assembleStiffnessMatrix(mat &K);

	vec::fixed<4> u;

	double EA, k;
	double ue, f;

	rowvec::fixed<4> T;
	mat::fixed<4, 4> K;

	friend void to_json(json& jsonObj, const TrussElastic2D& TrussElastic2DObj) {
		jsonObj["id"] = TrussElastic2DObj.id;
		jsonObj["IdNodeI"] = TrussElastic2DObj.nodeI->id;
		jsonObj["IdNodeJ"] = TrussElastic2DObj.nodeJ->id;
		jsonObj["EA"] = TrussElastic2DObj.EA;
	}
	friend void from_json(const json& jsonObj, TrussElastic2D& TrussElastic2DObj) {
		jsonObj.at("id").get_to(TrussElastic2DObj.id);
		jsonObj.at("IdNodeI").get_to(TrussElastic2DObj.IdNodeI);
		jsonObj.at("IdNodeJ").get_to(TrussElastic2DObj.IdNodeJ);
		jsonObj.at("EA").get_to(TrussElastic2DObj.EA);
	};
};