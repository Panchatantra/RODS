#pragma once
#include "element2D.h"
#include "node.h"
class spring2D :
	public element2D
{
public:
	spring2D(const int id, node * nodeI, node * nodeJ, double k1, double k2);
	~spring2D();

	void buildMatrix();
	void getResponse();

	node *nodeI, *nodeJ;

	double k1, k2;
	double L;
	double lxx, lxy, lyx, lyy;

	mat::fixed<2, 2> D;
	mat::fixed<2, 4> T;
	mat::fixed<4, 4> K;
};

