#include "spring2D.h"

spring2D::spring2D(const int id, node * nodeI, node * nodeJ, double k, ELE::localAxis un) :
	element2D(id, nodeI, nodeJ), k(k)
{
	double dx = nodeJ->x - nodeI->x;
	double dy = nodeJ->y - nodeI->y;
	L = sqrt(dx*dx + dy*dy);

	lxx = dx / L;
	lxy = dy / L;
	lyx = -lxy;
	lyy = lxx;

	if (un == ELE::U1)
	{
		T = rowvec({ -lxx , -lxy , lxx , lxy });
	}
	else
	{
		T = rowvec({ -lyx , -lyy , lyx , lyy });
	}
}


spring2D::~spring2D()
{
}

void spring2D::buildMatrix()
{
	K = T.t()*k*T;
}

void spring2D::getResponse()
{
	u = vec({ nodeI->dofX->dsp, nodeI->dofY->dsp, nodeJ->dofX->dsp, nodeJ->dofY->dsp });

	ue = 0.0;
	for (size_t i = 0; i < 4; i++)
	{
		ue += T(i)*u(i);
	}

	f = k * ue;

	force = &f;
	deformation = &ue;
}
