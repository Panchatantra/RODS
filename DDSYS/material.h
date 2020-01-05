#pragma once
class material
{
public:
	material();
	material(const int id, const double E);
	~material();

	int id;
	double E;
};

