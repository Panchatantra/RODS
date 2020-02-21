#include "Load.h"

Load::Load()
{
}

Load::Load(const int id, double *t, double *p, const int nP, const double arriveTime, const double scale):
	Basis(id), t(t), p(p), nP(nP), arriveTime(arriveTime), finishTime(arriveTime+t[nP-1]),
	scale(scale), keepConst(false)
{
}


Load::~Load()
{
}

double Load::getValue(const double time)
{
	double v = 0.0;
	if (time == arriveTime)
	{
		v = p[0];
	}
	else if (time < arriveTime)
	{
		v = 0.0;
	}
	else if (time > finishTime)
	{
		if (keepConst)
		{
			v = p[nP-1];
		}
		else
		{
			v = 0.0;
		}	
	}
	else
	{
		int i = 0;
		int j = 0;

		for (int q = 0; q < nP; ++q)
		{
			if (t[q] >= time)
			{
				i = q - 1;
				j = i + 1;
				break;
			}
		}
		v = p[i] + (p[j] - p[i])/(t[j] - t[i])*(time - t[i]);
	}
	return v*scale;
}

void Load::setConst(const bool keepConst)
{
	this->keepConst = keepConst;
}
