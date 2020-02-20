#include "DynamicSystem.h"
#include "numeric.h"

#include <fstream>
#include <vector>

#include "material/elastic.h"
#include "material/elastoplastic.h"
#include "material/steelBilinear.h"
#include "material/concreteTrilinear.h"
#include "material/SMABilinear.h"

DynamicSystem::DynamicSystem(const double z) :
	zeta(z), eqnCount(0), fixedDofCount(0), eigenVectorNormed(false),
	dynamicSolver(StateSpace), dt(0.02), ctime(0.0), nsteps(0), cstep(0),
	useRayleighDamping(true), RayleighOmg1(2 * PI / 0.3), RayleighOmg2(2 * PI / 0.1),
	NumModesInherentDamping(-1)
{
}

DynamicSystem::~DynamicSystem()
{
}

void DynamicSystem::addNode(node * nd)
{
	auto it = nodes.insert(std::make_pair(nd->id, nd));
	if (!it.second)
	{
		cout << "Node ID: " << nd->id << " already exists! The node will not be added." << endl;
	}
}

void DynamicSystem::addNode(const int id, const double x, const int dofId)
{
	dof *d = dofs.at(dofId);
	node *nd = new node(id, x);
	nd->setDof(d);
	addNode(nd);
}

void DynamicSystem::addNode(const int id, const double x, const double y, const double z)
{
	node *nd = new node(id, x, y, z);
	addNode(nd);
}

void DynamicSystem::addNode(const int id, const double x, const double z, const int dofXId, const int dofZId, const int dofRYId)
{
	dof *dx = dofs.at(dofXId);
	dof *dz = dofs.at(dofZId);

	node *nd = new node(id, x, 0.0, z);
	nd->setDof(dx);
	nd->setDof(dz);

	if (dofRYId >= 0)
	{
		nd->setDof(dofs.at(dofRYId));
	}

	addNode(nd);
}

void DynamicSystem::addNodeWithDof(const int id, const double x, const int dofId)
{
	dof *d = new dof(dofId, X);
	node *nd = new node(id, x, 0.0, 0.0);
	nd->setDof(d);

	addDof(d);
	addNode(nd);
}

void DynamicSystem::addLine(line *l)
{
	auto it = lines.insert(std::make_pair(l->id, l));
	if (!it.second)
	{
		cout << "Line ID: " << l->id << " already exists! The line will not be added." << endl;
	}
}

void DynamicSystem::addLine(const int id, const int ni, const int nj)
{
	line *l = new line(id, nodes.at(ni), nodes.at(nj));
	addLine(l);
}

void DynamicSystem::fixDof(const int id)
{
	dofs.at(id)->isFixed = true;
}

void DynamicSystem::fixNode(const int id)
{
	nodes.at(id)->fixDof();
}

void DynamicSystem::addLoad(const int id, double* t, double* p, const int nP, const double arriveTime,
	const double scale)
{
	if (Loads.count(id) == 0)
	{
		Load *load = new Load(id, t, p, nP, arriveTime, scale);
		Loads[id] = load;
	}
	else
	{
		cout << "Load ID: " << id << " already exists! The load will not be added." << endl;
	}
}

void DynamicSystem::addDofLoad(const int dofId, const int loadId)
{
	dofs.at(dofId)->addLoad(Loads.at(loadId));
}

void DynamicSystem::exportGmsh(char * fileName)
{
	std::ofstream outFile;
	outFile.open(fileName, ios::out);

	outFile << "$MeshFormat\n2.2 0 8\n$EndMeshFormat" << std::endl;
	outFile << "$Nodes" << std::endl;
	outFile << nodes.size() << std::endl;
	for (auto it = nodes.begin(); it != nodes.end(); ++it)
	{
		auto nd = it->second;
		outFile << it->first << " " << nd->x0 << " " << nd->y0 << " " << nd->z0 << std::endl;
	}
	outFile << "$EndNodes" << std::endl;

	if (ele2Ds.size() > 1)
	{
		outFile << "$Elements" << std::endl;
		outFile << ele2Ds.size() << std::endl;
		for (auto it = ele2Ds.begin(); it != ele2Ds.end(); ++it)
		{
			auto ele = it->second;
			outFile << it->first << " 1 1 1 " << ele->nodeI->id << " " << ele->nodeJ->id << std::endl;
		}
		outFile << "$EndElements" << std::endl;
	}

	if (Plane2Ds.size() > 1)
	{
		outFile << "$Elements" << std::endl;
		outFile << Plane2Ds.size() << std::endl;
		for (auto it = Plane2Ds.begin(); it != Plane2Ds.end(); it++)
		{
			auto ele = it->second;
			outFile << it->first << " 3 1 1 " << ele->nodeI->id << " " << ele->nodeJ->id << " "
				<< ele->nodeP->id << " " << ele->nodeQ->id << std::endl;
		}
		outFile << "$EndElements" << std::endl;
	}

	outFile << "$NodeData" << std::endl;

	vec modeshape = Phi.col(0);
	for (int i = 0; i < eqnCount; i++)
	{
		dof *d = dofs.at(eqnMapDof.at(i));
		d->dsp = modeshape(i);
	}

	outFile << "1" << std::endl;
	outFile << "\"First Mode: T = " << P(0) << "s\"" << std::endl;
	outFile << "1" << std::endl;
	outFile << "0.0" << std::endl;
	outFile << "3" << std::endl;
	outFile << "0" << std::endl;
	outFile << "3" << std::endl;
	outFile << nodes.size() << std::endl;

	for (auto it = nodes.begin(); it != nodes.end(); it++)
	{
		auto nd = it->second;
		outFile << it->first << " " << nd->dofX->dsp << " " << 0.0 << " " << nd->dofZ->dsp << std::endl;
	}

	outFile << "$EndNodeData" << std::endl;

	outFile.close();

	dof *d = nullptr;
	for (int i = 0; i < eqnCount; i++)
	{
		d = dofs.at(eqnMapDof.at(i));
		d->dsp = 0.0;
	}
}

void DynamicSystem::addDof(dof * d)
{
	if (dofs.count(d->id) == 0)
	{
		dofs[d->id] = d;
	}
	else
	{
		cout << "dof ID: " << d->id << " already exists! The dof will not be added." << endl;
	}
}

void DynamicSystem::addDof(const int id, const double m, const bool fixed)
{
	dof *d = new dof(id, X, m, fixed);
	addDof(d);
}

void DynamicSystem::addDof(const int id, direction dir, const double m, const bool fixed)
{
	dof *d = new dof(id, dir, m, fixed);
	addDof(d);
}

void DynamicSystem::setMass(const int id, const double m)
{
	dofs.at(id)->mass = m;
}

void DynamicSystem::setNodeMass(const int id, const double m)
{
	node *nd = nodes.at(id);
	if (nd->isActivated(X))
	{
		nd->dofX->mass = m;
	}
	if (nd->isActivated(Z))
	{
		nd->dofZ->mass = m;
	}
	if (nd->isActivated(RY))
	{
		nd->dofRY->mass = m;
	}
}

void DynamicSystem::mapDofNode(dof * d, node * nd)
{
	nd->setDof(d);
	dofMapNode[d->id] = nd->id;
}

