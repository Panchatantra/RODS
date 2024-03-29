#include "SpringBoucWen.h"

SpringBoucWen::SpringBoucWen(const int id, DOF *i, DOF *j, const double k0, const double uy, const double alpha, const double beta, const double n) :
	ROD1D(id, i, j),
	k0(k0), uy(uy),k1(alpha*k0), fy(k0*uy),
	alpha(alpha), beta(beta), n(n),
	k(k0), u(0), f(0), v(0), z(0), zp(0)
{
	buildMatrix();
}

SpringBoucWen::~SpringBoucWen()
{
}

void SpringBoucWen::buildMatrix()
{
	K(0, 0) = k;
	K(0, 1) = -k;
	K(1, 0) = -k;
	K(1, 1) = k;
}

void SpringBoucWen::assembleStiffnessMatrix(mat & K)
{
	int i_local = 0;
	int j_local = 1;

	int i_global = dofI->eqnId;
	int j_global = dofJ->eqnId;

	K(i_global, i_global) += this->K(i_local, i_local);
	K(i_global, j_global) += this->K(i_local, j_local);
	K(j_global, i_global) += this->K(j_local, i_local);
	K(j_global, j_global) += this->K(j_local, j_local);
}

void SpringBoucWen::getResponse(const bool update)
{
	u = dofJ->dsp - dofI->dsp;
	v = dofJ->vel - dofI->vel;

	z = zp;
	f = alpha*k0*u + (1.0-alpha)*fy*z;
	z += Element::dt*k0/fy*(v-beta*fabs(v)*pow(fabs(z),(n-1))*z-(1.0-beta)*v*pow(fabs(z),n));

	k = k0 + (1.0-alpha)*fy*(z-zp)/(v*Element::dt);

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

void SpringBoucWen::assembleNonlinearForceVector(vec & q)
{
	int i_local = 0;
	int j_local = 1;

	if (dofI->isFixed)
	{
		int j_global = dofJ->eqnId;
		q(j_global) += this->q(j_local);
	}
	else if (dofJ->isFixed)
	{
		int i_global = dofI->eqnId;
		q(i_global) += this->q(i_local);
	}
	else
	{
		int i_global = dofI->eqnId;
		int j_global = dofJ->eqnId;

		q(i_global) += this->q(i_local);
		q(j_global) += this->q(j_local);
	}
}