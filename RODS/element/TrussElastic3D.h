#pragma once

#include "ROD3D.h"

/**
 * @brief      The elastic 3D truss element.
 */
class TrussElastic3D :
	public ROD3D
{
public:
	TrussElastic3D();
	TrussElastic3D(const int id, Node * nodeI, Node * nodeJ, const double EA);
	~TrussElastic3D();

	void buildMatrix();
	void getResponse(const bool update = false);
	void assembleStiffnessMatrix(mat &K);

	double EA, k;
	double ue, f;

	rowvec::fixed<6> T;
	mat::fixed<6, 6> K;

	friend void to_json(json& jsonObj, const TrussElastic3D& TrussElastic3DObj) {
		jsonObj["id"] = TrussElastic3DObj.id;
		jsonObj["IdNodeI"] = TrussElastic3DObj.nodeI->id;
		jsonObj["IdNodeJ"] = TrussElastic3DObj.nodeJ->id;
		jsonObj["EA"] = TrussElastic3DObj.EA;
	}
	friend void from_json(const json& jsonObj, TrussElastic3D& TrussElastic3DObj) {
		jsonObj.at("id").get_to(TrussElastic3DObj.id);
		jsonObj.at("IdNodeI").get_to(TrussElastic3DObj.IdNodeI);
		jsonObj.at("IdNodeJ").get_to(TrussElastic3DObj.IdNodeJ);
		jsonObj.at("EA").get_to(TrussElastic3DObj.EA);
	};
};