void DynamicSystem::mapDofNode(const int id_d, const int id_nd)
{
	node *nd = nodes.at(id_nd);
	dof *d = dofs.at(id_d);
	nd->setDof(d);
	dofMapNode[id_d] = id_nd;
}

bool DynamicSystem::addMaterial1D(material1D * mt)
{
	if (material1Ds.count(mt->id) == 0)
	{
		material1Ds[mt->id] = mt;
		return true;
	}
	else
	{
		cout << "material1D ID: " << mt->id << " already exists! The material will not be added." << endl;
		return false;
	}
}

bool DynamicSystem::addMaterialElastic(const int id, const double E0)
{
	material1D *mtrl = new elastic(id, E0);
	return addMaterial1D(mtrl);
}

bool DynamicSystem::addMaterialElastoplastic(const int id, const double E0, const double fy, const double alpha)
{
	material1D *mtrl = new elastoplastic(id, E0, fy, alpha);
	return addMaterial1D(mtrl);
}

bool DynamicSystem::addMaterialSteelBilinear(const int id, const double E0, const double fy, const double alpha, const double beta)
{
	material1D *mtrl = new steelBilinear(id, E0, fy, alpha, beta);
	return addMaterial1D(mtrl);
}

bool DynamicSystem::addMaterialConcreteTrilinear(const int id, const double E0, const double fc, const double epsilon_c, const double sigma_cr, const double sigma_u, const double epsilon_u)
{
	material1D *mtrl = new concreteTrilinear(id, E0, fc, epsilon_c, sigma_cr, sigma_u, epsilon_u);
	return addMaterial1D(mtrl);
}

bool DynamicSystem::addMaterialSMABilinear(const int id, const double E0, const double fy, const double alpha, const double sigma_shift)
{
	material1D *mtrl = new SMABilinear(id, E0, fy, alpha, sigma_shift);
	return addMaterial1D(mtrl);
}

bool DynamicSystem::addFiber(const int id, const int matId, const double A, const double y, const double z)
{
	if (Fibers.count(id) == 0)
	{
		Fiber *fiber = new Fiber(id, material1Ds.at(matId), A, y, z);
		Fibers[id] = fiber;
		return true;
	}
	else
	{
		cout << "Fiber ID: " << id << " already exists! The fiber will not be added." << endl;
		return false;
	}
}

bool DynamicSystem::addSectionTruss(const int id, int* fiberIds, const int nFibers)
{
	if (Sections.count(id) == 0)
	{
		vector<Fiber *> fibers(0);
		for (int i = 0; i < nFibers; ++i)
		{
			fibers.push_back(Fibers.at(fiberIds[i]));
		}
		SectionTruss *sec = new SectionTruss(id, fibers);
		Sections[id] = sec;
		SectionTrusss[id] = sec;
		return true;
	}
	else
	{
		cout << "Section ID: " << id << " already exists! The section will not be added." << endl;
		return false;
	}
}

bool DynamicSystem::addSectionFrame2D(const int id, int* fiberIds, const int nFibers)
{
	if (Sections.count(id) == 0)
	{
		vector<Fiber *> fibers(0);
		for (int i = 0; i < nFibers; ++i)
		{
			fibers.push_back(Fibers.at(fiberIds[i]));
		}
		SectionFrame2D *sec = new SectionFrame2D(id, fibers);
		Sections[id] = sec;
		SectionFrame2Ds[id] = sec;
		return true;
	}
	else
	{
		cout << "Section ID: " << id << " already exists! The section will not be added." << endl;
		return false;
	}
}

bool DynamicSystem::addElement(element * e)
{
	if (eles.count(e->id) == 0)
	{
		eles[e->id] = e;
		return true;
	}
	else
	{
		cout << "element ID: " << e->id << " already exists! The element will not be added." << endl;
		return false;
	}
}

void DynamicSystem::addSpring(spring *s)
{
	if (addElement(s)) {
		springs[s->id] = s;
		linearElasticElements[s->id] = s;
	}
}

void DynamicSystem::addSpring(const int id, const int ni, const int nj, const double k)
{
	dof *i = dofs.at(ni);
	dof *j = dofs.at(nj);
	spring *s = new spring(id, i, j, k);
	addSpring(s);
}

void DynamicSystem::addSpringBilinear(springBilinear * s)
{
	if (addElement(s))
	{
		springBLs[s->id] = s;
		nonlinearTangentElements[s->id] = s;
	}
}

void DynamicSystem::addSpringBilinear(const int id, const int ni, const int nj, const double k0, const double uy, const double alpha)
{
	dof *i = dofs.at(ni);
	dof *j = dofs.at(nj);
	springBilinear *s = new springBilinear(id, i, j, k0, uy, alpha);
	addSpringBilinear(s);
}

void DynamicSystem::addSpringNonlinear(springNonlinear * s)
{
	if (addElement(s))
	{
		springNLs[s->id] = s;
		nonlinearTangentElements[s->id] = s;
	}
}

void DynamicSystem::addSpringNonlinear(const int id, const int ni, const int nj, const int matId)
{
	dof *i = dofs.at(ni);
	dof *j = dofs.at(nj);
	springNonlinear *s = new springNonlinear(id, i, j, material1Ds.at(matId));
	addSpringNonlinear(s);
}

void DynamicSystem::addSpringBoucWen(springBoucWen * s)
{
	if (addElement(s))
	{
		springBWs[s->id] = s;
		nonlinearTangentElements[s->id] = s;
	}
}

void DynamicSystem::addSpringBoucWen(const int id, const int ni, const int nj, const double k0, const double uy, const double alpha, const double beta, const double n)
{
	dof *i = dofs.at(ni);
	dof *j = dofs.at(nj);
	springBoucWen *s = new springBoucWen(id, i, j, k0, uy, alpha);
	addSpringBoucWen(s);
}

void DynamicSystem::addDashpot(dashpot * d)
{
	if (addElement(d))
	{
		dashpots[d->id] = d;
		linearDampingElements[d->id] = d;
	}
}

void DynamicSystem::addDashpot(const int id, const int ni, const int nj, const double c)
{
	dof *i = dofs.at(ni);
	dof *j = dofs.at(nj);
	dashpot *d = new dashpot(id, i, j, c);
	addDashpot(d);
}

void DynamicSystem::addDashpotExp(dashpotExp * d)
{
	if (addElement(d))
	{
		dashpotExps[d->id] = d;
		nonlinearElements[d->id] = d;
	}
}

void DynamicSystem::addDashpotExp(const int id, const int ni, const int nj, const double c, const double alpha)
{
	dof *i = dofs.at(ni);
	dof *j = dofs.at(nj);
	dashpotExp *d = new dashpotExp(id, i, j, c, alpha);
	addDashpotExp(d);
}

void DynamicSystem::addDashpotMaxwell(dashpotMaxwell * d)
{
	if (addElement(d))
	{
		dashpotMaxwells[d->id] = d;
		nonlinearElements[d->id] = d;
	}
}

void DynamicSystem::addDashpotMaxwell(const int id, const int ni, const int nj, const double k, const double c, const double alpha)
{
	dof *i = dofs.at(ni);
	dof *j = dofs.at(nj);
	dashpotMaxwell *d = new dashpotMaxwell(id, i, j, c, alpha);
	addDashpotMaxwell(d);
}

void DynamicSystem::addInerter(inerter * in)
{
	if (addElement(in))
	{
		inerters[in->id] = in;
		inertialMassElements[in->id] = in;
	}
}

