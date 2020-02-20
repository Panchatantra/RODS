#include "dof.h"

dof::dof(const int id, direction dir) :
    Basis(id), eqnId(0), mass(0.0), dir(dir), isFixed(false),
    dsp(0.0), vel(0.0), acc(0.0)
{
	loads.clear();
}

dof::dof(const int id, direction dir, const double m, const bool fixed) :
	Basis(id), eqnId(0), mass(m), dir(dir), isFixed(fixed),
    dsp(0.0), vel(0.0), acc(0.0)
{
	loads.clear();
}

dof::~dof()
{
}

void dof::setResponse(const double dsp, const double vel, const double acc)
{
	this->dsp = dsp;
	this->vel = vel;
	this->acc = acc;
}

void dof::addLoad(Load* load)
{
	loads.push_back(load);
}

double dof::getLoad(const double time, const bool withConst)
{
	double v = 0.0;
	for (auto it = loads.begin(); it != loads.end(); ++it)
	{
		auto load = (*it);
		if (withConst)
		{
			v += load->getValue(time);
		}
		else
		{
			if (!load->keepConst) v += load->getValue(time);
		}
	}
	return v;
}

double dof::g = 9800.0;