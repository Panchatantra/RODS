#include "elastic.h"


elastic::elastic(const int id, const double E) :
	material1D(id, E)
{
}

elastic::~elastic()
{
}