void DynamicSystem::addInerter(const int id, const int ni, const int nj, const double m)
{
	dof *i = dofs.at(ni);
	dof *j = dofs.at(nj);
	inerter *in = new inerter(id, i, j, m);
	addInerter(in);
}

void DynamicSystem::addSlider(slider * s)
{
	if (addElement(s))
	{
		sliders[s->id] = s;
		nonlinearElements[s->id] = s;
	}
}

void DynamicSystem::addSlider(const int id, const int ni, const int nj, const double muN)
{
	dof *i = dofs.at(ni);
	dof *j = dofs.at(nj);
	slider *s = new slider(id, i, j, muN);
	addSlider(s);
}

void DynamicSystem::addSPIS2(spis2 * s)
{
	if (addElement(s))
	{
		spis2s[s->id] = s;
		linearElasticElements[s->id] = s;
		linearDampingElements[s->id] = s;
		inertialMassElements[s->id] = s;
	}
}

void DynamicSystem::addSPIS2(const int id, const int ni, const int nj, const int nin, const double m, const double c, const double k)
{
	dof *i = dofs.at(ni);
	dof *j = dofs.at(nj);
	dof *in = dofs.at(nin);
	spis2 *s = new spis2(id, i, j, in, m, c, k);
	addSPIS2(s);
}

void DynamicSystem::addTVMD(TVMD *d)
{
	if (addElement(d))
	{
		TVMDs[d->id] = d;
		nonlinearElements[d->id] = d;
	}
}

void DynamicSystem::addTVMD(const int id, const int ni, const int nj, const double m, const double c, const double k)
{
	dof *i = dofs.at(ni);
	dof *j = dofs.at(nj);
	TVMD *d = new TVMD(id, i, j, m, c, k);
	addTVMD(d);
}

void DynamicSystem::addSpring2D(spring2D *s)
{
	if (addElement(s))
	{
		ele2Ds[s->id] = s;
		spring2Ds[s->id] = s;
		linearElasticElements[s->id] = s;
	}
}

void DynamicSystem::addSpring2D(const int id, const int ni, const int nj, const double k, ELE::localAxis U)
{
	spring2D *s = new spring2D(id, nodes.at(ni), nodes.at(nj), k, U);
	addSpring2D(s);
}

void DynamicSystem::addSpringBoucWen2D(springBoucWen2D *s)
{
	if (addElement(s))
	{
		ele2Ds[s->id] = s;
		springBoucWen2Ds[s->id] = s;
		nonlinearTangentElements[s->id] = s;
	}
}

void DynamicSystem::addSpringBoucWen2D(const int id, const int ni, const int nj, const double k0, const double uy, const double alpha, const double beta, const double n, ELE::localAxis U /*= ELE::U1*/)
{
	springBoucWen2D *s = new springBoucWen2D(id, nodes.at(ni), nodes.at(nj), k0, uy, alpha, beta, n, U);
	addSpringBoucWen2D(s);
}

void DynamicSystem::addDashpot2D(dashpot2D *s)
{
	if (addElement(s))
	{
		ele2Ds[s->id] = s;
		dashpot2Ds[s->id] = s;
		linearDampingElements[s->id] = s;
	}
}

void DynamicSystem::addDashpot2D(const int id, const int ni, const int nj, const double c, ELE::localAxis U)
{
	dashpot2D *s = new dashpot2D(id, nodes.at(ni), nodes.at(nj), c, U);
	addDashpot2D(s);
}

void DynamicSystem::addInerter2D(inerter2D *s)
{
	if (addElement(s))
	{
		ele2Ds[s->id] = s;
		inerter2Ds[s->id] = s;
		inertialMassElements[s->id] = s;
	}
}

void DynamicSystem::addInerter2D(const int id, const int ni, const int nj, const double m, ELE::localAxis U)
{
	inerter2D *s = new inerter2D(id, nodes.at(ni), nodes.at(nj), m, U);
	addInerter2D(s);
}

void DynamicSystem::addDashpotExp2D(dashpotExp2D *s)
{
	if (addElement(s))
	{
		ele2Ds[s->id] = s;
		dashpotExp2Ds[s->id] = s;
		nonlinearElements[s->id] = s;
	}
}

void DynamicSystem::addDashpotExp2D(const int id, const int ni, const int nj, const double c, const double alpha, ELE::localAxis U /*= ELE::U1*/)
{
	dashpotExp2D *s = new dashpotExp2D(id, nodes.at(ni), nodes.at(nj), c, alpha, U);
	addDashpotExp2D(s);
}

void DynamicSystem::addDashpotMaxwell2D(dashpotMaxwell2D *s)
{
	if (addElement(s))
	{
		ele2Ds[s->id] = s;
		dashpotMaxwell2Ds[s->id] = s;
		nonlinearElements[s->id] = s;
	}
}

void DynamicSystem::addDashpotMaxwell2D(const int id, const int ni, const int nj, const double k, const double c, const double alpha, ELE::localAxis U)
{
	dashpotMaxwell2D *s = new dashpotMaxwell2D(id, nodes.at(ni), nodes.at(nj), k, c, alpha, U);
	addDashpotMaxwell2D(s);
}

void DynamicSystem::addTrussElastic(trussElastic *truss)
{
	if (addElement(truss)) {
		ele2Ds[truss->id] = truss;
		trussElastics[truss->id] = truss;
		linearElasticElements[truss->id] = truss;
	}
}

void DynamicSystem::addTrussElastic(const int id, const int ni, const int nj, const double EA)
{
	trussElastic *truss = new trussElastic(id, nodes.at(ni), nodes.at(nj), EA);
	addTrussElastic(truss);
}

void DynamicSystem::addBeamElastic(beamElastic *beam)
{
	if (addElement(beam)) {
		ele2Ds[beam->id] = beam;
		beamElastics[beam->id] = beam;
		linearElasticElements[beam->id] = beam;
	}
}

void DynamicSystem::addBeamElastic(const int id, const int ni, const int nj, const double EI)
{
	beamElastic *beam = new beamElastic(id, nodes.at(ni), nodes.at(nj), EI);
	addBeamElastic(beam);
}

void DynamicSystem::addFrameElastic(FrameElastic2D *frame)
{
	if (addElement(frame)) {
		ele2Ds[frame->id] = frame;
		FrameElastic2Ds[frame->id] = frame;
		linearElasticElements[frame->id] = frame;
	}
}

void DynamicSystem::addFrameElastic(const int id, const int ni, const int nj, const double EA, const double EI)
{
	FrameElastic2D *frame = new FrameElastic2D(id, nodes.at(ni), nodes.at(nj), EA, EI);
	addFrameElastic(frame);
}

void DynamicSystem::addQuad4Elastic(Quad4Elastic* quad)
{
	if (addElement(quad)) {
		Quad4Elastics[quad->id] = quad;
		Plane2Ds[quad->id] = quad;
		linearElasticElements[quad->id] = quad;
	}
}

void DynamicSystem::addQuad4Elastic(const int id, const int nodeI, const int nodeJ, const int nodeP, const int nodeQ,
	const double E, const double nu, const double t)
{
	Quad4Elastic *quad = new Quad4Elastic(id, nodes.at(nodeI), nodes.at(nodeJ), nodes.at(nodeP), nodes.at(nodeQ), E, nu, t);
	addQuad4Elastic(quad);
}

