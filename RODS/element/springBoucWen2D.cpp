#include "springBoucWen2D.h"

springBoucWen2D::springBoucWen2D(const int id, node * nodeI, node * nodeJ, const double k0, const double uy,
		const double alpha, const double beta, const double n, ELE::localAxis axis) :
	element2D(id, nodeI, nodeJ),
	k0(k0), uy(uy), fy(k0*uy),
	alpha(alpha), beta(beta), n(n),
	k(k0), ue(0.0), f(0.0),
	ve(0.0), z(0.0), zp(0.0),
	axis(axis)
{
	if (axis == ELE::U1)
	{
		T = rowvec({ -lxx , -lxy , lxx , lxy });
	}
	else
	{
		T = rowvec({ -lyx , -lyy , lyx , lyy });
	}
	buildMatrix();
}

springBoucWen2D::~springBoucWen2D()
{
}

void springBoucWen2D::buildMatrix()
{
	K = T.t()*k*T;
}

void springBoucWen2D::getResponse(const bool update)
{
	double *u = new double[4] {nodeI->dofX->dsp, nodeI->dofZ->dsp,
							   nodeJ->dofX->dsp, nodeJ->dofZ->dsp};
	double *v = new double[4] {nodeI->dofX->vel, nodeI->dofZ->vel,
							   nodeJ->dofX->vel, nodeJ->dofZ->vel};

	ue = 0.0;
	double ve = 0.0;
	for (size_t i = 0; i < 4; i++)
	{
		ue += T(i)*u[i];
		ve += T(i)*v[i];
	}

	z = zp;
	f = alpha*k0*ue + (1.0-alpha)*fy*z;
	z += element::dt*k0/fy*(ve-beta*fabs(ve)*pow(fabs(z),(n-1))*z-(1.0-beta)*ve*pow(fabs(z),n));

	k = k0 + (1.0-alpha)*fy*(z-zp)/(ve*element::dt);

	for (size_t i = 0; i < 4; i++)
	{
		q(i) = T(i)*f;
	}

	force = &f;
	deformation = &ue;

	buildMatrix();

	if (update)
	{
		zp = z;
	}
}

void springBoucWen2D::assembleNonlinearForceVector(vec &q)
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

void springBoucWen2D::assembleStiffnessMatrix(mat &K)
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