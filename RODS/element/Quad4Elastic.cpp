#include "Quad4Elastic.h"

Quad4Elastic::Quad4Elastic(const int id, Node *nodeI, Node *nodeJ, Node *nodeP, Node *nodeQ,
		const double E, const double nu, const double t):
	Quad2D(id, nodeI, nodeJ, nodeP, nodeQ),
	E(E), nu(nu), t(t)
{
	N.fill(0.0);
	dN.fill(0.0);
	B.fill(0.0);
	buildMatrix();
	nv = 8;
}

Quad4Elastic::~Quad4Elastic()
{
}

void Quad4Elastic::buildMatrix()
{
	D = E/(1.0-nu*nu)*mat( {
		{1.0, nu, 0.0},
		{nu, 1.0, 0.0},
		{0.0, 0.0, 0.5-0.5*nu}
	} );

	const int nInt = 2;
	double xiInt[nInt] {-0.5773502691896258, 0.5773502691896258};
	double wInt[nInt] {1.0, 1.0};

	double xi = 0.0, eta = 0.0;
	double w_xi = 0.0, w_eta = 0.0;

	K.fill(0.0);
	for (int i = 0; i < nInt; i++)
	{
		for (int j = 0; j < nInt; j++)
		{
			xi  = xiInt[i];
			eta = xiInt[j];
			w_xi  = wInt[i];
			w_eta = wInt[j];
			calcShapeFunction(xi, eta);
			K += t*w_xi*w_eta*B.t()*D*B*detJ;
		}
	}
}

void Quad4Elastic::getResponse(const bool update)
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

void Quad4Elastic::assembleStiffnessMatrix(mat &K)
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

void Quad4Elastic::calcShapeFunction(const double xi, const double eta)
{
	N(0,0) = 0.25*(1.0-xi)*(1.0-eta);
	N(0,1) = 0.25*(1.0+xi)*(1.0-eta);
	N(0,2) = 0.25*(1.0+xi)*(1.0+eta);
	N(0,3) = 0.25*(1.0-xi)*(1.0+eta);

	dN(0, 0) = -0.25*(1.0-eta);
	dN(0, 1) = 0.25*(1.0-eta);
	dN(0, 2) = 0.25*(1.0+eta);
	dN(0, 3) = -0.25*(1.0+eta);

	dN(1, 0) = -0.25*(1.0-xi);
	dN(1, 1) = -0.25*(1.0+xi);
	dN(1, 2) = 0.25*(1.0+xi);
	dN(1, 3) = 0.25*(1.0-xi);

	mat::fixed<4, 2> Crds( {
		{nodeI->x0, nodeI->z0},
		{nodeJ->x0, nodeJ->z0},
		{nodeP->x0, nodeP->z0},
		{nodeQ->x0, nodeQ->z0}
	} );

	J = dN*Crds;
	detJ = det(J);

	dN = inv(J)*dN;

	for (int i = 0; i < 4; ++i)
	{
		B(0, i*2) = dN(0, i);
		B(1, i*2+1) = dN(1, i);
		B(2, i*2) = dN(1, i);
		B(2, i*2+1) = dN(0, i);
	}
}