void DynamicSystem::addTruss2D(const int id, const int ni, const int nj, const int secId)
{
	if (eles.count(id) == 0)
	{
		Truss2D *truss = new Truss2D(id, nodes.at(ni), nodes.at(nj), SectionTrusss.at(secId));
		eles[id] = truss;
		ele2Ds[id] = truss;
		Truss2Ds[id] = truss;
		nonlinearInitialTangentElements[id] = truss;
	}
	else
	{
		cout << "element ID: " << id << " already exists! The element will not be added." << endl;
	}
}

void DynamicSystem::addFrame2D(const int id, const int ni, const int nj, const int secId, const int nIntP)
{
	if (eles.count(id) == 0)
	{
		Frame2D *frame = new Frame2D(id, nodes.at(ni), nodes.at(nj), SectionFrame2Ds.at(secId), nIntP);
		eles[id] = frame;
		ele2Ds[id] = frame;
		Frame2Ds[id] = frame;
		nonlinearInitialTangentElements[id] = frame;
	}
	else
	{
		cout << "element ID: " << id << " already exists! The element will not be added." << endl;
	}
}

void DynamicSystem::addFramePDelta2D(const int id, const int ni, const int nj, const int secId, const int nIntP)
{
	if (eles.count(id) == 0)
	{
		FramePDelta2D *frame = new FramePDelta2D(id, nodes.at(ni), nodes.at(nj), SectionFrame2Ds.at(secId), nIntP);
		eles[id] = frame;
		ele2Ds[id] = frame;
		FramePDelta2Ds[id] = frame;
		nonlinearInitialTangentElements[id] = frame;
	}
	else
	{
		cout << "element ID: " << id << " already exists! The element will not be added." << endl;
	}
}

void DynamicSystem::addTimeSeries(TimeSeries * ts)
{
	tss[ts->id] = ts;
}

void DynamicSystem::addTimeSeries(const int id, const double dt, const vec &s)
{
	TimeSeries *ts = new TimeSeries(id, dt, s);
	addTimeSeries(ts);
}

void DynamicSystem::addTimeSeries(const int id, const double dt, char* fileName)
{
	TimeSeries *ts = new TimeSeries(id, dt, fileName);
	addTimeSeries(ts);
}

void DynamicSystem::addDofRecorder(dofRecorder * dr)
{
	drs[dr->id] = dr;
}

void DynamicSystem::addDofRecorder(const int id, int *dofIds, const int n, response rtype, char * fileName)
{
	std::vector<dof *> rdofs(n);

	for (int i = 0; i < n; i++)
	{
		rdofs[i] = dofs.at(dofIds[i]);
	}

	dofRecorder *dr = new dofRecorder(id, rdofs, rtype, fileName);
	addDofRecorder(dr);
}

void DynamicSystem::addElementRecorder(elementRecorder * er)
{
	ers[er->id] = er;
}

void DynamicSystem::addElementRecorder(const int id, int * eleIds, const int n, response rtype, char * fileName)
{
	std::vector<element *> reles(n);

	for (int i = 0; i < n; i++)
	{
		reles[i] = eles.at(eleIds[i]);
	}

	elementRecorder *er = new elementRecorder(id, reles, rtype, fileName);
	addElementRecorder(er);
}

void DynamicSystem::addSpringRecorder(const int id, int * eleIds, const int n, response rtype, char * fileName)
{
	std::vector<element *> reles(n);

	for (int i = 0; i < n; i++)
	{
		reles[i] = springs.at(eleIds[i]);
	}

	elementRecorder *er = new elementRecorder(id, reles, rtype, fileName);
	addElementRecorder(er);
}

void DynamicSystem::addDashpotRecorder(const int id, int * eleIds, const int n, response rtype, char * fileName)
{
	std::vector<element *> reles(n);

	for (int i = 0; i < n; i++)
	{
		reles[i] = dashpots.at(eleIds[i]);
	}

	elementRecorder *er = new elementRecorder(id, reles, rtype, fileName);
	addElementRecorder(er);
}

void DynamicSystem::addInerterRecorder(const int id, int * eleIds, const int n, response rtype, char * fileName)
{
	std::vector<element *> reles(n);

	for (int i = 0; i < n; i++)
	{
		reles[i] = inerters.at(eleIds[i]);
	}

	elementRecorder *er = new elementRecorder(id, reles, rtype, fileName);
	addElementRecorder(er);
}

void DynamicSystem::setRayleighDamping(const double omg1, const double omg2)
{
	useRayleighDamping = true;
	RayleighOmg1 = omg1;
	RayleighOmg2 = omg2;
}

void DynamicSystem::activeGroundMotion(direction dir)
{
	for (int i = 0; i < eqnCount; i++)
	{
		dof *d = dofs.at(eqnMapDof.at(i));
		if (d->dir == dir)
		{
			E(i) = 1.0;
		}
	}
}

void DynamicSystem::buildDofEqnMap()
{
	std::map<int, dof *>::iterator it;
	eqnCount = 0;
	fixedDofCount = 0;
	dofMapEqn.clear();
	eqnMapDof.clear();

	for (it = dofs.begin(); it != dofs.end(); it++)
	{
		dof *d = it->second;
		if ((d->isFixed)) fixedDofCount += 1;

		dofMapEqn[d->id] = eqnCount;
		d->eqnId = eqnCount;
		eqnMapDof[eqnCount] = d->id;
		eqnCount += 1;
	}
}

void DynamicSystem::assembleMatrix()
{
	assembleMassMatrix();
	assembleStiffnessMatrix();
	if (useRayleighDamping)
	{
		buildRayleighDampingMatrix(RayleighOmg1, RayleighOmg2);
	}
	else
	{
		buildInherentDampingMatrix();
	}
	assembleDampingMatrix();
	applyRestraint();
	applyLoad();

	dsp = zeros<vec>(eqnCount);
	vel = zeros<vec>(eqnCount);
	acc = zeros<vec>(eqnCount);
}

void DynamicSystem::assembleMassMatrix()
{
	buildDofEqnMap();
	vec m(eqnCount);
	for (int i = 0; i < eqnCount; i++)
	{
		m(i) = dofs.at(eqnMapDof[i])->mass;
	}

	Mp = diagmat(m);
	M = diagmat(m);

	if (!(inertialMassElements.empty()))
	{
		for (auto it = inertialMassElements.begin(); it != inertialMassElements.end(); it++)
		{
			auto *in = it->second;
			in->assembleMassMatrix(M);
		}
	}

	if (!(physicalMassElements.empty()))
	{
		for (auto it = physicalMassElements.begin(); it != physicalMassElements.end(); it++)
		{
			auto *in = it->second;
			in->assembleMassMatrix(Mp);
		}
	}
}

