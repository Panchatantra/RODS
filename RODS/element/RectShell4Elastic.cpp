#include "RectShell4Elastic.h"

RectShell4Elastic::RectShell4Elastic(const int id, Node *nodeI, Node *nodeJ, Node *nodeP, Node *nodeQ,
		const double E, const double nu, const double t):
	Shell3D(id, nodeI, nodeJ, nodeP, nodeQ),
	E(E), nu(nu), t(t)
{	
	double dx_12, dy_12, dz_12;
	double dx_14, dy_14, dz_14;

	dx_12 = nodeI->x0 - nodeJ->x0;
	dy_12 = nodeI->y0 - nodeJ->y0;
	dz_12 = nodeI->z0 - nodeJ->z0;
	a = sqrt(dx_12*dx_12 + dy_12*dy_12 + dz_12*dz_12)*0.5;
	dx_14 = nodeI->x0 - nodeQ->x0;
	dy_14 = nodeI->y0 - nodeQ->y0;
	dz_14 = nodeI->z0 - nodeQ->z0;
	b = sqrt(dx_14*dx_14 + dy_14*dy_14 + dz_14*dz_14)*0.5;

	double _two_a = 1.0/(2.0*a);
	double _two_b = 1.0/(2.0*b);
	double _four_a_b = 1.0/(4.0*a*b);

	lxx = dx_12*_two_a;
	lxy = dy_12*_two_a;
	lxz = dz_12*_two_a;
	lyx = dx_14*_two_b;
	lyy = dy_14*_two_b;
	lyz = dz_14*_two_b;
	lzx = (dy_12*dz_14 - dz_12*dy_14)*_four_a_b;
	lzy = (dz_12*dx_14 - dx_12*dz_14)*_four_a_b;
	lzz = (dx_12*dy_14 - dy_12*dx_14)*_four_a_b;

	T.fill(0.0);
	for (auto i = 0; i < 8; i++)
	{
		T(3*i, 3*i) = lxx;
		T(3*i, 3*i+1) = lxy;
		T(3*i, 3*i+2) = lxz;
		T(3*i+1, 3*i) = lyx;
		T(3*i+1, 3*i+1) = lyy;
		T(3*i+1, 3*i+2) = lyz;
		T(3*i+2, 3*i) = lzx;
		T(3*i+2, 3*i+1) = lzy;
		T(3*i+2, 3*i+2) = lzz;
	}

	buildMatrix();
	nv = 24;
}

RectShell4Elastic::~RectShell4Elastic()
{
}

