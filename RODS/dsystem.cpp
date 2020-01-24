#include "dsystem.h"
#include "numeric.h"


dsystem::dsystem(const double z) :
	eqnCount(0), eigenVectorNormed(false), dt(0.02), dynamicSolver(StateSpace)
{
	zeta = z;
}


dsystem::~dsystem()
{
}

void dsystem::addNode(node * nd)
{
	nodes[nd->id] = nd;
}

void dsystem::addNode(const int id, const double x, const double y, const double z)
{
	node *nd = new node(id, x, y, z);
	addNode(nd);
}

void dsystem::addDof(dof * d)
{
	if (dofs.count(d->id) == 0)
	{
		dofs[d->id] = d;
	}
	else 
	{
		cout << "dof ID: " << d->id << " already exists!"<< endl;
	}
	
}

void dsystem::addDof(const int id, const double m, const bool fixed)
{
	dof *d = new dof(id, X, m, fixed);
	addDof(d);
}

void dsystem::addDof(const int id, direction dir, const double m, const bool fixed)
{
	dof *d = new dof(id, dir, m, fixed);
	addDof(d);
}

void dsystem::mapDofNode(dof * d, node * nd)
{
	nd->setDof(d);
	dofMapNode[d->id] = nd->id;
}

void dsystem::mapDofNode(const int id_d, const int id_nd)
{
	node *nd = nodes.at(id_nd);
	dof *d = dofs.at(id_d);
	nd->setDof(d);
	dofMapNode[id_d] = id_nd;
}

bool dsystem::addElement(element * e)
{
	if (eles.count(e->id) == 0)
	{
		eles[e->id] = e;
		return true;
	}
	else
	{
		cout << "element ID: " << e->id << " already exists!" << endl;
		return false;
	}
}

void dsystem::addSpring(spring * s)
{
	if (addElement(s)) springs[s->id] = s;
}

void dsystem::addSpring(const int id, const int ni, const int nj, const double k)
{
	dof *i = dofs.at(ni);
	dof *j = dofs.at(nj);
	spring *s = new spring(id, i, j, k);
	addSpring(s);
}

void dsystem::addSpringBL(springBilinear * s)
{
	if (addElement(s)) springBLs[s->id] = s;
}

void dsystem::addSpringBL(const int id, const int ni, const int nj, const double k0, const double uy, const double alpha)
{
	dof *i = dofs.at(ni);
	dof *j = dofs.at(nj);
	springBilinear *s = new springBilinear(id, i, j, k0, uy, alpha);
	addSpringBL(s);
}

void dsystem::addSpringBW(springBoucWen * s)
{
	if (addElement(s)) springBWs[s->id] = s;
}

void dsystem::addSpringBW(const int id, const int ni, const int nj, const double k0, const double uy, const double alpha, const double beta, const double n)
{
	dof *i = dofs.at(ni);
	dof *j = dofs.at(nj);
	springBoucWen *s = new springBoucWen(id, i, j, k0, uy, alpha);
	addSpringBW(s);
}

void dsystem::addDashpot(dashpot * d)
{
	if (addElement(d)) dashpots[d->id] = d;
}

void dsystem::addDashpot(const int id, const int ni, const int nj, const double c)
{
	dof *i = dofs.at(ni);
	dof *j = dofs.at(nj);
	dashpot *d = new dashpot(id, i, j, c);
	addDashpot(d);
}

void dsystem::addDashpotExp(dashpotExp * d)
{
	if (addElement(d)) dashpotExps[d->id] = d;
}

void dsystem::addDashpotExp(const int id, const int ni, const int nj, const double c, const double alpha)
{
	dof *i = dofs.at(ni);
	dof *j = dofs.at(nj);
	dashpotExp *d = new dashpotExp(id, i, j, c, alpha);
	addDashpotExp(d);
}

void dsystem::addInerter(inerter * in)
{
	if (addElement(in)) inerters[in->id] = in;
}

void dsystem::addInerter(const int id, const int ni, const int nj, const double m)
{
	dof *i = dofs.at(ni);
	dof *j = dofs.at(nj);
	inerter *in = new inerter(id, i, j, m);
	addInerter(in);
}

void dsystem::addSlider(slider * s)
{
	if (addElement(s)) sliders[s->id] = s;
}

void dsystem::addSlider(const int id, const int ni, const int nj, const double muN)
{
	dof *i = dofs.at(ni);
	dof *j = dofs.at(nj);
	slider *s = new slider(id, i, j, muN);
	addSlider(s);
}