void DynamicSystem::applyRestraint()
{
	eqnCount = 0;
	dofMapEqn.clear();
	eqnMapDof.clear();

	fixedIds = zeros<uvec>(fixedDofCount);
	int fixedDofCount_ = 0;

	for (auto it = dofs.begin(); it != dofs.end(); ++it)
	{
		dof *d = it->second;
		if ((d->isFixed))
		{
			fixedIds(fixedDofCount_) = d->eqnId;
			fixedDofCount_ += 1;
			d->eqnId = -1;
		}
		else
		{
			dofMapEqn[d->id] = eqnCount;
			d->eqnId = eqnCount;
			eqnMapDof[eqnCount] = d->id;
			eqnCount += 1;
		}
	}

	M.shed_cols(fixedIds);
	M.shed_rows(fixedIds);
	Mp.shed_cols(fixedIds);
	Mp.shed_rows(fixedIds);
	K.shed_cols(fixedIds);
	K.shed_rows(fixedIds);
	K0.shed_cols(fixedIds);
	K0.shed_rows(fixedIds);
	C.shed_cols(fixedIds);
	C.shed_rows(fixedIds);

	E = zeros<vec>(eqnCount); // Ground motion vector
	Q = zeros<vec>(eqnCount); // Nodal load vector
	Q0 = zeros<vec>(eqnCount); // Const Nodal load vector
}

void DynamicSystem::applyLoad()
{
	for (int i = 0; i < eqnCount; i++)
	{
		dof *d = dofs.at(eqnMapDof.at(i));
		Q(i) = d->getLoad(ctime);
	}
}

void DynamicSystem::setLoadConst(const bool isConst)
{
	Load* load = nullptr;
	for (auto it = Loads.begin(); it != Loads.end(); ++it)
	{
		load = it->second;
		load->setConst(isConst);
	}

	if (isConst)
	{
		dsp0 = dsp;
		q0 = q;
		Q0 = Q;
	}
	else
	{
		dsp0 = zeros<vec>(eqnCount);
		q0 = zeros<vec>(eqnCount);
		Q0 = zeros<vec>(eqnCount);
	}
}

void DynamicSystem::assembleStiffnessMatrix()
{
	K = zeros<mat>(eqnCount, eqnCount);

	if (!(linearElasticElements.empty()))
	{
		for (auto it = linearElasticElements.begin(); it != linearElasticElements.end(); it++)
		{
			auto *s = it->second;
			s->assembleStiffnessMatrix(K);
		}
	}

	if (!(nonlinearSecantElements.empty()))
	{
		for (auto it = nonlinearSecantElements.begin(); it != nonlinearSecantElements.end(); it++)
		{
			auto *s = it->second;
			s->assembleStiffnessMatrix(K);
		}
	}

	if (!(nonlinearInitialTangentElements.empty()))
	{
		for (auto it = nonlinearInitialTangentElements.begin(); it != nonlinearInitialTangentElements.end(); ++it)
		{
			auto *s = it->second;
			s->assembleInitialStiffnessMatrix(K);
		}
	}

	K0 = K;

	if (!(nonlinearTangentElements.empty()))
	{
		for (auto it = nonlinearTangentElements.begin(); it != nonlinearTangentElements.end(); it++)
		{
			auto *s = it->second;
			s->assembleStiffnessMatrix(K);
		}
	}
}

void DynamicSystem::reassembleStiffnessMatrix()
{
	auto nDof = dofs.size();
	int eqnId = 0;
	for (auto it = dofs.begin(); it != dofs.end(); ++it)
	{
		it->second->eqnId = eqnId;
		eqnId += 1;
	}

	K = zeros<mat>(nDof, nDof);

	if (!(linearElasticElements.empty()))
	{
		for (auto it = linearElasticElements.begin(); it != linearElasticElements.end(); it++)
		{
			auto *s = it->second;
			s->assembleStiffnessMatrix(K);
		}
	}

	if (!(nonlinearSecantElements.empty()))
	{
		for (auto it = nonlinearSecantElements.begin(); it != nonlinearSecantElements.end(); it++)
		{
			auto *s = it->second;
			s->assembleStiffnessMatrix(K);
		}
	}

	if (!(nonlinearInitialTangentElements.empty()))
	{
		for (auto it = nonlinearInitialTangentElements.begin(); it != nonlinearInitialTangentElements.end(); ++it)
		{
			auto *s = it->second;
			s->assembleStiffnessMatrix(K);
		}
	}

	if (!(nonlinearTangentElements.empty()))
	{
		for (auto it = nonlinearTangentElements.begin(); it != nonlinearTangentElements.end(); it++)
		{
			auto *s = it->second;
			s->assembleStiffnessMatrix(K);
		}
	}

	K.shed_cols(fixedIds);
	K.shed_rows(fixedIds);

	for (auto it = dofMapEqn.begin(); it != dofMapEqn.end(); ++it)
	{
		dofs.at(it->first)->eqnId = it->second;
	}
}

void DynamicSystem::setNumModesInherentDamping(const int n)
{
	NumModesInherentDamping = n;
	useRayleighDamping = false;
}

void DynamicSystem::buildInherentDampingMatrix()
{
	int eigenNum = NumModesInherentDamping > 0 ? NumModesInherentDamping : eqnCount;
	if (zeta == 0.0)
	{
		C = zeros<mat>(eqnCount, eqnCount);
	}
	else
	{
		if (P.n_rows == 0)
		{
			solveEigen();
		}
		mat Phi_ = Phi.head_cols(eigenNum);
		mat MPhi = Mp * Phi_;
		if (eigenVectorNormed)
		{
			C = diagmat(2.0*zeta*omg);
			C = MPhi * C*MPhi.t();
		}
		else
		{
			C = diagmat(2.0*zeta*omg / diagvec(Phi.t()*MPhi));
			C = MPhi * C*MPhi.t();
		}
	}
}

void DynamicSystem::buildRayleighDampingMatrix(const double omg1, const double omg2)
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

void DynamicSystem::buildRayleighDampingMatrix(const int md1, const int md2)
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

void DynamicSystem::assembleDampingMatrix()
{
	if (!(linearDampingElements.empty()))
	{
		for (auto it = linearDampingElements.begin(); it != linearDampingElements.end(); it++)
		{
			auto *d = it->second;
			d->assembleDampingMatrix(C);
		}
	}
}

void DynamicSystem::solveEigen()
{
	omg = zeros<vec>(eqnCount);
	Phi = zeros<mat>(eqnCount, eqnCount);

	eig_sym(K, M, omg, Phi);

	eigenVectorNormed = true;

	P = 2.0*PI / omg;
}

void DynamicSystem::solveComplexEigen()
{
	mat O = zeros<mat>(eqnCount, eqnCount);
	mat A = join_cols(join_rows(C, M), join_rows(M, O));
	mat B = join_cols(join_rows(K, O), join_rows(O, -M));

	mat B_eig = solve(A, -B);

	cx_vec cx_lbd;
	cx_mat cx_Phi;
	eig_gen(cx_lbd, cx_Phi, B_eig);
}

