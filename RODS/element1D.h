#pragma once

#include "element.h"
#include "dof.h"

class element1D :
	public element
{
public:
	element1D();
	~element1D();

	dof *dofI;
	dof *dofJ;
};