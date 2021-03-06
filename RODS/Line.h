#pragma once
#include "Basis.h"
#include "Node.h"

/**
 * @brief      The line.
 */
class Line :
	public Basis
{
public:
	Line(const int id, Node *i, Node *j);
	~Line();

	Node *nodeI, *nodeJ;
};

