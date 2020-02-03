#include "springBoucWen.h"


springBoucWen::springBoucWen(const int id, dof *i, dof *j, const double k0, const double uy, const double alpha, const double beta, const double n) :
	element1D(id, i, j), k0(k0), uy(uy), alpha(alpha), beta(beta), n(n),
	k1(alpha*k0), fy(k0*uy), k(k0),
	u(0), f(0), v(0), z(0), zp(0)
{
	buildMatrix();
}


springBoucWen::~springBoucWen()
{
}

void springBoucWen::buildMatrix()
{
	K(0, 0) = k;
	K(0, 1) = -k;
	K(1, 0) = -k;
	K(1, 1) = k;
}

void springBoucWen::assembleStiffnessMatrix(mat & K)
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

void springBoucWen::getResponse(const bool update)
{
	u = dofJ->dsp - dofI->dsp;
	v = dofJ->vel - dofI->vel;

	double dt = this->dt;

	z = zp;
	f = alpha*k0*u + (1.0-alpha)*fy*z;
	z += dt*k0/fy*(v-beta*fabs(v)*pow(fabs(z),(n-1))*z-(1.0-beta)*v*pow(fabs(z),n));

	//q(0) = -(f - k*u);
	q(0) = -f;
	q(1) = f;

	force = &f;
	deformation = &u;

	buildMatrix();

	if (update)
	{
		zp = z;
	}
}

void springBoucWen::assembleNonlinearForceVector(vec & q)
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
