#pragma once

#include "Element3D.h"

/**
 * @brief      The elastic 3D frame element.
 */
class FrameElastic3D :
	public Element3D
{
public:
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
};