void dsystem::addSPIS2(spis2 * s)
{
	if (addElement(s)) spis2s[s->id] = s;
}

void dsystem::addSPIS2(const int id, const int ni, const int nj, const int nin, const double m, const double c, const double k)
{
	dof *i = dofs.at(ni);
	dof *j = dofs.at(nj);
	dof *in = dofs.at(nin);
	spis2 *s = new spis2(id, i, j, in, m, c, k);
	addSPIS2(s);
}

void dsystem::addTimeseries(timeseries * ts)
{
	tss[ts->id] = ts;
}

void dsystem::addTimeseries(const int id, const double dt, const vec &s)
{
	timeseries *ts = new timeseries(id, dt, s);
	addTimeseries(ts);
}

void dsystem::addTimeseries(const int id, const double dt, char* fileName)
{
	timeseries *ts = new timeseries(id, dt, fileName);
	addTimeseries(ts);
}

void dsystem::addDofRecorder(dofRecorder * dr)
{
	drs[dr->id] = dr;
}

void dsystem::addDofRecorder(const int id, int *dofIds, const int n, response rtype, char * fileName)
{
	std::vector<dof *> rdofs(n);

	for (int i = 0; i < n; i++)
	{
		rdofs[i] = dofs.at(dofIds[i]);
	}

	dofRecorder *dr = new dofRecorder(id, rdofs, rtype, fileName);
	addDofRecorder(dr);
}

void dsystem::addElementRecorder(elementRecorder * er)
{
	ers[er->id] = er;
}

void dsystem::addElementRecorder(const int id, int * eleIds, const int n, response rtype, char * fileName)
{
	std::vector<element *> reles(n);

	for (int i = 0; i < n; i++)
	{
		reles[i] = eles.at(eleIds[i]);
	}

	elementRecorder *er = new elementRecorder(id, reles, rtype, fileName);
	addElementRecorder(er);
}

void dsystem::addSpringRecorder(const int id, int * eleIds, const int n, response rtype, char * fileName)
{
	std::vector<element *> reles(n);

	for (int i = 0; i < n; i++)
	{
		reles[i] = springs.at(eleIds[i]);
	}

	elementRecorder *er = new elementRecorder(id, reles, rtype, fileName);
	addElementRecorder(er);
}

void dsystem::addDashpotRecorder(const int id, int * eleIds, const int n, response rtype, char * fileName)
{
	std::vector<element *> reles(n);

	for (int i = 0; i < n; i++)
	{
		reles[i] = dashpots.at(eleIds[i]);
	}

	elementRecorder *er = new elementRecorder(id, reles, rtype, fileName);
	addElementRecorder(er);
}

void dsystem::addInerterRecorder(const int id, int * eleIds, const int n, response rtype, char * fileName)
{
	std::vector<element *> reles(n);

	for (int i = 0; i < n; i++)
	{
		reles[i] = inerters.at(eleIds[i]);
	}

	elementRecorder *er = new elementRecorder(id, reles, rtype, fileName);
	addElementRecorder(er);
}

void dsystem::buildDofEqnMap()
{
	std::map<int, dof *>::iterator it;
	eqnCount = 0;
	for (it = dofs.begin(); it != dofs.end(); it++)
	{
		dof *d = it->second;
		if (!(d->isFixed))
		{
			eqnCount += 1;
			dofMapEqn[d->id] = eqnCount - 1;
			d->eqnId = eqnCount - 1;
			eqnMapDof[eqnCount - 1] = d->id;
		}
	}
}

void dsystem::assembleMatrix()
{
	assembleMassMatrix();
	assembleStiffnessMatrix();
	buildInherentDampingMatrix();
	assembleDampingMatrix();
}

