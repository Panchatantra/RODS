#include "section.h"



section::section() :
	mat(nullptr), A(0.0)
{
}

section::section(const int id, material * mat, const double A)
{
	this->id = id;
	this->mat = mat;
	this->A = A;
}


section::~section()
{
}