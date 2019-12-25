#include "dsystem.h"
#include "numeric.h"

dsystem::dsystem(const double z)
{
	zeta = z;
	eqnCount = 0;
	eigenVectorNormed = false;
}


dsystem::~dsystem()
{
}

void dsystem::addDof(dof * d)
{
	dofs[d->num] = d;
}

void dsystem::addDof(const int n, const double m, const bool fixed)
{
	dof *d = new dof(n, m, fixed);
	addDof(d);
}

void dsystem::addSpring(spring * s)
{
	springs[s->num] = s;
}

void dsystem::addSpring(const int n, const int ni, const int nj, const double k)
{
	dof *i = dofs[ni];
	dof *j = dofs[nj];
	spring *s = new spring(n, i, j, k);
	addSpring(s);
}

void dsystem::addDashpot(dashpot * d)
{
	dashpots[d->num] = d;
}

void dsystem::addDashpot(const int n, const int ni, const int nj, const double c)
{
	dof *i = dofs[ni];
	dof *j = dofs[nj];
	dashpot *d = new dashpot(n, i, j, c);
	addDashpot(d);
}

void dsystem::addInerter(inerter * in)
{
	inerters[in->num] = in;
}

void dsystem::addInerter(const int n, const int ni, const int nj, const double m)
{
	dof *i = dofs[ni];
	dof *j = dofs[nj];
	inerter *in = new inerter(n, i, j, m);
	addInerter(in);
}

void dsystem::addSPIS2(spis2 * s)
{
	spis2s[s->num] = s;
}

void dsystem::addSPIS2(const int n, const int ni, const int nj, const int nin, const double m, const double c, const double k)
{
	dof *i = dofs[ni];
	dof *j = dofs[nj];
	dof *in = dofs[nin];
	spis2 *s = new spis2(n, i, j, in, m, c, k);
	addSPIS2(s);
}

void dsystem::addTimeseries(timeseries * ts)
{
	tss[ts->id] = ts;
}

void dsystem::addTimeseries(const int n, const double dt, const vec & s)
{
	timeseries *ts = new timeseries(n, dt, s);
	addTimeseries(ts);
}

void dsystem::addTimeseries(const int n, const double dt, const char* fileName)
{
	timeseries *ts = new timeseries(n, dt, fileName);
	addTimeseries(ts);
}

void dsystem::buildDofEqnMap()
{
	std::map<int, dof *>::iterator it;

	for (it = dofs.begin(); it != dofs.end(); it++)
	{
		dof *d = it->second;
		if (!(d->isFixed))
		{
			eqnCount += 1;
			dofMapEqn[d->num] = eqnCount - 1;
			eqnMapDof[eqnCount - 1] = d->num;
		}
	}
}

void dsystem::assembleMassMatrix()
{
	buildDofEqnMap();
	vec m(eqnCount);
	for (int i = 0; i < eqnCount; i++)
	{
		m(i) = dofs[eqnMapDof[i]]->mass;
	}
	Mp = diagmat(m);
	M = diagmat(m);

	if ( !(inerters.empty()) )
	{
		std::map<int, inerter *>::iterator it;
		for (it = inerters.begin(); it != inerters.end(); it++)
		{
			inerter *in = it->second;
			int i_local = 0;
			int j_local = 1;

			if (in->dofI->isFixed)
			{
				int j_global = dofMapEqn[in->dofJ->num];
				M(j_global, j_global) += in->M(j_local, j_local);
			}
			else if (in->dofJ->isFixed)
			{
				int i_global = dofMapEqn[in->dofI->num];
				M(i_global, i_global) += in->M(i_local, i_local);
			}
			else
			{
				int i_global = dofMapEqn[in->dofI->num];
				int j_global = dofMapEqn[in->dofJ->num];

				M(i_global, i_global) += in->M(i_local, i_local);
				M(i_global, j_global) += in->M(i_local, j_local);
				M(j_global, i_global) += in->M(j_local, i_local);
				M(j_global, j_global) += in->M(j_local, j_local);
			}
		}
	}
	
	if (!(spis2s.empty()))
	{
		std::map<int, spis2 *>::iterator it;
		for (it = spis2s.begin(); it != spis2s.end(); it++)
		{
			spis2 *s = it->second;
			int in_local = 1;
			int in_global = dofMapEqn[s->dofIN->num];
			M(in_global, in_global) += s->M(in_local, in_local);
		}
	}
}

