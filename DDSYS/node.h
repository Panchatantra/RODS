#pragma once

#include <map>

class node
{
public:
	node(int id, double x=0, double y=0, double z=0);
	~node();

	int id;
	double x, y, z;

};