void DynamicSystem::solveStochasticSeismicResponse(const double f_h, const int nf, const char method)
{
	double omg_h = 2.0*PI*f_h;
	vec omg = linspace(0, omg_h, nf + 1);
	cx_double cx_I(0.0, 1.0);

	if (method == 'c')
	{
		vec F = zeros<vec>(2 * eqnCount);
		F.head(eqnCount) = -Mp * E;

		mat O = zeros<mat>(eqnCount, eqnCount);
		mat A = join_cols(join_rows(C, M), join_rows(M, O));
		mat B = join_cols(join_rows(K, O), join_rows(O, -M));

		mat B_eig = solve(A, -B);
		cx_vec cx_lbd;
		cx_mat cx_Phi;
		eig_gen(cx_lbd, cx_Phi, B_eig);

		cx_mat A_d = cx_mat(A, zeros<mat>(2 * eqnCount, 2 * eqnCount));

		cx_vec Ad = diagvec(cx_Phi.st()*A*cx_Phi);
		cx_vec r = cx_Phi.st()*F / Ad;

		cx_mat Z = zeros<cx_mat>(2 * eqnCount, nf + 1);

		for (int i = 0; i < nf + 1; i++)
		{
			Z.col(i) = r / (omg(i)*cx_I - cx_lbd);
		}

		cx_mat Y = cx_Phi * Z;
		cx_mat X = Y.head_rows(eqnCount);
		mat Sx = real(conj(X) % X);

		dsp = sqrt(trapz(omg, Sx, 1));
	}
	else if (method == 'd')
	{
		cx_mat P = cx_vec(-Mp * E, zeros<vec>(eqnCount));

		cx_mat X = zeros<cx_mat>(eqnCount, nf + 1);
		for (int i = 0; i < nf + 1; i++)
		{
			cx_mat Q = -omg(i)*omg(i)*M + omg(i)*cx_I*C + K;
			X.col(i) = solve(Q, P);
		}
		mat Sx = real(conj(X) % X);

		dsp = sqrt(trapz(omg, Sx, 1));
	}
}

void DynamicSystem::solveLinearStaticResponse()
{
	ctime = 1.0;
	applyLoad();
	dsp = solve(K, Q);
	dsp0 = dsp;
	nsteps = 1;
	cstep = 0;
	ctime = 0;
	initRecorders();
	setDofResponse();
	getElementResponse();
	recordResponse();
	saveResponse();
}

void DynamicSystem::solveNonlinearStaticResponse(const int nsub)
{
	double tol = 1e-6;
	int maxIter = 20;
	int nIter = 0;
	double error = 1.0;

	vec du = zeros<vec>(eqnCount);

	nsteps = nsub;
	dt = 1.0/nsub;
	cstep = 0;
	ctime = 0.0;

	initRecorders();

	setDofResponse();
	assembleNonlinearForceVector();

	for (auto i=1; i<nsub+1; ++i)
	{
		cstep = i-1;
		ctime = i*dt;
		applyLoad();
		nIter = 0;
		
		while (nIter<maxIter)
		{
			du = solve(K, Q-q-K0*dsp); // solve for the correction response vector
			dsp = dsp + du; // correct the response
			setDofResponse(); // set the corrected response to the DOFs

			error = norm(du)/norm(dsp); // calculate the norm error
			if (error>tol) // not converged
			{
				if (nIter < (maxIter-1)) // not exceed max iterations
				{
					// calculate nonlinear force vector q and tangent stiffness matrix K
					// without update history variables
					assembleNonlinearForceVector(false);
					reassembleStiffnessMatrix();
				}
				else // reach max iterations
				{
					cout << "Fail to Converge after " << maxIter << " iterations, "
						<< "norm error = " << error << endl;

					// record response, update history variables,
					// calculate nonlinear force vector q and tangent stiffness matrix K,
					// then go to next substep
					// the following results maybe unreliable
					getElementResponse();
					recordResponse();
					assembleNonlinearForceVector(true);
					reassembleStiffnessMatrix();
				}
			}
			else  // converged
			{
				// record response, update history variables,
				// calculate nonlinear force vector q and tangent stiffness matrix K,
				// then break the while loop to next substep
				getElementResponse();
				recordResponse();
				assembleNonlinearForceVector(true);
				reassembleStiffnessMatrix();
				break;
			}
			nIter += 1; // increase the iteration count
		}
	}
	saveResponse();
}

void DynamicSystem::solveNonlinearStaticResponse(const double endTime, const int nsub)
{
	double tol = 1e-6;
	int maxIter = 10;
	int nIter = 0;
	double error = 1.0;

	vec du = zeros<vec>(eqnCount);

	nsteps = nsub;
	dt = endTime/nsub;

	initRecorders();

	setDofResponse();
	assembleNonlinearForceVector();

	for (auto i=1; i<nsub+1; ++i)
	{
		cstep = i-1;
		ctime = i*dt;
		applyLoad();
		nIter = 0;

		while (nIter<maxIter)
		{
			du = solve(K, Q-q-K0*dsp); // solve for the correction response vector
			dsp = dsp + du; // correct the response
			setDofResponse(); // set the corrected response to the DOFs

			error = norm(du)/norm(dsp); // calculate the norm error

			if (error>tol) // not converged
			{
				if (nIter < (maxIter-1)) // not exceed max iterations
				{
					// calculate nonlinear force vector q and tangent stiffness matrix K
					// without update history variables
					assembleNonlinearForceVector(false);
					reassembleStiffnessMatrix();
				}
				else // reach max iterations
				{
					cout << "Fail to Converge after " << maxIter << " iterations, "
						<< "norm error = " << error << endl;

					// record response, update history variables,
					// calculate nonlinear force vector q and tangent stiffness matrix K,
					// then go to next substep
					// the following results maybe unreliable
					getElementResponse();
					recordResponse();
					assembleNonlinearForceVector(true);
					reassembleStiffnessMatrix();
				}
			}
			else  // converged
			{
				// record response, update history variables,
				// calculate nonlinear force vector q and tangent stiffness matrix K,
				// then break the while loop to next substep
				getElementResponse();
				recordResponse();
				assembleNonlinearForceVector(true);
				reassembleStiffnessMatrix();
				break;
			}
			nIter += 1; // increase the iteration count
		}
	}
	saveResponse();
}

void DynamicSystem::setDispControlDof(const int dofId, const int loadId)
{
	dispControlDOFId = dofId;
	dispControlLoadId = loadId;
	dispControlEqn = dofMapEqn.at(dispControlDOFId);
}

void DynamicSystem::solveNonlinearStaticResponseDispControl(const double loadedTime, const int nsub)
{
	int dispControlEqn = dofMapEqn.at(dispControlDOFId);
	Load *load = Loads.at(dispControlLoadId);

	vec x = dsp;
	vec dx = zeros<vec>(eqnCount);
	x(dispControlEqn) = 0.0;

	ctime = loadedTime;
	vec p(size(Q));

	dof *d = nullptr;
	for (int i = 0; i < eqnCount; i++)
	{
		d = dofs.at(eqnMapDof.at(i));
		p(i) = d->getLoad(ctime, false);
	}

	mat J = K;
	J.col(dispControlEqn) = p;

	double tol = 1e-4;
	int maxIter = 20;
	int nIter = 0;
	double error = 1.0;
	double cdisp = 0.0;
	double lambda = 0.0;

	nsteps = nsub;
	dt = loadedTime/nsub;

	initRecorders();

	setDofResponse();
	assembleNonlinearForceVector();

	for (auto i=1; i<nsub+1; ++i)
	{
		cstep = i-1;
		ctime = i*dt;
		cdisp = load->getValue(ctime);
		dsp(dispControlEqn) = cdisp;
		dsp0 = dsp;
		nIter = 0;

		while (nIter<maxIter)
		{
			lambda = x(dispControlEqn);
			dx = solve(J, lambda*p - q - K0*dsp + Q0); // solve for the correction response vector
			x = x + dx;
			dsp = x;
			dsp(dispControlEqn) = cdisp;

			setDofResponse(); // set the corrected response to the DOFs

			error = norm(dx)/norm(x); // calculate the norm error

			if (error>tol) // not converged
			{
				if (nIter < (maxIter-1)) // not exceed max iterations
				{
					// calculate nonlinear force vector q and tangent stiffness matrix K
					// without update history variables
					assembleNonlinearForceVector(false);
					reassembleStiffnessMatrix();
					J = K;
					J.col(dispControlEqn) = -p;
				}
				else // reach max iterations
				{
					cout << "Time " << ctime << ": "
						<< "Fail to Converge after " << maxIter << " iterations, "
						<< "norm error = " << error << endl;

					// record response, update history variables,
					// calculate nonlinear force vector q and tangent stiffness matrix K,
					// then go to next substep
					// the following results maybe unreliable
					
					dsp = dsp0;
					getElementResponse();
					recordResponse();
					assembleNonlinearForceVector(false);
					//reassembleStiffnessMatrix();
					//J = K;
					//J.col(dispControlEqn) = -p;
					
				}
			}
			else  // converged
			{
				// record response, update history variables,
				// calculate nonlinear force vector q and tangent stiffness matrix K,
				// then break the while loop to next substep
				getElementResponse();
				recordResponse();
				assembleNonlinearForceVector(true);
				reassembleStiffnessMatrix();
				J = K;
				J.col(dispControlEqn) = -p;
				break;
			}
			nIter += 1; // increase the iteration count
		}
	}
	saveResponse();
}