void RectShell4Elastic::buildMatrix()
{
	double xi[4] = {1.0,-1.0,-1.0,1.0};
	double eta[4] = {1.0,1.0,-1.0,-1.0};

	double nu_ = (1.0 - nu)*0.5;
	double p_p = E*t/4.0/(1.0 - nu*nu);
	double a_3b = a/3.0/b;
	double b_3a = b/3.0/a;

	double p = E*t*t*t/720.0/a/b/(1.0 - nu*nu);
	double a2 = a*a;
	double b2 = b*b;
	double a2_b2 = a2/b2;
	double b2_a2 = b2/a2;

	double xi_0 = 0.0, eta_0 = 0.0;

	k.fill(0.0);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			xi_0 = xi[i]*xi[j];
			eta_0 = eta[i]*eta[j];
			k(6*i, 6*j) = p_p*(nu_*a_3b*eta_0*(xi_0+3.0) + b_3a*(eta_0+3.0)*xi_0);
			k(6*i, 6*j+1) = p_p*(nu_*eta[i]*xi[j] + xi[i]*eta[j]*nu);
			k(6*i+1, 6*j) = p_p*(nu_*xi[i]*eta[j] + eta[i]*xi[j]*nu);
			k(6*i+1, 6*j+1) = p_p*(nu_*b_3a*xi_0*(eta_0+3.0) + a_3b*(xi_0+3.0)*eta_0);

			k(6*i+2, 6*j+2) = p*(15.0*b2_a2*xi_0*(eta_0+3.0) + 15.0*a2_b2*eta_0*(xi_0+3.0) + 6.0*(7.0-2.0*nu)*xi_0*eta_0);
			k(6*i+2, 6*j+3) = p*(-3.0*b*xi_0*(5.0*eta[j]*nu + eta[i]*(3.0*nu + 2.0 + 5.0*a2_b2)) - 45.0*a2/b*eta[i]);
			k(6*i+2, 6*j+4) = p*(3.0*a*eta_0*(5.0*xi[j]*nu + xi[i]*(3.0*nu + 2.0 + 5.0*b2_a2)) + 45.0*b2/a*xi[i]);
			k(6*i+3, 6*j+2) = p*(-3.0*b*xi_0*(5.0*eta[i]*nu + eta[j]*(3.0*nu + 2.0 + 5.0*a2_b2)) - 45.0*a2/b*eta[j]);
			k(6*i+3, 6*j+3) = p*((a2 + 2.0*b2*(1.0-nu))*xi_0*(5.0*eta_0+3.0) + 3.0*a2*(4.0*xi_0 + 5.0*eta_0 + 15.0));
			k(6*i+3, 6*j+4) = p*(-15.0*a*b*(eta[j] + eta[i])*(xi[j] + xi[i])*nu);
			k(6*i+4, 6*j+2) = p*(3.0*a*eta_0*(5.0*xi[i]*nu + xi[j]*(3.0*nu + 2.0 + 5.0*b2_a2)) + 45.0*b2/a*xi[j]);
			k(6*i+4, 6*j+3) = p*(-15.0*a*b*(eta[i] + eta[j])*(xi[i] + xi[j])*nu);
			k(6*i+4, 6*j+4) = p*((b2 + 2.0*a2*(1.0-nu))*eta_0*(5.0*xi_0+3.0) + 3.0*b2*(4.0*eta_0 + 5.0*xi_0 + 15.0));

			k(6*i+5, 6*j+5) = 1.0;
		}
	}

	K = T.t()*k*T;
}

void RectShell4Elastic::getResponse(const bool update)
{
	u = vec( { 
		nodeI->dofX->dsp, nodeI->dofY->dsp, nodeI->dofZ->dsp, nodeI->dofRX->dsp, nodeI->dofRY->dsp, nodeI->dofRZ->dsp, 
		nodeJ->dofX->dsp, nodeJ->dofY->dsp, nodeJ->dofZ->dsp, nodeJ->dofRX->dsp, nodeJ->dofRY->dsp, nodeJ->dofRZ->dsp, 
		nodeP->dofX->dsp, nodeP->dofY->dsp, nodeP->dofZ->dsp, nodeP->dofRX->dsp, nodeP->dofRY->dsp, nodeP->dofRZ->dsp, 
		nodeQ->dofX->dsp, nodeQ->dofY->dsp, nodeQ->dofZ->dsp, nodeQ->dofRX->dsp, nodeQ->dofRY->dsp, nodeQ->dofRZ->dsp, 
	} );

	f = K*u;

	force = f.memptr();
	deformation = u.memptr();
}

void RectShell4Elastic::assembleStiffnessMatrix(mat &K)
{
	int local[24] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23};
	int global[24] = {
		nodeI->dofX->eqnId, nodeI->dofY->eqnId, nodeI->dofZ->eqnId, nodeI->dofRX->eqnId, nodeI->dofRY->eqnId, nodeI->dofRZ->eqnId,
		nodeJ->dofX->eqnId, nodeJ->dofY->eqnId, nodeJ->dofZ->eqnId, nodeJ->dofRX->eqnId, nodeJ->dofRY->eqnId, nodeJ->dofRZ->eqnId,
		nodeP->dofX->eqnId, nodeP->dofY->eqnId, nodeP->dofZ->eqnId, nodeP->dofRX->eqnId, nodeP->dofRY->eqnId, nodeP->dofRZ->eqnId,
		nodeQ->dofX->eqnId, nodeQ->dofY->eqnId, nodeQ->dofZ->eqnId, nodeQ->dofRX->eqnId, nodeQ->dofRY->eqnId, nodeQ->dofRZ->eqnId,
		};

	for (int i = 0; i < 24; i++)
	{
		for (int j = 0; j < 24; j++)
		{
			K(global[i], global[j]) += this->K(local[i], local[j]);
		}
	}
}