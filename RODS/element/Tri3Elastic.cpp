#include "Tri3Elastic.h"

Tri3Elastic::Tri3Elastic(const int id, Node *nodeI, Node *nodeJ, Node *nodeP,
		const double E, const double nu, const double t):
	Tri2D(id, nodeI, nodeJ, nodeP),
	E(E), nu(nu), t(t)
{	
	buildMatrix();
	nv = 6;
}

Tri3Elastic::~Tri3Elastic()
{
}

void Tri3Elastic::buildMatrix()
{
	double x_i = nodeI->x0;
	double y_i = nodeI->z0;
	double x_j = nodeJ->x0;
	double y_j = nodeJ->z0;
	double x_k = nodeP->x0;
	double y_k = nodeP->z0;

	mat::fixed<3,3> A_(mat({
		{x_i, y_i, 1.0},
		{x_j, y_j, 1.0},
		{x_k, y_k, 1.0},
		}));

	A = 0.5*det(A_);

	double a_i = x_j*y_k - x_k*y_j;
	double a_j = x_k*y_i - x_i*y_k;
	double a_k = x_i*y_j - x_j*y_i;

	double b_i = y_j - y_k;
	double b_j = y_k - y_i;
	double b_k = y_i - y_j;

	double c_i = x_k - x_j;
	double c_j = x_i - x_k;
	double c_k = x_j - x_i;

	double a[3] = {a_i, a_j, a_k};
	double b[3] = {b_i, b_j, b_k};
	double c[3] = {c_i, c_j, c_k};

	double nu_ = (1.0 - nu)*0.5;
	double p = E*t/4.0/(1.0 - nu*nu)/A;

	K.fill(0.0);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			K(2*i, 2*j) = p*(nu_*c[i]*c[j] + b[i]*b[j]);
			K(2*i, 2*j+1) = p*(nu_*c[i]*b[j] + b[i]*c[j]*nu);
			K(2*i+1, 2*j) = p*(nu_*b[i]*c[j] + c[i]*b[j]*nu);
			K(2*i+1, 2*j+1) = p*(nu_*b[i]*b[j] + c[i]*c[j]);
		}
	}
}

void Tri3Elastic::getResponse(const bool update)
{
	u = vec( { nodeI->dofX->dsp, nodeI->dofZ->dsp,
				nodeJ->dofX->dsp, nodeJ->dofZ->dsp,
				nodeP->dofX->dsp, nodeP->dofZ->dsp
	} );

	f = K*u;

	force = f.memptr();
	deformation = u.memptr();
}

void Tri3Elastic::assembleStiffnessMatrix(mat &K)
{
	int local[6] = {0,1,2,3,4,5};
	int global[6] = {nodeI->dofX->eqnId, nodeI->dofZ->eqnId,
					 nodeJ->dofX->eqnId, nodeJ->dofZ->eqnId,
					 nodeP->dofX->eqnId, nodeP->dofZ->eqnId
		};

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			K(global[i], global[j]) += this->K(local[i], local[j]);
		}
	}
}
