#include "FramePDelta2D.h"
#include "numeric.h"

FramePDelta2D::FramePDelta2D(const int id, Node *nodeI, Node *nodeJ, SectionFrame2D *sec, const int nIntPoints):
	ROD2D(id, nodeI, nodeJ), B(zeros<mat>(2, 3)),
	k(zeros<mat>(3, 3)), k0(zeros<mat>(3, 3)),
	xi(new double[nIntPoints]), wt(new double[nIntPoints])
{
	gaussInt(nIntPoints, xi, wt);
	for (int i = 0; i < nIntPoints; ++i)
	{
		xi[i] = 0.5*(xi[i] + 1.0);
		wt[i] = 0.5*wt[i];
		sections.push_back(sec->copy());
		B(0,0) = 1.0;
        B(1,1) = 6.*xi[i]-4.0;
        B(1,2) = 6.*xi[i]-2.0;
        k += wt[i]/L*B.t()*sec->k0*B;
	}

	k0 = k;

	T = mat({
				{-lxx,   -lxz,   0.0,  lxx,   lxz,   0.0},
				{-lzx/L, -lzz/L, 1.0, lzx/L,  lzz/L, 0.0},
				{-lzx/L, -lzz/L, 0.0, lzx/L,  lzz/L, 1.0}
			});

	Kg = 1.0/30.0/L*mat(
		{
			{0,0,0,0,0,0},
			{0,36,3*L,0,-36,3*L},
			{0,3*L,4*L*L,0,-3*L,-L*L},
			{0,0,0,0,0,0},
			{0,-36,-3*L,0,36,-3*L},
			{0,3*L,-L*L,0,-3*L,4*L*L}
		}
	);

	Tg = mat(
		{
			{lxx,lxz,0,0,0,0},
			{lzx,lzz,0,0,0,0},
			{0,0,1,0,0,0},
			{0,0,0,lxx,lxz,0},
			{0,0,0,lzx,lzz,0},
			{0,0,0,0,0,1}
		}
	);

	buildMatrix();

	K0 = K;
	nv = 3;
}

FramePDelta2D::~FramePDelta2D()
{
}

void FramePDelta2D::buildMatrix()
{
	K = T.t()*k*T;
}

void FramePDelta2D::getResponse(const bool update)
{
	u = vec( { nodeI->dofX->dsp, nodeI->dofZ->dsp, nodeI->dofRY->dsp,
		       nodeJ->dofX->dsp, nodeJ->dofZ->dsp, nodeJ->dofRY->dsp } );

	ue = T*u;
	f.fill(0.0);
	k.fill(0.0);

	SectionFrame2D *section = nullptr;
	mat::fixed<2,2> D;

	double x;
    double *sectionStrain = new double[2];
    sectionStrain[0] = ue(0)/L;

	int i = 0;
	for (auto it=sections.begin(); it!=sections.end(); ++it)
	{
		section = *it;
		x = xi[i]*L;
    	sectionStrain[1] = -1.0/(L*L)*((4.0*L-6.0*x)*ue(1)+(2.0*L-6.0*x)*ue(2));

		section->setStrain(sectionStrain);
		section->getResponse(update);

		B(0,0) = 1.0;
		B(1,1) = 6.0*xi[i]-4.0;
		B(1,2) = 6.0*xi[i]-2.0;
		D = section->k;
        k += wt[i]/L*B.t()*D*B;
        f(0) += wt[i]*section->f(0);
        f(1) += wt[i]*(xi[i]*6.0-4.0)*section->f(1);
        f(2) += wt[i]*(xi[i]*6.0-2.0)*section->f(1);
		i++;
	}

	force = f.memptr();
	deformation = ue.memptr();

	buildMatrix();

	q = T.t()*(f - k0*ue) + f(0)*Kg*Tg*u;
}

void FramePDelta2D::assembleStiffnessMatrix(mat &K)
{
	int local[6] = {0,1,2,3,4,5};
	int global[6] = {nodeI->dofX->eqnId, nodeI->dofZ->eqnId, nodeI->dofRY->eqnId,
					 nodeJ->dofX->eqnId, nodeJ->dofZ->eqnId, nodeJ->dofRY->eqnId };

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			K(global[i], global[j]) += this->K(local[i], local[j]);
		}
	}
}

void FramePDelta2D::assembleInitialStiffnessMatrix(mat &K0)
{
	int local[6] = {0,1,2,3,4,5};
	int global[6] = {nodeI->dofX->eqnId, nodeI->dofZ->eqnId, nodeI->dofRY->eqnId,
					 nodeJ->dofX->eqnId, nodeJ->dofZ->eqnId, nodeJ->dofRY->eqnId };

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			K0(global[i], global[j]) += this->K0(local[i], local[j]);
		}
	}
}

void FramePDelta2D::assembleNonlinearForceVector(vec &q)
{
	int local[6] = {0,1,2,3,4,5};
	int global[6] = {nodeI->dofX->eqnId, nodeI->dofZ->eqnId, nodeI->dofRY->eqnId,
					 nodeJ->dofX->eqnId, nodeJ->dofZ->eqnId, nodeJ->dofRY->eqnId };
	bool fixed[6] = {nodeI->dofX->isFixed, nodeI->dofZ->isFixed, nodeI->dofRY->isFixed,
					 nodeJ->dofX->isFixed, nodeJ->dofZ->isFixed, nodeJ->dofRY->isFixed };

	for (int i = 0; i < 6; i++)
	{
		if (!fixed[i])
		{
			q(global[i]) += this->q(local[i]);
		}
	}
}