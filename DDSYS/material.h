#pragma once
class material
{
public:
	material();
	material(const double E, const double nv, const double G);
	~material();

	double E, G, nv;
};

