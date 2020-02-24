#include "Slider2D.h"

Slider2D::Slider2D(const int id, Node * nodeI, Node * nodeJ, const double mu, const double ka) :
	Element2D(id, nodeI, nodeJ), mu(mu), ka(ka)
{
	T = mat({
		{ -lxx , -lxy , lxx , lxy },
		{ -lyx , -lyy , lyx , lyy }
			});
	buildMatrix();
}

Slider2D::~Slider2D()
{
}

void Slider2D::buildMatrix()
{
	mat k;
	if (Element::isStatic)
	{
		k = mat({
			{ka,  0.0},
			{0.0, 1000.0*ka}
		});
	}
	else
	{
		k = mat({
			{ka,  0.0},
			{0.0, 0.0}
		});
	}

	K = T.t()*k*T;
}

void Slider2D::getResponse(const bool update)
{
	vec u = vec( {nodeI->dofX->dsp, nodeI->dofZ->dsp,
				   nodeJ->dofX->dsp, nodeJ->dofZ->dsp});
	vec v = vec( {nodeI->dofX->vel, nodeI->dofZ->vel,
				   nodeJ->dofX->vel, nodeJ->dofZ->vel});

	ue = T*u;
	ve = T*v;

	f(0) = ka*ue(0);
	if (f(0) < 0)
	{
		f(1) = sign(ve(1))*mu*f(0);
	}
	else
	{
		f(1) = 0.0;
	}

	q = T*f;

	force = f.memptr();
	deformation = ue.memptr();

	buildMatrix();
}

void Slider2D::assembleNonlinearForceVector(vec &q)
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

void Slider2D::assembleStiffnessMatrix(mat &K)
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