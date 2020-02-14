#include "FrameElastic2D.h"

FrameElastic2D::FrameElastic2D(const int id, node * nodeI, node * nodeJ, const double EA, const double EI):
	element2D(id, nodeI, nodeJ), EA(EA), EI(EI),
	ue(new double[3]{ 0.0, 0.0, 0.0 }),
	f(new double[3]{ 0.0, 0.0, 0.0 })
{
	k = mat({
				{EA/L, 0.0,    0.0},
				{0.0,  4*EI/L, 2*EI/L},
			  	{0.0,  2*EI/L, 4*EI/L}
			});

	T = mat({
				{-lxx,  lxy,   0.0,  lxx,   lxy,   0.0},
				{lyx/L, lyy/L, 1.0, -lyx/L, lyy/L, 0.0},
				{lyx/L, lyy/L, 0.0, -lyx/L, lyy/L, 1.0}
			});

	buildMatrix();

	nv = 3;
}

FrameElastic2D::~FrameElastic2D()
{
}

void FrameElastic2D::buildMatrix()
{
	K = T.t()*k*T;
}

void FrameElastic2D::getResponse(const bool update)
{
	u = vec( { nodeI->dofX->dsp, nodeI->dofZ->dsp, nodeI->dofRY->dsp,
		       nodeJ->dofX->dsp, nodeJ->dofZ->dsp, nodeJ->dofRY->dsp } );

	for (size_t i = 0; i < 3; i++)
	{
		ue[i] = 0.0;
		f[i] = 0.0;
	}

	for (size_t i = 0; i < 6; i++)
	{
		ue[0] += T(0, i)*u(i);
		ue[1] += T(1, i)*u(i);
		ue[2] += T(2, i)*u(i);
	}

	f[0] = k(0, 0)*ue[0] + k(0, 1)*ue[1] + k(0, 2)*ue[2];
	f[1] = k(1, 0)*ue[0] + k(1, 1)*ue[1] + k(1, 2)*ue[2];
	f[2] = k(2, 0)*ue[0] + k(2, 1)*ue[1] + k(2, 2)*ue[2];

	force = f;
	deformation = ue;
}

void FrameElastic2D::assembleStiffnessMatrix(mat &K)
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
