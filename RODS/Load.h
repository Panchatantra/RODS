#pragma once
#include "Basis.h"

class Load :
	public Basis
{
public:
	Load();
	Load(const int id, double *t, double *p, const int nP, const double arriveTime=0.0, const double scale=1.0);
	~Load();

	double getValue(const double time);
	void setConst(const bool keepConst=true);
	
	double *t, *p;
	int nP;
	double arriveTime, finishTime;
	double scale;
	bool keepConst;
};

