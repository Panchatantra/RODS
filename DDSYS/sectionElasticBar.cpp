#include "sectionElasticBar.h"



sectionElasticBar::sectionElasticBar(material * mat, const double A) :
	section(mat, A), Ix(0.0), Iy(0.0)
{
}

sectionElasticBar::sectionElasticBar(material *mat, const double A, const double Ix, const double Iy):
	section(mat, A)
{
	this->Ix = Ix;
	this->Iy = Iy;
}


sectionElasticBar::~sectionElasticBar()
{
}