void dsystem::assembleStiffnessMatrix()
{
	K = zeros<mat>(eqnCount, eqnCount);

	if (!(springs.empty()))
	{
		std::map<int, spring *>::iterator it;
		for (it = springs.begin(); it != springs.end(); it++)
		{
			spring *s = it->second;
			int i_local = 0;
			int j_local = 1;

			if (s->dofI->isFixed)
			{
				int j_global = dofMapEqn[s->dofJ->num];
				K(j_global, j_global) += s->K(j_local, j_local);
			}
			else if (s->dofJ->isFixed)
			{
				int i_global = dofMapEqn[s->dofI->num];
				K(i_global, i_global) += s->K(i_local, i_local);
			}
			else
			{
				int i_global = dofMapEqn[s->dofI->num];
				int j_global = dofMapEqn[s->dofJ->num];

				K(i_global, i_global) += s->K(i_local, i_local);
				K(i_global, j_global) += s->K(i_local, j_local);
				K(j_global, i_global) += s->K(j_local, i_local);
				K(j_global, j_global) += s->K(j_local, j_local);
			}
		}
	}

	if (!(spis2s.empty()))
	{
		std::map<int, spis2 *>::iterator it;
		for (it = spis2s.begin(); it != spis2s.end(); it++)
		{
			spis2 *s = it->second;
			int in_global = dofMapEqn[s->dofIN->num];

			int i_local = 0;
			int in_local = 1;
			int j_local = 2;

			if (s->dofI->isFixed)
			{
				int j_global = dofMapEqn[s->dofJ->num];
				K(in_global, in_global) += s->K(in_local, in_local);
				K(in_global, j_global) += s->K(in_local, j_local);
				K(j_global, in_global) += s->K(j_local, in_local);
				K(j_global, j_global) += s->K(j_local, j_local);
			}
			else if (s->dofJ->isFixed)
			{
				int i_global = dofMapEqn[s->dofI->num];
				K(in_global, in_global) += s->K(in_local, in_local);
				K(in_global, i_global) += s->K(in_local, i_local);
				K(i_global, in_global) += s->K(i_local, in_local);
				K(i_global, i_global) += s->K(i_local, i_local);
			}
			else
			{
				int i_global = dofMapEqn[s->dofI->num];
				int j_global = dofMapEqn[s->dofJ->num];

				K(i_global, i_global) += s->K(i_local, i_local);
				K(i_global, j_global) += s->K(i_local, j_local);
				K(i_global, in_global) += s->K(i_local, in_local);
				K(j_global, i_global) += s->K(j_local, i_local);
				K(j_global, j_global) += s->K(j_local, j_local);
				K(j_global, in_global) += s->K(j_local, in_local);
				K(in_global, i_global) += s->K(in_local, i_local);
				K(in_global, j_global) += s->K(in_local, j_local);
				K(in_global, in_global) += s->K(in_local, in_local);
			}
		}
	}
}

void dsystem::buildInherentDampingMatrix(const int n)
{
	if (zeta == 0.0)
	{
		C = zeros<mat>(eqnCount, eqnCount);
	}
	else
	{
		mat MPhi = Mp * Phi;
		if (eigenVectorNormed)
		{
			C = diagmat(2.0*zeta*omg);
			C = MPhi * C*MPhi.t();
		}
		else
		{
			C = diagmat(2.0*zeta*omg / diagvec(Phi.t()*MPhi));
			C = MPhi*C*MPhi.t();
		}
	}
	
}

void dsystem::buildRayleighDampingMatrix(const double omg1, const double omg2)
{
	if (zeta == 0.0)
	{
		C = zeros<mat>(eqnCount, eqnCount);
	}
	else
	{
		double a0 = zeta * 2.0*omg1*omg2 / (omg1 + omg2);
		double a1 = zeta * 2.0 / (omg1 + omg2);
		C = a0 * Mp + a1 * K;
	}
}

void dsystem::buildRayleighDampingMatrix(const int md1, const int md2)
{
	if (zeta == 0.0)
	{
		C = zeros<mat>(eqnCount, eqnCount);
	}
	else
	{
		double omg1 = omg(md1 - 1);
		double omg2 = omg(md2 - 1);
		buildRayleighDampingMatrix(omg1, omg2);
	}
	
}

void dsystem::assembleDampingMatrix()
{

	if (!(dashpots.empty()))
	{
		std::map<int, dashpot *>::iterator it;
		for (it = dashpots.begin(); it != dashpots.end(); it++)
		{
			dashpot *d = it->second;
			int i_local = 0;
			int j_local = 1;

			if (d->dofI->isFixed)
			{
				int j_global = dofMapEqn[d->dofJ->num];
				C(j_global, j_global) += d->C(j_local, j_local);
			}
			else if (d->dofJ->isFixed)
			{
				int i_global = dofMapEqn[d->dofI->num];
				C(i_global, i_global) += d->C(i_local, i_local);
			}
			else
			{
				int i_global = dofMapEqn[d->dofI->num];
				int j_global = dofMapEqn[d->dofJ->num];

				C(i_global, i_global) += d->C(i_local, i_local);
				C(i_global, j_global) += d->C(i_local, j_local);
				C(j_global, i_global) += d->C(j_local, i_local);
				C(j_global, j_global) += d->C(j_local, j_local);
			}
		}
	}

	if (!(spis2s.empty()))
	{
		std::map<int, spis2 *>::iterator it;
		for (it = spis2s.begin(); it != spis2s.end(); it++)
		{
			spis2 *s = it->second;
			int in_local = 1;
			int in_global = dofMapEqn[s->dofIN->num];
			C(in_global, in_global) += s->C(in_local, in_local);
		}
	}
}

