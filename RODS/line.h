#pragma once
#include "basis.h"
#include "node.h"

class line :
	public basis
{
public:
	line(const int id, node *i, node *j);
	~line();

	node *nodeI, *nodeJ;
};

