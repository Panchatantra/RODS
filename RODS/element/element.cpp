#include "element.h"

element::element()
{
	nv = 1;
}

element::element(const int id) : Basis(id)
{
	nv = 1;
}

element::~element()
{
}

double element::dt = 0.0;