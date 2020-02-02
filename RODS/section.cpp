#include "section.h"



section::section() :
	mat(nullptr), A(0.0), nv(1)
{
}

section::section(const int id, material * mat, const double A):
	basis(id), mat(mat->copy()), A(A), nv(1)
{
}


section::~section()
{
}