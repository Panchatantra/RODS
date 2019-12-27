#include "location.h"


location::location(const int id, const double x, const double y, const double z)
{
	this->id = id;
	this->x = x;
	this->y = y;
	this->z = z;

	this->x0 = x;
	this->y0 = y;
	this->z0 = z;

}


location::~location()
{
}