void DynamicSystem::solveNonlinearStaticResponseDispControlDelta(const double loadedTime, const int nsub)
{
	Load *load = Loads.at(dispControlLoadId);

	ctime = loadedTime;
	vec p(size(Q));

	dof *d = nullptr;
	for (int i = 0; i < eqnCount; i++)
	{
		d = dofs.at(eqnMapDof.at(i));
		p(i) = d->getLoad(ctime, false);
	}

	double tol = 1e-6;
	int maxIter = 20;
	int nIter = 0;
	double error = 1.0;
	double cdisp = 0.0;
	double lambda = 0.0;
	double dlambda = 0.0;

	nsteps = nsub;
	dt = loadedTime/nsub;

	initRecorders();
	
	vec du = zeros<vec>(eqnCount); /// variation of displacement response
	vec du0 = zeros<vec>(eqnCount); /// reference variation of displacement response due to reference load and current Jacobian matrix
	vec du_ = zeros<vec>(eqnCount); /// datum vector to calculate the relative norm error
	double normDu; /// datum value to calculate the relative norm error
	
	for (auto i=1; i<nsub+1; ++i)
	{
		cstep = i-1;
		ctime = i*dt;
		cdisp = load->getValue(ctime);
		
		nIter = 0;

		du = solve(K, p);
		dlambda = (cdisp - load->getValue(ctime-dt))/du(dispControlEqn);
		du_ = du*dlambda;
		normDu = norm(du_);
		lambda += dlambda;
		dsp = dsp + du_;
		dsp0 = dsp;
		setDofResponse(); // set the corrected response to the DOFs
		assembleNonlinearForceVector(false);
		reassembleStiffnessMatrix();

		while (nIter<maxIter)
		{
			du = solve(K, lambda*p - q - K0*dsp + Q0); // solve for the correction response vector
			du0 = solve(K, p);
			dlambda = -du(dispControlEqn)/du0(dispControlEqn);
			lambda += dlambda;
			du = du + du0*dlambda;
			dsp = dsp + du;
			setDofResponse(); // set the corrected response to the DOFs

			error = norm(du)/normDu; // calculate the norm error

			if (error>tol) // not converged
			{
				if (nIter < (maxIter-1)) // not exceed max iterations
				{
					// calculate nonlinear force vector q and tangent stiffness matrix K
					// without update history variables
					assembleNonlinearForceVector(false);
					reassembleStiffnessMatrix();
				}
				else // reach max iterations
				{
					cout << "Time " << ctime << ": "
						<< "Fail to converge after " << maxIter << " iterations, "
						<< "norm error = " << error << endl;

					// set the response to the initial approximation,
					// record response,
					// do not update history variables,
					// do not calculate nonlinear force vector q and tangent stiffness matrix K,
					// then go to next substep
					// the following results maybe unreliable
					dsp = dsp0;
					getElementResponse();
					recordResponse();
					assembleNonlinearForceVector(false);
				}
			}
			else  // converged
			{
				// record response, update history variables,
				// calculate nonlinear force vector q and tangent stiffness matrix K,
				// then break the while loop to next substep
				getElementResponse();
				recordResponse();
				assembleNonlinearForceVector(true);
				reassembleStiffnessMatrix();
				break;
			}
			nIter += 1; // increase the iteration count
		}
	}
	saveResponse();
}

void DynamicSystem::solveTimeDomainSeismicResponse(const int tsId, const double s, const int nsub)
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

void DynamicSystem::solveTimeDomainSeismicResponseNMK(const int tsId, const double s, const int nsub)
{
	nsteps = tss.at(tsId)->nsteps;
	dt = tss.at(tsId)->dt;
	vec ag = s * tss.at(tsId)->series;

	vec u0(dsp);
	vec v0(vel);
	vec a0(acc);

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

	mat K_h = c1*M + c3*C + K0;

	u.col(0) = u0;
	v.col(0) = v0;
	a0 = solve(M, -Mp*E*ag(0) - C*v0);
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
	for (int i = 0; i < nsteps - 1; i++)
	{
		cstep += 1;
		ctime += dt * nsub;
		agd = (ag(i + 1) - ag(i)) / nsub;
		agi = ag(i);
		for (int j = 1; j < nsub + 1; j++)
		{
			agj = agi + agd * j;
			p_h = -Mp*E*agj + c7*u0 + c8*v0 + c9*a0 + Q0;
			u_p = u0*1.0;
			u0 = solve(K_h, p_h);
			v0 = c3*(u0 - u_p) + c4*v0 + dt*c5*a0;
			a0 = solve(M, -Mp*E*agj - C*v0 - K0*u0 + Q0);
		}

		u.col(i + 1) = u0;
		v.col(i + 1) = v0;
		a.col(i + 1) = a0;

		dsp = u.col(i + 1);
		vel = v.col(i + 1);
		acc = a.col(i + 1);

		setDofResponse();
		getElementResponse();
		recordResponse();
	}
	saveResponse();
}

