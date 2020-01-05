#pragma once

#include <map>

class node
{
public:
	node(int id, double x=0, double y=0, double z=0);
	~node();

	int id;
	double x0, y0, z0;
	double x, y, z;

};

