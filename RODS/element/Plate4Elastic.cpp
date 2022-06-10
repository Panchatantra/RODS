#include "Plate4Elastic.h"

Plate4Elastic::Plate4Elastic(const int id, Node *nodeI, Node *nodeJ, Node *nodeP, Node *nodeQ,
		const double E, const double nu, const double t):
	Quad2D(id, nodeI, nodeJ, nodeP, nodeQ),
	E(E), nu(nu), t(t)
{	
	double dx, dy;

	dx = nodeJ->x0 - nodeI->x0;
	dy = nodeJ->y0 - nodeI->y0;
	a = sqrt(dx*dx + dy*dy)*0.5;
	dx = nodeQ->x0 - nodeI->x0;
	dy = nodeQ->y0 - nodeI->y0;
	b = sqrt(dx*dx + dy*dy)*0.5;
	buildMatrix();
	nv = 12;
}

Plate4Elastic::~Plate4Elastic()
{
}

void Plate4Elastic::buildMatrix()
{
	double xi[4] = {1.0,-1.0,-1.0,1.0};
	double eta[4] = {1.0,1.0,-1.0,-1.0};

	double p = E*t*t*t/720.0/a/b/(1.0 - nu*nu);
	double a2 = a*a;
	double b2 = b*b;
	double a2_b2 = a2/b2;
	double b2_a2 = b2/a2;

	double xi_0 = 0.0, eta_0 = 0.0;

	K.fill(0.0);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			xi_0 = xi[i]*xi[j];
			eta_0 = eta[i]*eta[j];
			K(3*i, 3*j) = p*(15.0*b2_a2*xi_0*(eta_0+3.0) + 15.0*a2_b2*eta_0*(xi_0+3.0) + 6.0*(7.0-2.0*nu)*xi_0*eta_0);
			K(3*i, 3*j+1) = p*(-3.0*b*xi_0*(5.0*eta[j]*nu + eta[i]*(3.0*nu + 2.0 + 5.0*a2_b2)) - 45.0*a2/b*eta[i]);
			K(3*i, 3*j+2) = p*(3.0*a*eta_0*(5.0*xi[j]*nu + xi[i]*(3.0*nu + 2.0 + 5.0*b2_a2)) + 45.0*b2/a*xi[i]);
			K(3*i+1, 3*j) = p*(-3.0*b*xi_0*(5.0*eta[i]*nu + eta[j]*(3.0*nu + 2.0 + 5.0*a2_b2)) - 45.0*a2/b*eta[j]);
			K(3*i+1, 3*j+1) = p*((a2 + 2.0*b2*(1.0-nu))*xi_0*(5.0*eta_0+3.0) + 3.0*a2*(4.0*xi_0 + 5.0*eta_0 + 15.0));
			K(3*i+1, 3*j+2) = p*(-15.0*a*b*(eta[j] + eta[i])*(xi[j] + xi[i])*nu);
			K(3*i+2, 3*j) = p*(3.0*a*eta_0*(5.0*xi[i]*nu + xi[j]*(3.0*nu + 2.0 + 5.0*b2_a2)) + 45.0*b2/a*xi[j]);
			K(3*i+2, 3*j+1) = p*(-15.0*a*b*(eta[i] + eta[j])*(xi[i] + xi[j])*nu);
			K(3*i+2, 3*j+2) = p*((b2 + 2.0*a2*(1.0-nu))*eta_0*(5.0*xi_0+3.0) + 3.0*b2*(4.0*eta_0 + 5.0*xi_0 + 15.0));
		}
	}
}

void Plate4Elastic::getResponse(const bool update)
{
	u = vec( { 
		nodeI->dofZ->dsp, nodeI->dofRX->dsp, nodeI->dofRY->dsp,
		nodeJ->dofZ->dsp, nodeJ->dofRX->dsp, nodeJ->dofRY->dsp,
		nodeP->dofZ->dsp, nodeP->dofRX->dsp, nodeP->dofRY->dsp,
		nodeQ->dofZ->dsp, nodeQ->dofRX->dsp, nodeQ->dofRY->dsp,
	} );

	f = K*u;

	force = f.memptr();
	deformation = u.memptr();
}

void Plate4Elastic::assembleStiffnessMatrix(mat &K)
{
	int local[12] = {0,1,2,3,4,5,6,7,8,9,10,11};
	int global[12] = {
		nodeI->dofZ->eqnId, nodeI->dofRX->eqnId, nodeI->dofRY->eqnId,
		nodeJ->dofZ->eqnId, nodeJ->dofRX->eqnId, nodeJ->dofRY->eqnId,
		nodeP->dofZ->eqnId, nodeP->dofRX->eqnId, nodeP->dofRY->eqnId,
		nodeQ->dofZ->eqnId, nodeQ->dofRX->eqnId, nodeQ->dofRY->eqnId,
		};

	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			K(global[i], global[j]) += this->K(local[i], local[j]);
		}
	}
}
