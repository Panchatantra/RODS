#include "sectionElasticBar.h"



sectionElasticBar::sectionElasticBar(const int id, material * mat, const double A) :
	section(id, mat, A), Ix(0.0), Iy(0.0)
{
}

sectionElasticBar::sectionElasticBar(const int id, material *mat, const double A, const double Ix, const double Iy):
	section(id, mat, A)
{
	this->Ix = Ix;
	this->Iy = Iy;
}


sectionElasticBar::~sectionElasticBar()
{
}
