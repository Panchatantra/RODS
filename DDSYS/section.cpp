#include "section.h"



section::section() :
	mat(new material()), A(0.0)
{
}

section::section(material * mat, const double A)
{
	this->mat = mat;
	this->A = A;
}


section::~section()
{
}
