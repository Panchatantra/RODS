#pragma once

#include "ROD3D.h"

/**
 * @brief      The elastic 3D frame element.
 */
class FrameElastic3D :
	public ROD3D
{
public:
	FrameElastic3D();
	FrameElastic3D(const int id, Node * nodeI, Node * nodeJ, const double EA,
					const double EIy, const double EIz, const double GIp);
	~FrameElastic3D();

	void buildMatrix();
	void getResponse(const bool update = false);
	void assembleStiffnessMatrix(mat &K);

	double EA;
	double EIy;
	double EIz;
	double GIp;

	mat::fixed<6, 6> k;
	mat::fixed<6, 12> T;
	mat::fixed<12, 12> K;
	vec::fixed<12> u;
	vec::fixed<6> ue;
	vec::fixed<6> f;

	friend void to_json(json& jsonObj, const FrameElastic3D& FrameElastic3DObj) {
		jsonObj["id"] = FrameElastic3DObj.id;
		jsonObj["IdNodeI"] = FrameElastic3DObj.nodeI->id;
		jsonObj["IdNodeJ"] = FrameElastic3DObj.nodeJ->id;
		jsonObj["EA"] = FrameElastic3DObj.EA;
		jsonObj["EIy"] = FrameElastic3DObj.EIy;
		jsonObj["EIz"] = FrameElastic3DObj.EIz;
		jsonObj["GIp"] = FrameElastic3DObj.GIp;
	}
	friend void from_json(const json& jsonObj, FrameElastic3D& FrameElastic3DObj) {
		jsonObj.at("id").get_to(FrameElastic3DObj.id);
		jsonObj.at("IdNodeI").get_to(FrameElastic3DObj.IdNodeI);
		jsonObj.at("IdNodeJ").get_to(FrameElastic3DObj.IdNodeJ);
		jsonObj.at("EA").get_to(FrameElastic3DObj.EA);
		jsonObj.at("EIy").get_to(FrameElastic3DObj.EIy);
		jsonObj.at("EIz").get_to(FrameElastic3DObj.EIz);
		jsonObj.at("GIp").get_to(FrameElastic3DObj.GIp);
	};
};