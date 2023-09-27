#include "DOF.h"
#include <iostream>

DOF::DOF() :
	Basis(0), eqnId(0), mass(0.0), dir(RODS::Direction::X),
	isFixed(false), nodeId(-1), pointId(-1),
	dsp(0.0), vel(0.0), acc(0.0)
{

}

DOF::DOF(const int id, RODS::Direction dir) :
	Basis(id), eqnId(0), mass(0.0), dir(dir), isFixed(false),
	nodeId(-1), pointId(-1),
	dsp(0.0), vel(0.0), acc(0.0)
{
	loads.clear();
}

DOF::DOF(const int id, RODS::Direction dir, const double m, const bool fixed) :
	Basis(id), eqnId(0), mass(m), dir(dir), isFixed(fixed),
    nodeId(-1), pointId(-1),
	dsp(0.0), vel(0.0), acc(0.0)
{
	loads.clear();
}

DOF::~DOF()
{
}

void DOF::setMass(const double m)
{
	mass = m;
}

void DOF::setFixed(const bool isFixed)
{
	this->isFixed = isFixed;
}

void DOF::setResponse(const double dsp, const double vel, const double acc)
{
	this->dsp = dsp;
	this->vel = vel;
	this->acc = acc;
}

void DOF::setResponse(const double dsp)
{
	this->dsp = dsp;
}

void DOF::addLoad(Load* load)
{
	loads.push_back(load);
}

double DOF::getLoad(const double time, const bool withConst)
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

void DOF::printResponse()
{
	std::cout << "Response of DOF (ID: " << id << "): "<< std::endl;
	std::cout << "Deformation: " << dsp << std::endl;
}

double DOF::g = 9800.0;
