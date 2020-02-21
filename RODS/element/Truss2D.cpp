#include "Truss2D.h"

Truss2D::Truss2D(const int id, Node *nodeI, Node *nodeJ, SectionTruss *sec):
	Element2D(id, nodeI, nodeJ), sec(sec->copy()), k(sec->k/L), k0(sec->k/L), ue(0.0), f(0.0)
{
	T = rowvec( {-lxx, -lxy, lxx, lxy} );
	buildMatrix();
	K0 = K;
}

Truss2D::~Truss2D()
{
}

void Truss2D::buildMatrix()
{
	K = T.t()*k*T;
}

void Truss2D::getResponse(const bool update)
{
	u = vec( { nodeI->dofX->dsp, nodeI->dofZ->dsp,
				 nodeJ->dofX->dsp, nodeJ->dofZ->dsp } );

	ue = 0.0;
	for (size_t i = 0; i < 4; i++)
	{
		ue += T(i)*u(i);
	}

	double strain = ue/L;
	sec->setStrain(&strain);
	sec->getResponse(update);

	k = sec->k/L;
	f = sec->f;

	force = &f;
	deformation = &ue;

	buildMatrix();

	double f_ = f - k0*ue;
	for (size_t i = 0; i < 4; i++)
	{
		q(i) = T(i)*f_;
	}
}

void Truss2D::assembleInitialStiffnessMatrix(mat &K0)
{
	int local[4] = { 0,1,2,3 };
	int global[4] = { nodeI->dofX->eqnId, nodeI->dofZ->eqnId,
					  nodeJ->dofX->eqnId, nodeJ->dofZ->eqnId };

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			K0(global[i], global[j]) += this->K0(local[i], local[j]);
		}
	}
}

void Truss2D::assembleStiffnessMatrix(mat &K)
{
	int local[4] = {0,1,2,3};
	int global[4] = {nodeI->dofX->eqnId, nodeI->dofZ->eqnId,
					 nodeJ->dofX->eqnId, nodeJ->dofZ->eqnId};

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			K(global[i], global[j]) += this->K(local[i], local[j]);
		}
	}
}

void Truss2D::assembleNonlinearForceVector(vec &q)
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