void dsystem::assembleMassMatrix()
{
	buildDofEqnMap();
	vec m(eqnCount);
	for (int i = 0; i < eqnCount; i++)
	{
		m(i) = dofs.at(eqnMapDof[i])->mass;
	}
	Mp = diagmat(m);
	M = diagmat(m);
	E = ones<vec>(eqnCount);

	if ( !(inerters.empty()) )
	{
		std::map<int, inerter *>::iterator it;
		for (it = inerters.begin(); it != inerters.end(); it++)
		{
			inerter *in = it->second;
			in->assembleMassMatrix(M);
		}
	}
	
	if (!(spis2s.empty()))
	{
		std::map<int, spis2 *>::iterator it;
		for (it = spis2s.begin(); it != spis2s.end(); it++)
		{
			spis2 *s = it->second;
			s->assembleMassMatrix(M);
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
			s->assembleStiffnessMatrix(K);
		}
	}

	if (!(spis2s.empty()))
	{
		std::map<int, spis2 *>::iterator it;
		for (it = spis2s.begin(); it != spis2s.end(); it++)
		{
			spis2 *s = it->second;
			s->assembleStiffnessMatrix(K);
		}
	}

	K0 = K;

	if (!(springBLs.empty()))
	{
		std::map<int, springBilinear *>::iterator it;
		for (it = springBLs.begin(); it != springBLs.end(); it++)
		{
			springBilinear *s = it->second;
			s->assembleStiffnessMatrix(K);
		}
	}

	if (!(springBWs.empty()))
	{
		std::map<int, springBoucWen *>::iterator it;
		for (it = springBWs.begin(); it != springBWs.end(); it++)
		{
			springBoucWen *s = it->second;
			s->assembleStiffnessMatrix(K);
		}
	}
}

