#pragma once
#include "Basis.h"
#include "Node.h"

class Line :
	public Basis
{
public:
	Line(const int id, Node *i, Node *j);
	~Line();

	Node *nodeI, *nodeJ;
};