void dsystem::buildGroundMotionVector()
{
	E = ones<vec>(eqnCount);
	//std::map<int, dof *>::iterator it;
	//for (it = dofs.begin(); it != dofs.end(); it++)
	//{
	//	dof *d = it->second;
	//	if (d->isRelative)
	//	{
	//		E(dofMapEqn[d->num]) = 0.0;
	//	}
	//}
}

void dsystem::solveEigen()
{

	omg = zeros<vec>(eqnCount);
	Phi = zeros<mat>(eqnCount, eqnCount);

	eig_sym(K, M, omg, Phi);

	eigenVectorNormed = true;

	P = 2.0*PI / omg;
}

void dsystem::solveComplexEigen()
{
	mat O = zeros<mat>(eqnCount, eqnCount);
	mat A = join_cols(join_rows(C, M), join_rows(M, O));
	mat B = join_cols(join_rows(K, O), join_rows(O, -M));

	mat B_eig = solve(A, -B);

	cx_vec cx_lbd;
	cx_mat cx_Phi;
	eig_gen(cx_lbd, cx_Phi, B_eig);
}

void dsystem::solveStochasticResponse(const double f_h, const int nf, const char method)
{
	double omg_h = 2.0*PI*f_h;
	vec omg = linspace(0, omg_h, nf+1);
	cx_double cx_I(0.0, 1.0);

	if (method=='c')
	{
		buildGroundMotionVector();
		vec F = zeros<vec>(2*eqnCount);
		F.head(eqnCount) = -Mp*E;

		mat O = zeros<mat>(eqnCount, eqnCount);
		mat A = join_cols(join_rows(C, M), join_rows(M, O));
		mat B = join_cols(join_rows(K, O), join_rows(O, -M));

		mat B_eig = solve(A, -B);
		cx_vec cx_lbd;
		cx_mat cx_Phi;
		eig_gen(cx_lbd, cx_Phi, B_eig);

		cx_mat A_d = cx_mat(A, zeros<mat>(2*eqnCount, 2*eqnCount));

		cx_vec Ad = diagvec( cx_Phi.st()*A*cx_Phi );
		cx_vec r = cx_Phi.st()*F / Ad;

		cx_mat Z = zeros<cx_mat>(2*eqnCount, nf+1);
		
		for (int i = 0; i < nf+1; i++)
		{
			Z.col(i) = r/(omg(i)*cx_I - cx_lbd);
		}

		cx_mat Y = cx_Phi*Z;
		cx_mat X = Y.head_rows(eqnCount);
		mat Sx = real(conj(X) % X);

		dsp = sqrt(trapz(omg, Sx, 1));
	}
	else if (method=='d')
	{
		buildGroundMotionVector();
		cx_mat P = cx_vec(-Mp*E, zeros<vec>(eqnCount));

		cx_mat X = zeros<cx_mat>(eqnCount, nf+1);
		for (int i = 0; i < nf+1; i++)
		{
			cx_mat Q = -omg(i)*omg(i)*M + omg(i)*cx_I*C + K;
			X.col(i) = solve(Q, P);
		}
		mat Sx = real(conj(X) % X);

		dsp = sqrt(trapz(omg, Sx, 1));
	}
}

void dsystem::solveTimeDomainSeismicResponse(const int tsn, const double s, const char method)
{
	int nstep = tss[tsn]->nsteps;
	double dt = tss[tsn]->dt;
	vec ag = s*tss[tsn]->series;

	vec u0 = zeros<vec>(eqnCount);
	vec v0 = zeros<vec>(eqnCount);

	u = zeros<mat>(eqnCount, nstep);
	v = zeros<mat>(eqnCount, nstep);
	a = zeros<mat>(eqnCount, nstep);

	double gma = 0.5;
	double bta = 0.25;
	double c1 = 1.0 / bta / dt / dt;
	double c2 = 1.0 / bta / dt;
	double c3 = gma / bta / dt;
	double c4 = 1.0 - gma / bta;
	double c5 = 1.0 - 0.5*gma / bta;
	double c6 = 0.5 / bta - 1.0;

	mat c7 = c1*M + c3*C;
	mat c8 = c2*M - c4*C;
	mat c9 = c6*M - dt*c5*C;

	mat K_h = c1*M + c3*C + K;

	u.col(0) = u0;
	v.col(0) = v0;
	a.col(0) = solve(M, -Mp*E*ag(0) - C*v0 - K*u0);

	vec p_h;
	for (int i = 0; i < nstep-1; i++)
	{
		p_h = -Mp*E*ag(i+1) + c7*u.col(i) + c8*v.col(i) + c9*a.col(i);
		u.col(i+1) = solve(K_h, p_h);
		v.col(i+1) = c3*(u.col(i+1) - u.col(i)) + c4*v.col(i) + dt*c5*a.col(i);
		a.col(i+1) = solve(M, -Mp*E*ag(i+1)-C*v.col(i+1)-K*u.col(i+1));
	}
}

void dsystem::setDofResponse()
{
	for (int i = 0; i < eqnCount; i++)
	{
		dofs[eqnMapDof[i]]->dsp = dsp(i);
	}
}
