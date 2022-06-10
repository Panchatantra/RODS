#include "SpringBilinear2D.h"

SpringBilinear2D::SpringBilinear2D(const int id, Node *nodeI, Node *nodeJ, const double k0, const double uy,
									const double alpha, RODS::LocalAxis axis) :
	ROD2D(id, nodeI, nodeJ), k0(k0), uy(uy),
	k1(alpha*k0), fy(k0*uy), alpha(alpha),
	k(k0), ue(0), f(0),
	kp(k0), up(0), dup(0), fp(0),
	status(0), sp(0), axis(axis)
{
	if (axis == RODS::LocalAxis::U1)
	{
		T = rowvec({ -lxx , -lxz , lxx , lxz });
	}
	else
	{
		T = rowvec({ -lzx , -lzz , lzx , lzz });
	}
	buildMatrix();
}

SpringBilinear2D::~SpringBilinear2D()
{
}

void SpringBilinear2D::buildMatrix()
{
	K = T.t()*k*T;
}

void SpringBilinear2D::getResponse(const bool update)
{
	double *u = new double[4] {nodeI->dofX->dsp, nodeI->dofZ->dsp,
							   nodeJ->dofX->dsp, nodeJ->dofZ->dsp};

	ue = 0.0;
	for (size_t i = 0; i < 4; i++)
	{
		ue += T(i)*u[i];
	}

	double du = ue - up;
	double f_try = fp + du * kp;
	double bup = fy + k1 * (ue - uy);
	double bdn = -fy + k1 * (ue + uy);

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

	for (size_t i = 0; i < 4; i++)
	{
		q(i) = T(i)*f;
	}

	force = &f;
	deformation = &ue;

	buildMatrix();

	if (update)
	{
		up = ue;
		dup = du;
		fp = f;
		kp = k;
		sp = status;
	}
}

void SpringBilinear2D::assembleNonlinearForceVector(vec &q)
{
	int local[4] = {0,1,2,3};
	int global[4] = {nodeI->dofX->eqnId, nodeI->dofZ->eqnId,
					 nodeJ->dofX->eqnId, nodeJ->dofZ->eqnId };
	bool fixed[4] = {nodeI->dofX->isFixed, nodeI->dofZ->isFixed,
					 nodeJ->dofX->isFixed, nodeJ->dofZ->isFixed };

	for (int i = 0; i < 4; i++)
	{
		if (!fixed[i])
		{
			q(global[i]) += this->q(local[i]);
		}
	}
}

void SpringBilinear2D::assembleStiffnessMatrix(mat &K)
{
	int local[4] = { 0,1,2,3 };
	int global[4] = { nodeI->dofX->eqnId, nodeI->dofZ->eqnId,
					  nodeJ->dofX->eqnId, nodeJ->dofZ->eqnId };

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			K(global[i], global[j]) += this->K(local[i], local[j]);
		}
	}
}