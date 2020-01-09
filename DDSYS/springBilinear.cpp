#include "springBilinear.h"



springBilinear::springBilinear(const int id, dof *i, dof *j, const double k0, const double uy, const double alpha) :
	u(0), f(0), kp(0), up(0), dup(0), fp(0), status(0), sp(0)
{
	this->id = id;
	
	dofI = i;
	dofJ = j;

	this->k0 = k0;
	this->uy = uy;

	k1 = alpha*k;
	fy = k0*uy;
	k = k0;

	buildMatrix();
}


springBilinear::~springBilinear()
{
}

void springBilinear::buildMatrix()
{
	K(0, 0) = k;
	K(0, 1) = -k;
	K(1, 0) = -k;
	K(1, 1) = k;
}

void springBilinear::getResponse(const bool update)
{
	u = dofJ->dsp - dofI->dsp;

	double du = u - up;
	double f_try = fp + du * kp;
	double bup = fy + k1 * (u - uy);
	double bdn = -fy + k1 * (u + uy);

	status = sp;

	if (du == 0.0)
	{	
		k = kp;
		f = fp;
	}
	else if (status == 0)
	{	
		if (du > 0.0)
		{
			if (f_try > bup)
			{	
				f = bup;
				k = k1;
				status = 1;
			}
			else
			{
				k = k0;
				f = f_try;
			}
		}
		else
		{
			if (f_try < bdn)
			{
				f = bdn;
				k = k1;
				status = 1;
			}
			else
			{	
				k = k0;
				f = f_try;
			}
		}
	}
	else if (status == 1)
	{
		if (dup*du > 0)
		{
			f = f_try;
			k = k1;
		}
		else
		{
			k = k0;
			f = fp + k * du;
			status = 0;
			if (f > bup)
			{
				f = bup;
				k = k1;
				status = 1;
			}
			else if (f < bdn)
			{
				f = bdn;
				k = k1;
				status = 1;
			}
		}
	}

	if (update)
	{
		up = u;
		dup = du;
		fp = f;
		kp = k;
		sp = status;
	}
}