void dsystem::buildInherentDampingMatrix(const int n)
{
	int eigenNum = n>0 ? n: eqnCount;
	if (zeta == 0.0)
	{
		C = zeros<mat>(eqnCount, eqnCount);
	}
	else
	{
		mat Phi_ = Phi.head_cols(eigenNum);
		mat MPhi = Mp * Phi_;
		if (eigenVectorNormed)
		{
			C = diagmat(2.0*zeta*omg);
			C = MPhi*C*MPhi.t();
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
			d->assembleDampingMatrix(C);
		}
	}

	if (!(spis2s.empty()))
	{
		std::map<int, spis2 *>::iterator it;
		for (it = spis2s.begin(); it != spis2s.end(); it++)
		{
			spis2 *s = it->second;
			s->assembleDampingMatrix(C);
		}
	}
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

void dsystem::solveStochasticSeismicResponse(const double f_h, const int nf, const char method)
{
	double omg_h = 2.0*PI*f_h;
	vec omg = linspace(0, omg_h, nf+1);
	cx_double cx_I(0.0, 1.0);

	if (method=='c')
	{
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

void dsystem::solveTimeDomainSeismicResponse(const int tsId, const double s, const int nsub)
{
	switch (dynamicSolver)
	{
	case Newmark:
		solveTimeDomainSeismicResponseNMK(tsId, s, nsub);
		break;
	case Newmark_NL:
		solveTimeDomainSeismicResponseNMKNL(tsId, s, nsub);
		break;
	case StateSpace:
		solveTimeDomainSeismicResponseStateSpace(tsId, s, nsub);
		break;
	case StateSpace_NL:
		solveTimeDomainSeismicResponseStateSpaceNL(tsId, s, nsub);
		break;
	default:
		break;
	}
}

void dsystem::solveTimeDomainSeismicResponseNMK(const int tsId, const double s, const int nsub)
{
	nsteps = tss.at(tsId)->nsteps;
	dt = tss.at(tsId)->dt;
	vec ag = s*tss.at(tsId)->series;

	vec u0 = zeros<vec>(eqnCount);
	vec v0 = zeros<vec>(eqnCount);
	vec a0 = zeros<vec>(eqnCount);

	u = zeros<mat>(eqnCount, nsteps);
	v = zeros<mat>(eqnCount, nsteps);
	a = zeros<mat>(eqnCount, nsteps);

	cstep = 0;
	ctime = 0;
	initRecorders();

	dt = dt / nsub;
	element::dt = dt;

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
	a0 = solve(M, -Mp*E*ag(0) - C*v0 - K*u0);
	a.col(0) = a0;

	dsp = u.col(0);
	vel = v.col(0);
	acc = a.col(0);

	setDofResponse();
	getElementResponse();
	recordResponse();

	vec p_h;
	vec u_p;
	double agd, agi, agj;
	for (int i = 0; i < nsteps-1; i++)
	{
		cstep += 1;
		ctime += dt*nsub;
		agd = (ag(i + 1) - ag(i)) / nsub;
		agi = ag(i);
		for (int j = 1; j < nsub+1; j++)
		{
			agj = agi + agd*j;
			p_h = -Mp*E*agj + c7*u0 + c8*v0 + c9*a0;
			u_p = u0*1.0;
			u0 = solve(K_h, p_h);
			v0 = c3*(u0-u_p) + c4*v0 + dt*c5*a0;
			a0 = solve(M, -Mp*E*agj-C*v0-K*u0);
			
		}
		u.col(i+1) = u0;
		v.col(i+1) = v0;
		a.col(i+1) = a0;

		dsp = u.col(i + 1);
		vel = v.col(i + 1);
		acc = a.col(i + 1);

		setDofResponse();
		getElementResponse();
		recordResponse();
	}
	saveResponse();
}

void dsystem::solveTimeDomainSeismicResponseNMKNL(const int tsId, const double s, const int nsub, const double tol, const int maxiter)
{
	nsteps = tss.at(tsId)->nsteps;
	dt = tss.at(tsId)->dt;
	vec ag = s * tss.at(tsId)->series;

	vec u0 = zeros<vec>(eqnCount);
	vec v0 = zeros<vec>(eqnCount);
	vec a0 = zeros<vec>(eqnCount);

	u = zeros<mat>(eqnCount, nsteps);
	v = zeros<mat>(eqnCount, nsteps);
	a = zeros<mat>(eqnCount, nsteps);

	cstep = 0;
	ctime = 0;
	initRecorders();

	dt = dt / nsub;
	element::dt = dt;

	double gma = 0.5;
	double bta = 0.25;
	double c1 = 1.0 / bta / dt / dt;
	double c2 = 1.0 / bta / dt;
	double c3 = gma / bta / dt;
	double c4 = 1.0 - gma / bta;
	double c5 = 1.0 - 0.5*gma / bta;
	double c6 = 0.5 / bta - 1.0;

	mat c7 = c1 * M + c3 * C;
	mat c8 = c2 * M - c4 * C;
	mat c9 = c6 * M - dt * c5*C;

	mat K_h_ = c1 * M + c3 * C;
	mat K_h = K_h_ + K0;

	u.col(0) = u0;
	v.col(0) = v0;
	a0 = solve(M, -Mp*E*ag(0) - C*v0 - K0*u0);
	a.col(0) = a0;

	dsp = u.col(0);
	vel = v.col(0);
	acc = a.col(0);

	setDofResponse();
	getElementResponse();
	assembleNonlinearForceVector(true);
	recordResponse();

	vec p_h;
	vec u_p, v_p, a_p;
	vec du;
	double agd, agi, agj;
	double error;
	for (int i = 0; i < nsteps - 1; i++)
	{
		cstep += 1;
		ctime += dt * nsub;
		agd = (ag(i + 1) - ag(i)) / nsub;
		agi = ag(i);
		for (int j = 1; j < nsub + 1; j++)
		{
			agj = agi + agd * j;
			p_h = -Mp * E*agj + c7 * u0 + c8 * v0 + c9 * a0;
			u_p = u0 * 1.0;
			v_p = v0 * 1.0;
			a_p = a0 * 1.0;
			error = 1;
			for (int l = 0; l < maxiter; l++)
			{
				du = solve(K_h_+K, -K_h*u0-q+p_h);
				u0 = u0 + du;
				v0 = c3*(u0-u_p) + c4*v_p + dt*c5*a_p;
				a0 = solve(M, -Mp*E*agj - q - C*v0 - K0*u0);

				dsp = u0;
				vel = v0;
				acc = a0;

				setDofResponse();
				error = norm(du) / norm(u0);
				if (error>tol)
				{
					if (l < maxiter-1)
					{
						assembleNonlinearForceVector(false);
						assembleStiffnessMatrix();
					}
					else
					{
						cout << "Fail to Converge after " << maxiter << "iterations, "
							 << "norm error = " << error <<endl;
						if (j == (nsub - 1))
						{
							getElementResponse();
							recordResponse();
						}
						assembleNonlinearForceVector(true);
						assembleStiffnessMatrix();
					}
				}
				else
				{
					getElementResponse();
					recordResponse();
					assembleNonlinearForceVector(true);
					assembleStiffnessMatrix();
					break;
				}
			}
		}
		u.col(i + 1) = u0;
		v.col(i + 1) = v0;
		a.col(i + 1) = a0;

		dsp = u.col(i + 1);
		vel = v.col(i + 1);
		acc = a.col(i + 1);
	}
	saveResponse();
}

void dsystem::solveTimeDomainSeismicResponseStateSpace(const int tsId, const double s, const int nsub)
{
	nsteps = tss.at(tsId)->nsteps;
	dt = tss.at(tsId)->dt;
	vec ag = s * tss.at(tsId)->series;

	vec x0 = zeros<vec>(2*eqnCount);
	vec F = zeros<vec>(2*eqnCount);

	u = zeros<mat>(eqnCount, nsteps);
	v = zeros<mat>(eqnCount, nsteps);
	a = zeros<mat>(eqnCount, nsteps);

	cstep = 0;
	ctime = 0;
	initRecorders();

	dt = dt / nsub;
	element::dt = dt;

	mat O = zeros<mat>(eqnCount, eqnCount);
	mat I = eye<mat>(eqnCount, eqnCount);
	mat A = -solve(M,K);
	mat B = -solve(M,C);
	mat H = join_cols(join_rows(O, I), join_rows(A, B));

	double h = dt;
	mat T = expmat(H*h);

	u.col(0) = x0.head_rows(eqnCount);
	v.col(0) = x0.tail_rows(eqnCount);
	a.col(0) = solve(M, -Mp * E*ag(0) - C * v.col(0) - K * u.col(0));

	dsp = u.col(0);
	vel = v.col(0);
	acc = a.col(0);

	setDofResponse();
	getElementResponse();
	recordResponse();

	double agd, agi, agj;
	for (int i = 0; i < nsteps - 1; i++)
	{
		cstep += 1;
		ctime += dt * nsub;
		agd = (ag(i + 1) - ag(i)) / nsub;
		agi = ag(i);
		for (int j = 0; j < nsub; j++)
		{
			agj = agi + agd * j;
			F.tail_rows(eqnCount) = solve(M, -Mp*E*agj);
			x0 = T*(x0 + F*h);
			ctime += dt;
		}

		u.col(i + 1) = x0.head_rows(eqnCount);
		v.col(i + 1) = x0.tail_rows(eqnCount);
		a.col(i + 1) = solve(M, -Mp*E*agj - C*v.col(i + 1) - K*u.col(i + 1));

		dsp = u.col(i + 1);
		vel = v.col(i + 1);
		acc = a.col(i + 1);

		setDofResponse();
		getElementResponse();
		recordResponse();
	}
	saveResponse();
}

void dsystem::solveTimeDomainSeismicResponseStateSpaceNL(const int tsId, const double s, const int nsub)
{
	nsteps = tss.at(tsId)->nsteps;
	dt = tss.at(tsId)->dt;
	vec ag = s * tss.at(tsId)->series;

	vec x0 = zeros<vec>(2 * eqnCount);
	vec F = zeros<vec>(2 * eqnCount);

	u = zeros<mat>(eqnCount, nsteps);
	v = zeros<mat>(eqnCount, nsteps);
	a = zeros<mat>(eqnCount, nsteps);

	cstep = 0;
	ctime = 0;
	initRecorders();

	dt = dt / nsub;
	element::dt = dt;

	mat O = zeros<mat>(eqnCount, eqnCount);
	mat I = eye<mat>(eqnCount, eqnCount);
	mat A = -solve(M, K0);
	mat B = -solve(M, C);
	mat H = join_cols(join_rows(O, I), join_rows(A, B));

	double h = dt;
	mat T = expmat(H*h);

	u.col(0) = x0.head_rows(eqnCount);
	v.col(0) = x0.tail_rows(eqnCount);
	a.col(0) = solve(M, -Mp * E*ag(0) - C * v.col(0) - K0 * u.col(0));

	dsp = u.col(0);
	vel = v.col(0);
	acc = a.col(0);

	setDofResponse();
	getElementResponse();
	assembleNonlinearForceVector(true);
	recordResponse();

	double agd, agi, agj;
	for (int i = 0; i < nsteps - 1; i++)
	{
		cstep += 1;
		ctime += dt * nsub;
		agd = (ag(i + 1) - ag(i)) / nsub;
		agi = ag(i);
		for (int j = 0; j < nsub; j++)
		{
			agj = agi + agd * j;
			F.tail_rows(eqnCount) = solve(M, -Mp * E*agj - q);
			x0 = T * (x0 + F * h);
			dsp = x0.head_rows(eqnCount);
			vel = x0.tail_rows(eqnCount);
			setDofResponse();
			getElementResponse();
			if (j == (nsub-1))
			{
				recordResponse();
			}
			assembleNonlinearForceVector(true);
		}
		u.col(i + 1) = x0.head_rows(eqnCount);
		v.col(i + 1) = x0.tail_rows(eqnCount);
		a.col(i + 1) = solve(M, -Mp * E*agj - q - C * v.col(i + 1) - K0 * u.col(i + 1));

	}
	saveResponse();
}

void dsystem::solveTimeDomainSeismicResponseRK4(const int tsId, const double s, const int nsub)
{
	int nstep = tss.at(tsId)->nsteps;
	double dt = tss.at(tsId)->dt;
	vec ag = s * tss.at(tsId)->series;

	vec u0 = zeros<vec>(eqnCount);
	vec v0 = zeros<vec>(eqnCount);
	vec a0 = zeros<vec>(eqnCount);

	u = zeros<mat>(eqnCount, nstep);
	v = zeros<mat>(eqnCount, nstep);
	a = zeros<mat>(eqnCount, nstep);

	dt = dt / nsub;
	element::dt = dt;
}

void dsystem::setDofResponse()
{
	for (int i = 0; i < eqnCount; i++)
	{
		dof *d = dofs.at(eqnMapDof.at(i));
		d->dsp = dsp(i);
		d->vel = vel(i);
		d->acc = acc(i);
	}
}

void dsystem::getElementResponse()
{
	if (!(eles.empty()))
	{
		std::map<int, element *>::iterator it;
		for (it = eles.begin(); it != eles.end(); it++)
		{
			element *e = it->second;
			e->getResponse();
		}
	}
}

void dsystem::assembleNonlinearForceVector(const bool update)
{
	q = zeros<vec>(eqnCount);

	if (!(springBLs.empty()))
	{
		std::map<int, springBilinear *>::iterator it;
		for (it = springBLs.begin(); it != springBLs.end(); it++)
		{
			springBilinear *s = it->second;
			s->getResponse(update);
			s->assembleNonlinearForceVector(q);
		}
	}

	if (!(springBWs.empty()))
	{
		std::map<int, springBoucWen *>::iterator it;
		for (it = springBWs.begin(); it != springBWs.end(); it++)
		{
			springBoucWen *s = it->second;
			s->getResponse(update);
			s->assembleNonlinearForceVector(q);
		}
	}

	if (!(dashpotExps.empty()))
	{
		std::map<int, dashpotExp *>::iterator it;
		for (it = dashpotExps.begin(); it != dashpotExps.end(); it++)
		{
			dashpotExp *s = it->second;
			s->getResponse(update);
			s->assembleNonlinearForceVector(q);
		}
	}
	
	if (!(sliders.empty()))
	{
		std::map<int, slider *>::iterator it;
		for (it = sliders.begin(); it != sliders.end(); it++)
		{
			slider *s = it->second;
			s->getResponse(update);
			s->assembleNonlinearForceVector(q);
		}
	}
}

void dsystem::initRecorders()
{
	if (!(drs.empty()))
	{
		std::map<int, recorder *>::iterator it;
		for (it = drs.begin(); it != drs.end(); it++)
		{
			recorder *dr = it->second;
			dr->init(nsteps);
		}
	}

	if (!(ers.empty()))
	{
		std::map<int, recorder *>::iterator it;
		for (it = ers.begin(); it != ers.end(); it++)
		{
			recorder *er = it->second;
			er->init(nsteps);
		}
	}
}

void dsystem::recordResponse()
{
	if (!(drs.empty()))
	{
		std::map<int, recorder *>::iterator it;
		for (it = drs.begin(); it != drs.end(); it++)
		{
			recorder *dr = it->second;
			dr->record(cstep, ctime);
		}
	}

	if (!(ers.empty()))
	{
		std::map<int, recorder *>::iterator it;
		for (it = ers.begin(); it != ers.end(); it++)
		{
			recorder *er = it->second;
			er->record(cstep, ctime);
		}
	}
}

void dsystem::saveResponse()
{
	if (!(drs.empty()))
	{
		std::map<int, recorder *>::iterator it;
		for (it = drs.begin(); it != drs.end(); it++)
		{
			recorder *dr = it->second;
			dr->save();
		}
	}

	if (!(ers.empty()))
	{
		std::map<int, recorder *>::iterator it;
		for (it = ers.begin(); it != ers.end(); it++)
		{
			recorder *er = it->second;
			er->save();
		}
	}
}