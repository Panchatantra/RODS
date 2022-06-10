#include "Rect4Elastic.h"

Rect4Elastic::Rect4Elastic(const int id, Node *nodeI, Node *nodeJ, Node *nodeP, Node *nodeQ,
		const double E, const double nu, const double t):
	Quad2D(id, nodeI, nodeJ, nodeP, nodeQ),
	E(E), nu(nu), t(t)
{	
	double dx, dz;

	dx = nodeJ->x0 - nodeI->x0;
	dz = nodeJ->z0 - nodeI->z0;
	a = sqrt(dx*dx + dz*dz)*0.5;
	dx = nodeQ->x0 - nodeI->x0;
	dz = nodeQ->z0 - nodeI->z0;
	b = sqrt(dx*dx + dz*dz)*0.5;
	buildMatrix();
	nv = 8;
}

Rect4Elastic::~Rect4Elastic()
{
}

void Rect4Elastic::buildMatrix()
{
	double xi[4] = {1.0,-1.0,-1.0,1.0};
	double eta[4] = {1.0,1.0,-1.0,-1.0};

	double nu_ = (1.0 - nu)*0.5;
	double p = E*t/4.0/(1.0 - nu*nu);
	double a_3b = a/3.0/b;
	double b_3a = b/3.0/a;

	double xi_0, eta_0;

	K.fill(0.0);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			xi_0 = xi[i]*xi[j];
			eta_0 = eta[i]*eta[j];
			K(2*i, 2*j) = p*(nu_*a_3b*eta_0*(xi_0+3.0) + b_3a*(eta_0+3.0)*xi_0);
			K(2*i, 2*j+1) = p*(nu_*eta[i]*xi[j] + xi[i]*eta[j]*nu);
			K(2*i+1, 2*j) = p*(nu_*xi[i]*eta[j] + eta[i]*xi[j]*nu);
			K(2*i+1, 2*j+1) = p*(nu_*b_3a*xi_0*(eta_0+3.0) + a_3b*(xi_0+3.0)*eta_0);
		}
	}
}

void Rect4Elastic::getResponse(const bool update)
{
	u = vec( { nodeI->dofX->dsp, nodeI->dofZ->dsp,
				nodeJ->dofX->dsp, nodeJ->dofZ->dsp,
				nodeP->dofX->dsp, nodeP->dofZ->dsp,
				nodeQ->dofX->dsp, nodeQ->dofZ->dsp
	} );

	f = K*u;

	force = f.memptr();
	deformation = u.memptr();
}

void Rect4Elastic::assembleStiffnessMatrix(mat &K)
{
	int local[8] = {0,1,2,3,4,5,6,7};
	int global[8] = {nodeI->dofX->eqnId, nodeI->dofZ->eqnId,
					 nodeJ->dofX->eqnId, nodeJ->dofZ->eqnId,
					 nodeP->dofX->eqnId, nodeP->dofZ->eqnId,
					 nodeQ->dofX->eqnId, nodeQ->dofZ->eqnId
		};

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			K(global[i], global[j]) += this->K(local[i], local[j]);
		}
	}
}
