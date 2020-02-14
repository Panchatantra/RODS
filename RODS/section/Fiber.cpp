#include "Fiber.h"

Fiber::Fiber() :
	mat1D(nullptr), A(0.0), y(0.0), z(0.0),
	N(0.0), epsilon(0.0), k(0.0),
	nv(1), stress(nullptr), strain(nullptr)
{
}

Fiber::Fiber(const int id, material1D *mat1D, const double A, const double y, const double z):
	Basis(id), mat1D(mat1D->copy()), A(A), y(y), z(z),
	N(0.0), epsilon(0.0), k(mat1D->E*A),
	nv(1), stress(nullptr), strain(nullptr)
{
}


Fiber::~Fiber()
{
}

void Fiber::getResponse(const bool update)
{
	mat1D->setStrain(&epsilon);
	mat1D->getResponse(update);
	N = A*mat1D->sigma;
	k = A*mat1D->E;

	stress = &(mat1D->sigma);
	strain = &epsilon;
}