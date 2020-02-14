#pragma once
#include "Basis.h"
#include "node.h"

class line :
	public Basis
{
public:
	line(const int id, node *i, node *j);
	~line();

	node *nodeI, *nodeJ;
};

