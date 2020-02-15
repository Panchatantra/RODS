#include "Section.h"

Section::Section() :
	nv(1), strain(nullptr), force(nullptr)
{
}

Section::Section(const int id):
	Basis(id), nv(1), strain(nullptr), force(nullptr)
{
}

Section::~Section()
{
}