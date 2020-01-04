#pragma once
#include "element.h"
#include "dof.h"
class element2D :
	public element
{
public:
	element2D();
	~element2D();

	dof *dofIX, *dofIY;
	dof *dofJX, *dofJY;
};

