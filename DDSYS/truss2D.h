#pragma once
#include "element2D.h"
#include "node.h"
#include "section.h"
class truss2D :
	public element2D
{
public:
	truss2D(const int id, node * nodeI, node * nodeJ, section *sec);
	~truss2D();

	node * nodeI, * nodeJ;
	section *sec;

	double L;
	double lxx, lxy;

};