void DynamicSystem::solveTimeDomainSeismicResponseNMKNL(const int tsId, const double s, const int nsub, const double tol, const int maxiter)
{
	nsteps = tss.at(tsId)->nsteps;
	dt = tss.at(tsId)->dt;
	vec ag = s * tss.at(tsId)->series;

	vec u0(dsp);
	vec v0(vel);
	vec a0(acc);

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

	mat K_h_ = c1*M + c3*C;
	mat K_h = K_h_ + K0;

	u.col(0) = u0;
	v.col(0) = v0;
	a0 = solve(M, -Mp*E*ag(0) - C*v0);
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
	double agd = 0.0, agi = 0.0, agj = 0.0;
	double error = 1.0;
	for (int i = 0; i < nsteps - 1; i++)
	{
		cstep += 1;
		ctime += dt * nsub;
		agd = (ag(i + 1) - ag(i)) / nsub;
		agi = ag(i);
		for (int j = 1; j < nsub + 1; j++)
		{
			agj = agi + agd * j;
			p_h = -Mp*E*agj + c7*u0 + c8*v0 + c9*a0;
			u_p = u0*1.0;
			v_p = v0*1.0;
			a_p = a0*1.0;
			error = 1.0;
			for (int l = 0; l < maxiter; l++)
			{
				du = solve(K_h_ + K, -K_h*u0 - q + p_h + Q0);
				u0 = u0 + du;
				v0 = c3*(u0 - u_p) + c4*v_p + dt*c5*a_p;
				a0 = solve(M, -Mp*E*agj - q - C*v0 - K0*u0 + Q0);

				dsp = u0;
				vel = v0;
				acc = a0;

				setDofResponse();
				error = norm(du) / norm(u0);
				if (error > tol)
				{
					if (l < maxiter - 1)
					{
						assembleNonlinearForceVector(false);
						reassembleStiffnessMatrix();
					}
					else
					{
						cout << "Fail to Converge after " << maxiter << " iterations, "
							<< "norm error = " << error << endl;
						if (j == (nsub - 1))
						{
							getElementResponse();
							recordResponse();
						}
						assembleNonlinearForceVector(true);
						reassembleStiffnessMatrix();
					}
				}
				else
				{
					getElementResponse();
					recordResponse();
					assembleNonlinearForceVector(true);
					reassembleStiffnessMatrix();
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

void DynamicSystem::solveTimeDomainSeismicResponseStateSpace(const int tsId, const double s, const int nsub)
{
	nsteps = tss.at(tsId)->nsteps;
	dt = tss.at(tsId)->dt;
	vec ag = s * tss.at(tsId)->series;

	vec x0 = zeros<vec>(2 * eqnCount);
	x0.head_rows(eqnCount) = dsp;
	x0.tail_rows(eqnCount) = vel;
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
	//mat T = expmat(H*h);
	mat T = expm(H, h);

	u.col(0) = x0.head_rows(eqnCount);
	v.col(0) = x0.tail_rows(eqnCount);
	a.col(0) = solve(M, -Mp*E*ag(0) - C*v.col(0));

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
			F.tail_rows(eqnCount) = solve(M, -Mp*E*agj + Q0);
			x0 = T * (x0 + F*h);
		}

		u.col(i + 1) = x0.head_rows(eqnCount);
		v.col(i + 1) = x0.tail_rows(eqnCount);
		a.col(i + 1) = solve(M, -Mp*E*agj - C*v.col(i+1) - K*u.col(i+1) + Q0);

		dsp = u.col(i + 1);
		vel = v.col(i + 1);
		acc = a.col(i + 1);

		setDofResponse();
		getElementResponse();
		recordResponse();
	}
	saveResponse();
}

void DynamicSystem::solveTimeDomainSeismicResponseStateSpaceNL(const int tsId, const double s, const int nsub)
{
	nsteps = tss.at(tsId)->nsteps;
	dt = tss.at(tsId)->dt;
	vec ag = s * tss.at(tsId)->series;

	vec x0 = zeros<vec>(2 * eqnCount);
	x0.head_rows(eqnCount) = dsp;
	x0.tail_rows(eqnCount) = vel;
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
	//mat T = expmat(H*h);
	mat T = expm(H, h);

	u.col(0) = x0.head_rows(eqnCount);
	v.col(0) = x0.tail_rows(eqnCount);
	a.col(0) = solve(M, -Mp * E*ag(0) - C * v.col(0));

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
			agj = agi + agd*j;
			F.tail_rows(eqnCount) = solve(M, -Mp*E*agj - q + Q0);
			x0 = T*(x0 + F*h);
			dsp = x0.head_rows(eqnCount);
			vel = x0.tail_rows(eqnCount);
			setDofResponse();
			getElementResponse();
			if (j == (nsub - 1))
			{
				recordResponse();
			}
			assembleNonlinearForceVector(true);
		}
		u.col(i + 1) = x0.head_rows(eqnCount);
		v.col(i + 1) = x0.tail_rows(eqnCount);
		a.col(i + 1) = solve(M, -Mp*E*agj - q - C*v.col(i+1) - K0*u.col(i+1) + Q0);
	}
	saveResponse();
}

void DynamicSystem::solveTimeDomainSeismicResponseRK4(const int tsId, const double s, const int nsub)
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

void DynamicSystem::setDofResponse()
{
	for (int i = 0; i < eqnCount; i++)
	{
		dof *d = dofs.at(eqnMapDof.at(i));
		d->setResponse(dsp(i), vel(i), acc(i));
	}
}

void DynamicSystem::getElementResponse()
{
	if (!(eles.empty()))
	{
		for (auto it = eles.begin(); it != eles.end(); it++)
		{
			element *e = it->second;
			e->getResponse();
		}
	}
}

void DynamicSystem::assembleNonlinearForceVector(const bool update)
{
	q = zeros<vec>(eqnCount);

	if (!(nonlinearElements.empty()))
	{
		for (auto it = nonlinearElements.begin(); it != nonlinearElements.end(); it++)
		{
			auto *s = it->second;
			s->getResponse(update);
			s->assembleNonlinearForceVector(q);
		}
	}

	if (!(nonlinearTangentElements.empty()))
	{
		for (auto it = nonlinearTangentElements.begin(); it != nonlinearTangentElements.end(); it++)
		{
			auto *s = it->second;
			s->getResponse(update);
			s->assembleNonlinearForceVector(q);
		}
	}

	if (!(nonlinearSecantElements.empty()))
	{
		for (auto it = nonlinearSecantElements.begin(); it != nonlinearSecantElements.end(); it++)
		{
			auto *s = it->second;
			s->getResponse(update);
			s->assembleNonlinearForceVector(q);
		}
	}

	if (!(nonlinearInitialTangentElements.empty()))
	{
		for (auto it = nonlinearInitialTangentElements.begin(); it != nonlinearInitialTangentElements.end(); it++)
		{
			auto *s = it->second;
			s->getResponse(update);
			s->assembleNonlinearForceVector(q);
		}
	}
}

void DynamicSystem::initRecorders()
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

void DynamicSystem::recordResponse()
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

void DynamicSystem::saveResponse()
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

void DynamicSystem::printInfo()
{
	cout << "=================================================" << endl;
	cout << "            RRRR    OOO   DDD    SSS             " << endl;
	cout << "            R   R  O   O  D  D  S                " << endl;
	cout << "            RRRR   O   O  D  D   SSS             " << endl;
	cout << "            R R    O   O  D  D      S            " << endl;
	cout << "            R  RR   OOO   DDD   SSSS             " << endl;
	cout << "            R  RR   OOO   DDD   SSSS             " << endl;
	cout << "=================================================" << endl;

	cout << "Number of DOFs:" << dofs.size() << endl;
	cout << "Number of Nodes:" << nodes.size() << endl;
	cout << "Number of Elements:" << eles.size() << endl;
	cout << "Number of Equations:" << eqnCount << endl;

	int nModes = (int)P.n_rows;
	if (nModes > 0)
	{
		cout << "Number of Modes:" << nModes << endl;
		for (int i = 0; i < nModes; i++)
		{
			cout << "Mode " << i + 1 << ", T = " << P(i) << endl;
		}
	}
}