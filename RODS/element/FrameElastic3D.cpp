#include "FrameElastic3D.h"

FrameElastic3D::FrameElastic3D(const int id, Node * nodeI, Node * nodeJ, const double EA,
					const double EIy, const double EIz, const double GIp):
	ROD3D(id, nodeI, nodeJ), EA(EA), EIy(EIy), EIz(EIz), GIp(GIp)
{
	k = mat({
				{EA/L,  0.0,     0.0,     0.0,     0.0,     0.0  },
				{ 0.0,  GIp/L,   0.0,     0.0,     0.0,     0.0  },
				{ 0.0,  0.0,   4*EIy/L, 2*EIy/L,   0.0,     0.0  },
			  	{ 0.0,  0.0,   2*EIy/L, 4*EIy/L,   0.0,     0.0  },
				{ 0.0,  0.0,     0.0,     0.0,   4*EIz/L, 2*EIz/L},
			  	{ 0.0,  0.0,     0.0,     0.0,   2*EIz/L, 4*EIz/L}
			});

	T = mat({
				{-lxx,  -lxy,  -lxz,  0.0,  0.0,  0.0,  lxx,  lxy,  lxz,   0.0,  0.0,  0.0},
				{ 0.0,   0.0,   0.0, -lxx, -lxy, -lxz,  0.0,  0.0,  0.0,   lxx,  lxy,  lxz},
				{-lzx/L,-lzy/L,-lzz/L, lyx, lyy, lyz, lzx/L, lzy/L, lzz/L, 0.0,  0.0,  0.0},
				{-lzx/L,-lzy/L,-lzz/L, 0.0, 0.0, 0.0, lzx/L, lzy/L, lzz/L, lyx,  lyy,  lyz},
				{lyx/L, lyy/L, lyz/L,  lzx, lzy, lzz,-lyx/L,-lyy/L,-lyz/L, 0.0,  0.0,  0.0},
				{lyx/L, lyy/L, lyz/L,  0.0, 0.0, 0.0,-lyx/L,-lyy/L,-lyz/L, lzx,  lzy,  lzz}
			});

	buildMatrix();

	nv = 6;
}

FrameElastic3D::~FrameElastic3D()
{
}

void FrameElastic3D::buildMatrix()
{
	K = T.t()*k*T;
}

void FrameElastic3D::getResponse(const bool update)
{
	u = vec( {
			nodeI->dofX->dsp,  nodeI->dofY->dsp,  nodeI->dofZ->dsp,
			nodeI->dofRX->dsp, nodeI->dofRY->dsp, nodeI->dofRZ->dsp,
			nodeJ->dofX->dsp,  nodeJ->dofY->dsp,  nodeJ->dofZ->dsp,
			nodeJ->dofRX->dsp, nodeJ->dofRY->dsp, nodeJ->dofRZ->dsp
		} );

	ue = T*u;
	f = k*ue;

	force = f.memptr();
	deformation = ue.memptr();
}

void FrameElastic3D::assembleStiffnessMatrix(mat &K)
{
	int local[12] = {0,1,2,3,4,5,6,7,8,9,10,11};
	int global[12] = {
				nodeI->dofX->eqnId, nodeI->dofY->eqnId, nodeI->dofZ->eqnId,
				nodeI->dofRX->eqnId, nodeI->dofRY->eqnId, nodeI->dofRZ->eqnId,
				nodeJ->dofX->eqnId, nodeJ->dofY->eqnId, nodeJ->dofZ->eqnId,
				nodeJ->dofRX->eqnId, nodeJ->dofRY->eqnId, nodeJ->dofRZ->eqnId
			};

	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			K(global[i], global[j]) += this->K(local[i], local[j]);
		}
	}
}
