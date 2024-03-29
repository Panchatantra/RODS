#include "SpringBilinear.h"

SpringBilinear::SpringBilinear() :
	ROD1D(0, nullptr, nullptr), k0(0.0), uy(0.0),
	k1(0.0), fy(0.0), alpha(0.0),
	k(0.0), u(0), f(0),
	kp(0.0), up(0), dup(0), fp(0),
	status(0), sp(0)
{
}

SpringBilinear::SpringBilinear(const int id, DOF *i, DOF *j,
					const double k0, const double uy, const double alpha) :
					ROD1D(id, i, j), k0(k0), uy(uy),
					k1(alpha * k0), fy(k0 * uy), alpha(alpha),
					k(k0), u(0), f(0),
					kp(k0), up(0), dup(0), fp(0),
					status(0), sp(0)
{
	buildMatrix();
}


SpringBilinear::~SpringBilinear()
{
}

void SpringBilinear::buildMatrix()
{
	K(0, 0) = k;
	K(0, 1) = -k;
	K(1, 0) = -k;
	K(1, 1) = k;
}

void SpringBilinear::assembleStiffnessMatrix(mat & K)
{
	int i_local = 0;
	int j_local = 1;

	int i_global = this->dofI->eqnId;
	int j_global = this->dofJ->eqnId;

	K(i_global, i_global) += this->K(i_local, i_local);
	K(i_global, j_global) += this->K(i_local, j_local);
	K(j_global, i_global) += this->K(j_local, i_local);
	K(j_global, j_global) += this->K(j_local, j_local);
}

void SpringBilinear::getResponse(const bool update)
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

	//q(0) = -(f - k*u);
	q(0) = -f;
	q(1) = f;

	force = &f;
	deformation = &u;

	buildMatrix();

	if (update)
	{
		up = u;
		dup = du;
		fp = f;
		kp = k;
		sp = status;
	}
}

void SpringBilinear::assembleNonlinearForceVector(vec & q)
{
	int i_local = 0;
	int j_local = 1;

	if (this->dofI->isFixed)
	{
		int j_global = this->dofJ->eqnId;
		q(j_global) += this->q(j_local);
	}
	else if (this->dofJ->isFixed)
	{
		int i_global = this->dofI->eqnId;
		q(i_global) += this->q(i_local);
	}
	else
	{
		int i_global = this->dofI->eqnId;
		int j_global = this->dofJ->eqnId;

		q(i_global) += this->q(i_local);
		q(j_global) += this->q(j_local);
	}
}