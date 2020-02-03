#include "sectionElasticBar.h"



sectionElasticBar::sectionElasticBar(const int id, material * mat, const double A) :
	section(id, mat, A), Ix(0.0), Iy(0.0), eps(0.0), kpx(0.0), kpy(0.0), N(0.0), Mx(0.0), My(0.0)
{
	nv = 3;
}

sectionElasticBar::sectionElasticBar(const int id, material *mat, const double A, const double Ix, const double Iy):
	section(id, mat, A), Ix(Ix), Iy(Iy), eps(0.0), kpx(0.0), kpy(0.0), N(0.0), Mx(0.0), My(0.0)
{
	nv = 3;
}


sectionElasticBar::~sectionElasticBar()
{
}

void sectionElasticBar::setStrain(const double *strain)
{
	eps = strain[0];
	kpx = strain[1];
	kpy = strain[2];
}

void sectionElasticBar::getResponse(const bool update)
{
	N = mat->E*A*eps;
	Mx = mat->E*Ix*kpx;
	My = mat->E*Iy*kpy;
}
