#include "DynamicSystem.h"
#include "numeric.h"

#include <fstream>
#include <vector>

#include "material/elastic.h"
#include "material/Elastoplastic.h"
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

void DynamicSystem::addNode(Node * nd)
{
	auto it = Nodes.insert(std::make_pair(nd->id, nd));
	if (!it.second)
	{
		cout << "Node ID: " << nd->id << " already exists! The node will not be added." << endl;
	}
}

void DynamicSystem::addNode(const int id, const double x, const int dofId)
{
	DOF *d = DOFs.at(dofId);
	Node *nd = new Node(id, x);
	nd->setDof(d);
	addNode(nd);
}

void DynamicSystem::addNode(const int id, const double x, const double y, const double z)
{
	Node *nd = new Node(id, x, y, z);
	addNode(nd);
}

void DynamicSystem::addNode(const int id, const double x, const double z, const int dofXId, const int dofZId, const int dofRYId)
{
	DOF *dx = DOFs.at(dofXId);
	DOF *dz = DOFs.at(dofZId);

	Node *nd = new Node(id, x, 0.0, z);
	nd->setDof(dx);
	nd->setDof(dz);

	if (dofRYId >= 0)
	{
		nd->setDof(DOFs.at(dofRYId));
	}

	addNode(nd);
}

void DynamicSystem::addNodeWithDof(const int id, const double x, const int dofId)
{
	DOF *d = new DOF(dofId, Direction::X);
	Node *nd = new Node(id, x, 0.0, 0.0);
	nd->setDof(d);

	addDof(d);
	addNode(nd);
}

void DynamicSystem::addLine(Line *l)
{
	auto it = Lines.insert(std::make_pair(l->id, l));
	if (!it.second)
	{
		cout << "Line ID: " << l->id << " already exists! The line will not be added." << endl;
	}
}

void DynamicSystem::addLine(const int id, const int ni, const int nj)
{
	Line *l = new Line(id, Nodes.at(ni), Nodes.at(nj));
	addLine(l);
}

void DynamicSystem::fixDof(const int id)
{
	DOFs.at(id)->isFixed = true;
}

void DynamicSystem::fixNode(const int id)
{
	Nodes.at(id)->fixDof();
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
	DOFs.at(dofId)->addLoad(Loads.at(loadId));
}

void DynamicSystem::exportGmsh(char * fileName)
{
	std::ofstream outFile;
	outFile.open(fileName, ios::out);

	outFile << "$MeshFormat\n2.2 0 8\n$EndMeshFormat" << std::endl;
	outFile << "$Nodes" << std::endl;
	outFile << Nodes.size() << std::endl;
	for (auto it = Nodes.begin(); it != Nodes.end(); ++it)
	{
		auto nd = it->second;
		outFile << it->first << " " << nd->x0 << " " << nd->y0 << " " << nd->z0 << std::endl;
	}
	outFile << "$EndNodes" << std::endl;

	if (Element2Ds.size() > 1)
	{
		outFile << "$Elements" << std::endl;
		outFile << Element2Ds.size() << std::endl;
		for (auto it = Element2Ds.begin(); it != Element2Ds.end(); ++it)
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
		DOF *d = DOFs.at(eqnMapDof.at(i));
		d->dsp = modeshape(i);
	}

	outFile << "1" << std::endl;
	outFile << "\"First Mode: T = " << P(0) << "s\"" << std::endl;
	outFile << "1" << std::endl;
	outFile << "0.0" << std::endl;
	outFile << "3" << std::endl;
	outFile << "0" << std::endl;
	outFile << "3" << std::endl;
	outFile << Nodes.size() << std::endl;

	for (auto it = Nodes.begin(); it != Nodes.end(); it++)
	{
		auto nd = it->second;
		outFile << it->first << " " << nd->dofX->dsp << " " << 0.0 << " " << nd->dofZ->dsp << std::endl;
	}

	outFile << "$EndNodeData" << std::endl;

	outFile.close();

	DOF *d = nullptr;
	for (int i = 0; i < eqnCount; i++)
	{
		d = DOFs.at(eqnMapDof.at(i));
		d->dsp = 0.0;
	}
}

void DynamicSystem::addDof(DOF * d)
{
	if (DOFs.count(d->id) == 0)
	{
		DOFs[d->id] = d;
	}
	else
	{
		cout << "dof ID: " << d->id << " already exists! The dof will not be added." << endl;
	}
}

void DynamicSystem::addDof(const int id, const double m, const bool fixed)
{
	DOF *d = new DOF(id, Direction::X, m, fixed);
	addDof(d);
}

void DynamicSystem::addDof(const int id, Direction dir, const double m, const bool fixed)
{
	DOF *d = new DOF(id, dir, m, fixed);
	addDof(d);
}

void DynamicSystem::setMass(const int id, const double m)
{
	DOFs.at(id)->mass = m;
}

void DynamicSystem::setNodeMass(const int id, const double m)
{
	Node *nd = Nodes.at(id);
	if (nd->isActivated(Direction::X))
	{
		nd->dofX->mass = m;
	}
	if (nd->isActivated(Direction::Z))
	{
		nd->dofZ->mass = m;
	}
	if (nd->isActivated(Direction::RY))
	{
		nd->dofRY->mass = m;
	}
}

void DynamicSystem::mapDofNode(DOF * d, Node * nd)
{
	nd->setDof(d);
	dofMapNode[d->id] = nd->id;
}

void DynamicSystem::mapDofNode(const int id_d, const int id_nd)
{
	Node *nd = Nodes.at(id_nd);
	DOF *d = DOFs.at(id_d);
	nd->setDof(d);
	dofMapNode[id_d] = id_nd;
}

bool DynamicSystem::addMaterial1D(Material1D * mt)
{
	if (Material1Ds.count(mt->id) == 0)
	{
		Material1Ds[mt->id] = mt;
		return true;
	}
	else
	{
		cout << "Material1D ID: " << mt->id << " already exists! The material will not be added." << endl;
		return false;
	}
}

bool DynamicSystem::addMaterialElastic(const int id, const double E0)
{
	Material1D *mtrl = new elastic(id, E0);
	return addMaterial1D(mtrl);
}

bool DynamicSystem::addMaterialElastoplastic(const int id, const double E0, const double fy, const double alpha)
{
	Material1D *mtrl = new Elastoplastic(id, E0, fy, alpha);
	return addMaterial1D(mtrl);
}

bool DynamicSystem::addMaterialSteelBilinear(const int id, const double E0, const double fy, const double alpha, const double beta)
{
	Material1D *mtrl = new steelBilinear(id, E0, fy, alpha, beta);
	return addMaterial1D(mtrl);
}

bool DynamicSystem::addMaterialConcreteTrilinear(const int id, const double E0, const double fc, const double epsilon_c, const double sigma_cr, const double sigma_u, const double epsilon_u)
{
	Material1D *mtrl = new concreteTrilinear(id, E0, fc, epsilon_c, sigma_cr, sigma_u, epsilon_u);
	return addMaterial1D(mtrl);
}

bool DynamicSystem::addMaterialSMABilinear(const int id, const double E0, const double fy, const double alpha, const double sigma_shift)
{
	Material1D *mtrl = new SMABilinear(id, E0, fy, alpha, sigma_shift);
	return addMaterial1D(mtrl);
}

bool DynamicSystem::addFiber(const int id, const int matId, const double A, const double y, const double z)
{
	if (Fibers.count(id) == 0)
	{
		Fiber *fiber = new Fiber(id, Material1Ds.at(matId), A, y, z);
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

bool DynamicSystem::addElement(Element * e)
{
	if (Elements.count(e->id) == 0)
	{
		Elements[e->id] = e;
		return true;
	}
	else
	{
		cout << "Element ID: " << e->id << " already exists! The element will not be added." << endl;
		return false;
	}
}

void DynamicSystem::addSpring(Spring *s)
{
	if (addElement(s)) {
		Springs[s->id] = s;
		linearElasticElements[s->id] = s;
	}
}

void DynamicSystem::addSpring(const int id, const int ni, const int nj, const double k)
{
	DOF *i = DOFs.at(ni);
	DOF *j = DOFs.at(nj);
	Spring *s = new Spring(id, i, j, k);
	addSpring(s);
}

void DynamicSystem::addSpringBilinear(SpringBilinear * s)
{
	if (addElement(s))
	{
		SpringBilinears[s->id] = s;
		nonlinearTangentElements[s->id] = s;
	}
}

void DynamicSystem::addSpringBilinear(const int id, const int ni, const int nj, const double k0, const double uy, const double alpha)
{
	DOF *i = DOFs.at(ni);
	DOF *j = DOFs.at(nj);
	SpringBilinear *s = new SpringBilinear(id, i, j, k0, uy, alpha);
	addSpringBilinear(s);
}

void DynamicSystem::addSpringNonlinear(SpringNonlinear * s)
{
	if (addElement(s))
	{
		SpringNonlinears[s->id] = s;
		nonlinearTangentElements[s->id] = s;
	}
}

void DynamicSystem::addSpringNonlinear(const int id, const int ni, const int nj, const int matId)
{
	DOF *i = DOFs.at(ni);
	DOF *j = DOFs.at(nj);
	SpringNonlinear *s = new SpringNonlinear(id, i, j, Material1Ds.at(matId));
	addSpringNonlinear(s);
}

void DynamicSystem::addSpringBoucWen(SpringBoucWen * s)
{
	if (addElement(s))
	{
		SpringBoucWens[s->id] = s;
		nonlinearTangentElements[s->id] = s;
	}
}

void DynamicSystem::addSpringBoucWen(const int id, const int ni, const int nj, const double k0, const double uy, const double alpha, const double beta, const double n)
{
	DOF *i = DOFs.at(ni);
	DOF *j = DOFs.at(nj);
	SpringBoucWen *s = new SpringBoucWen(id, i, j, k0, uy, alpha);
	addSpringBoucWen(s);
}

void DynamicSystem::addDashpot(Dashpot * d)
{
	if (addElement(d))
	{
		Dashpots[d->id] = d;
		linearDampingElements[d->id] = d;
	}
}

void DynamicSystem::addDashpot(const int id, const int ni, const int nj, const double c)
{
	DOF *i = DOFs.at(ni);
	DOF *j = DOFs.at(nj);
	Dashpot *d = new Dashpot(id, i, j, c);
	addDashpot(d);
}

void DynamicSystem::addDashpotExp(DashpotExp * d)
{
	if (addElement(d))
	{
		DashpotExps[d->id] = d;
		nonlinearElements[d->id] = d;
	}
}

void DynamicSystem::addDashpotExp(const int id, const int ni, const int nj, const double c, const double alpha)
{
	DOF *i = DOFs.at(ni);
	DOF *j = DOFs.at(nj);
	DashpotExp *d = new DashpotExp(id, i, j, c, alpha);
	addDashpotExp(d);
}

void DynamicSystem::addDashpotMaxwell(DashpotMaxwell * d)
{
	if (addElement(d))
	{
		DashpotMaxwells[d->id] = d;
		nonlinearElements[d->id] = d;
	}
}

void DynamicSystem::addDashpotMaxwell(const int id, const int ni, const int nj, const double k, const double c, const double alpha)
{
	DOF *i = DOFs.at(ni);
	DOF *j = DOFs.at(nj);
	DashpotMaxwell *d = new DashpotMaxwell(id, i, j, c, alpha);
	addDashpotMaxwell(d);
}

void DynamicSystem::addInerter(Inerter * in)
{
	if (addElement(in))
	{
		Inerters[in->id] = in;
		inertialMassElements[in->id] = in;
	}
}

void DynamicSystem::addInerter(const int id, const int ni, const int nj, const double m)
{
	DOF *i = DOFs.at(ni);
	DOF *j = DOFs.at(nj);
	Inerter *in = new Inerter(id, i, j, m);
	addInerter(in);
}

void DynamicSystem::addSlider(Slider * s)
{
	if (addElement(s))
	{
		Sliders[s->id] = s;
		nonlinearElements[s->id] = s;
	}
}

void DynamicSystem::addSlider(const int id, const int ni, const int nj, const double muN)
{
	DOF *i = DOFs.at(ni);
	DOF *j = DOFs.at(nj);
	Slider *s = new Slider(id, i, j, muN);
	addSlider(s);
}

void DynamicSystem::addSPIS2(SPIS2 * s)
{
	if (addElement(s))
	{
		SPIS2s[s->id] = s;
		linearElasticElements[s->id] = s;
		linearDampingElements[s->id] = s;
		inertialMassElements[s->id] = s;
	}
}

void DynamicSystem::addSPIS2(const int id, const int ni, const int nj, const int nin, const double m, const double c, const double k)
{
	DOF *i = DOFs.at(ni);
	DOF *j = DOFs.at(nj);
	DOF *in = DOFs.at(nin);
	SPIS2 *s = new SPIS2(id, i, j, in, m, c, k);
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
	DOF *i = DOFs.at(ni);
	DOF *j = DOFs.at(nj);
	TVMD *d = new TVMD(id, i, j, m, c, k);
	addTVMD(d);
}

void DynamicSystem::addSpring2D(Spring2D *s)
{
	if (addElement(s))
	{
		Element2Ds[s->id] = s;
		Spring2Ds[s->id] = s;
		linearElasticElements[s->id] = s;
	}
}

void DynamicSystem::addSpring2D(const int id, const int ni, const int nj, const double k, ELE::localAxis U)
{
	Spring2D *s = new Spring2D(id, Nodes.at(ni), Nodes.at(nj), k, U);
	addSpring2D(s);
}

void DynamicSystem::addSpringBoucWen2D(SpringBoucWen2D *s)
{
	if (addElement(s))
	{
		Element2Ds[s->id] = s;
		SpringBoucWen2Ds[s->id] = s;
		nonlinearTangentElements[s->id] = s;
	}
}

void DynamicSystem::addSpringBoucWen2D(const int id, const int ni, const int nj, const double k0, const double uy, const double alpha, const double beta, const double n, ELE::localAxis U /*= ELE::U1*/)
{
	SpringBoucWen2D *s = new SpringBoucWen2D(id, Nodes.at(ni), Nodes.at(nj), k0, uy, alpha, beta, n, U);
	addSpringBoucWen2D(s);
}

void DynamicSystem::addDashpot2D(Dashpot2D *s)
{
	if (addElement(s))
	{
		Element2Ds[s->id] = s;
		Dashpot2Ds[s->id] = s;
		linearDampingElements[s->id] = s;
	}
}

void DynamicSystem::addDashpot2D(const int id, const int ni, const int nj, const double c, ELE::localAxis U)
{
	Dashpot2D *s = new Dashpot2D(id, Nodes.at(ni), Nodes.at(nj), c, U);
	addDashpot2D(s);
}

void DynamicSystem::addInerter2D(Inerter2D *s)
{
	if (addElement(s))
	{
		Element2Ds[s->id] = s;
		Inerter2Ds[s->id] = s;
		inertialMassElements[s->id] = s;
	}
}

void DynamicSystem::addInerter2D(const int id, const int ni, const int nj, const double m, ELE::localAxis U)
{
	Inerter2D *s = new Inerter2D(id, Nodes.at(ni), Nodes.at(nj), m, U);
	addInerter2D(s);
}

void DynamicSystem::addDashpotExp2D(DashpotExp2D *s)
{
	if (addElement(s))
	{
		Element2Ds[s->id] = s;
		DashpotExp2Ds[s->id] = s;
		nonlinearElements[s->id] = s;
	}
}

void DynamicSystem::addDashpotExp2D(const int id, const int ni, const int nj, const double c, const double alpha, ELE::localAxis U /*= ELE::U1*/)
{
	DashpotExp2D *s = new DashpotExp2D(id, Nodes.at(ni), Nodes.at(nj), c, alpha, U);
	addDashpotExp2D(s);
}

void DynamicSystem::addDashpotMaxwell2D(DashpotMaxwell2D *s)
{
	if (addElement(s))
	{
		Element2Ds[s->id] = s;
		DashpotMaxwell2Ds[s->id] = s;
		nonlinearElements[s->id] = s;
	}
}

void DynamicSystem::addDashpotMaxwell2D(const int id, const int ni, const int nj, const double k, const double c, const double alpha, ELE::localAxis U)
{
	DashpotMaxwell2D *s = new DashpotMaxwell2D(id, Nodes.at(ni), Nodes.at(nj), k, c, alpha, U);
	addDashpotMaxwell2D(s);
}

void DynamicSystem::addTrussElastic(TrussElastic *truss)
{
	if (addElement(truss)) {
		Element2Ds[truss->id] = truss;
		TrussElastics[truss->id] = truss;
		linearElasticElements[truss->id] = truss;
	}
}

void DynamicSystem::addTrussElastic(const int id, const int ni, const int nj, const double EA)
{
	TrussElastic *truss = new TrussElastic(id, Nodes.at(ni), Nodes.at(nj), EA);
	addTrussElastic(truss);
}

void DynamicSystem::addBeamElastic(BeamElastic *beam)
{
	if (addElement(beam)) {
		Element2Ds[beam->id] = beam;
		BeamElastics[beam->id] = beam;
		linearElasticElements[beam->id] = beam;
	}
}

void DynamicSystem::addBeamElastic(const int id, const int ni, const int nj, const double EI)
{
	BeamElastic *beam = new BeamElastic(id, Nodes.at(ni), Nodes.at(nj), EI);
	addBeamElastic(beam);
}

void DynamicSystem::addFrameElastic(FrameElastic2D *frame)
{
	if (addElement(frame)) {
		Element2Ds[frame->id] = frame;
		FrameElastic2Ds[frame->id] = frame;
		linearElasticElements[frame->id] = frame;
	}
}

void DynamicSystem::addFrameElastic(const int id, const int ni, const int nj, const double EA, const double EI)
{
	FrameElastic2D *frame = new FrameElastic2D(id, Nodes.at(ni), Nodes.at(nj), EA, EI);
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
	Quad4Elastic *quad = new Quad4Elastic(id, Nodes.at(nodeI), Nodes.at(nodeJ), Nodes.at(nodeP), Nodes.at(nodeQ), E, nu, t);
	addQuad4Elastic(quad);
}

void DynamicSystem::addTruss2D(const int id, const int ni, const int nj, const int secId)
{
	if (Elements.count(id) == 0)
	{
		Truss2D *truss = new Truss2D(id, Nodes.at(ni), Nodes.at(nj), SectionTrusss.at(secId));
		Elements[id] = truss;
		Element2Ds[id] = truss;
		Truss2Ds[id] = truss;
		nonlinearInitialTangentElements[id] = truss;
	}
	else
	{
		cout << "Element ID: " << id << " already exists! The element will not be added." << endl;
	}
}

void DynamicSystem::addFrame2D(const int id, const int ni, const int nj, const int secId, const int nIntP)
{
	if (Elements.count(id) == 0)
	{
		Frame2D *frame = new Frame2D(id, Nodes.at(ni), Nodes.at(nj), SectionFrame2Ds.at(secId), nIntP);
		Elements[id] = frame;
		Element2Ds[id] = frame;
		Frame2Ds[id] = frame;
		nonlinearInitialTangentElements[id] = frame;
	}
	else
	{
		cout << "Element ID: " << id << " already exists! The element will not be added." << endl;
	}
}

void DynamicSystem::addFramePDelta2D(const int id, const int ni, const int nj, const int secId, const int nIntP)
{
	if (Elements.count(id) == 0)
	{
		FramePDelta2D *frame = new FramePDelta2D(id, Nodes.at(ni), Nodes.at(nj), SectionFrame2Ds.at(secId), nIntP);
		Elements[id] = frame;
		Element2Ds[id] = frame;
		FramePDelta2Ds[id] = frame;
		nonlinearInitialTangentElements[id] = frame;
	}
	else
	{
		cout << "Element ID: " << id << " already exists! The element will not be added." << endl;
	}
}

void DynamicSystem::addTimeSeries(TimeSeries * ts)
{
	Waves[ts->id] = ts;
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
	DOFRecorders[dr->id] = dr;
}

void DynamicSystem::addDofRecorder(const int id, int *dofIds, const int n, response rtype, char * fileName)
{
	std::vector<DOF *> rdofs(n);

	for (int i = 0; i < n; i++)
	{
		rdofs[i] = DOFs.at(dofIds[i]);
	}

	dofRecorder *dr = new dofRecorder(id, rdofs, rtype, fileName);
	addDofRecorder(dr);
}

void DynamicSystem::addElementRecorder(elementRecorder * er)
{
	ElementRecorders[er->id] = er;
}

void DynamicSystem::addElementRecorder(const int id, int * eleIds, const int n, response rtype, char * fileName)
{
	std::vector<Element *> reles(n);

	for (int i = 0; i < n; i++)
	{
		reles[i] = Elements.at(eleIds[i]);
	}

	elementRecorder *er = new elementRecorder(id, reles, rtype, fileName);
	addElementRecorder(er);
}

void DynamicSystem::addSpringRecorder(const int id, int * eleIds, const int n, response rtype, char * fileName)
{
	std::vector<Element *> reles(n);

	for (int i = 0; i < n; i++)
	{
		reles[i] = Springs.at(eleIds[i]);
	}

	elementRecorder *er = new elementRecorder(id, reles, rtype, fileName);
	addElementRecorder(er);
}

void DynamicSystem::addDashpotRecorder(const int id, int * eleIds, const int n, response rtype, char * fileName)
{
	std::vector<Element *> reles(n);

	for (int i = 0; i < n; i++)
	{
		reles[i] = Dashpots.at(eleIds[i]);
	}

	elementRecorder *er = new elementRecorder(id, reles, rtype, fileName);
	addElementRecorder(er);
}

void DynamicSystem::addInerterRecorder(const int id, int * eleIds, const int n, response rtype, char * fileName)
{
	std::vector<Element *> reles(n);

	for (int i = 0; i < n; i++)
	{
		reles[i] = Inerters.at(eleIds[i]);
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

void DynamicSystem::activeGroundMotion(Direction dir)
{
	for (int i = 0; i < eqnCount; i++)
	{
		DOF *d = DOFs.at(eqnMapDof.at(i));
		if (d->dir == dir)
		{
			E(i) = 1.0;
		}
	}
}

void DynamicSystem::buildDofEqnMap()
{
	std::map<int, DOF *>::iterator it;
	eqnCount = 0;
	fixedDofCount = 0;
	dofMapEqn.clear();
	eqnMapDof.clear();

	for (it = DOFs.begin(); it != DOFs.end(); it++)
	{
		DOF *d = it->second;
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
		m(i) = DOFs.at(eqnMapDof[i])->mass;
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

	for (auto it = DOFs.begin(); it != DOFs.end(); ++it)
	{
		DOF *d = it->second;
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
		DOF *d = DOFs.at(eqnMapDof.at(i));
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
		Q0 = Q;
	}
	else
	{
		dsp0 = zeros<vec>(eqnCount);
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
	auto nDof = DOFs.size();
	int eqnId = 0;
	for (auto it = DOFs.begin(); it != DOFs.end(); ++it)
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
		DOFs.at(it->first)->eqnId = it->second;
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

	DOF *d = nullptr;
	for (int i = 0; i < eqnCount; i++)
	{
		d = DOFs.at(eqnMapDof.at(i));
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
	Load *load = Loads.at(dispControlLoadId); // time-varying load pattern of the control DOF

	// obtain the reference load vector:
	ctime = loadedTime;
	vec p(size(Q)); // reference load vector
	DOF *d = nullptr;
	for (int i = 0; i < eqnCount; i++)
	{
		d = DOFs.at(eqnMapDof.at(i));
		p(i) = d->getLoad(ctime, false);
	}

	double tol = 1e-6; // tolerance for convergence check
	int maxIter = 20; // maximum iteration times
	int nIter = 0; // iteration count
	double error = 1.0; // norm error for convergence check
	double cdisp = 0.0; // current desired value of control DOF
	double lambda = 0.0; // load factor
	double dlambda = 0.0; // variation of load factor

	nsteps = nsub; // number of analysis steps
	dt = loadedTime/nsub; // analysis time interval
	initRecorders(); // initialize the recorders with given number of analysis steps
	
	vec du = zeros<vec>(eqnCount); // variation of displacement response
	vec du0 = zeros<vec>(eqnCount); // reference variation of displacement response due to reference load and current Jacobian matrix
	vec du_ = zeros<vec>(eqnCount); // initial variation of displacement and datum vector to calculate the relative norm error
	double normDu; // datum value to calculate the relative norm error
	
	for (auto i=1; i<nsub+1; ++i)
	{
		cstep = i-1;
		ctime = i*dt;
		nIter = 0;
		
		du = solve(K, p);
		cdisp = load->getValue(ctime);
		dlambda = (cdisp - load->getValue(ctime-dt))/du(dispControlEqn);
		du_ = du*dlambda; // initial trial displacement
		normDu = norm(du_);
		lambda += dlambda; // 
		dsp = dsp + du_; // initial trial displacement
		dsp0 = dsp; // set the corrected response to the DOFs
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
	nsteps = Waves.at(tsId)->nsteps;
	dt = Waves.at(tsId)->dt;
	vec ag = s*Waves.at(tsId)->series;

	vec u0(dsp);
	vec v0(vel);
	vec a0(acc);

	cstep = 0;
	ctime = 0.0;
	initRecorders();

	dt = dt/nsub;
	Element::dt = dt;

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

	a0 = solve(M, -Mp*E*ag(0) - C*v0);

	dsp = u0;
	vel = v0;
	acc = a0;

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

		dsp = u0;
		vel = v0;
		acc = a0;

		setDofResponse();
		getElementResponse();
		recordResponse();
	}
	saveResponse();
}

void DynamicSystem::solveTimeDomainSeismicResponseNMKNL(const int tsId, const double s, const int nsub, const double tol, const int maxiter)
{
	nsteps = Waves.at(tsId)->nsteps;
	dt = Waves.at(tsId)->dt;
	vec ag = s*Waves.at(tsId)->series;

	vec u0(dsp);
	vec v0(vel);
	vec a0(acc);

	cstep = 0;
	ctime = 0.0;
	initRecorders();

	dt = dt/nsub;
	Element::dt = dt;

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

	a0 = solve(M, -Mp*E*ag(0) - C*v0);

	dsp = u0;
	vel = v0;
	acc = a0;

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
				error = norm(du)/norm(u0);
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

		dsp = u0;
		vel = v0;
		acc = a0;
	}
	saveResponse();
}

void DynamicSystem::solveTimeDomainSeismicResponseStateSpace(const int tsId, const double s, const int nsub)
{
	nsteps = Waves.at(tsId)->nsteps;
	dt = Waves.at(tsId)->dt;
	vec ag = s*Waves.at(tsId)->series;

	vec x0 = zeros<vec>(2*eqnCount);
	x0.head_rows(eqnCount) = dsp;
	x0.tail_rows(eqnCount) = vel;
	vec F = zeros<vec>(2*eqnCount);

	cstep = 0;
	ctime = 0.0;
	initRecorders();

	dt = dt/nsub;
	Element::dt = dt;

	mat O = zeros<mat>(eqnCount, eqnCount);
	mat I = eye<mat>(eqnCount, eqnCount);
	mat A = -solve(M, K0);
	mat B = -solve(M, C);
	mat H = join_cols(join_rows(O, I), join_rows(A, B));

	double h = dt;
	//mat T = expmat(H*h);
	mat T = expm(H, h);

	dsp = x0.head_rows(eqnCount);
	vel = x0.tail_rows(eqnCount);
	acc = solve(M, -Mp*E*ag(0) - C*vel);

	setDofResponse();
	getElementResponse();
	recordResponse();

	double agd, agi, agj;
	for (int i = 0; i < nsteps - 1; i++)
	{
		cstep += 1;
		ctime += dt*nsub;
		agd = (ag(i + 1) - ag(i)) / nsub;
		agi = ag(i);
		for (int j = 0; j < nsub; j++)
		{
			agj = agi + agd*j;
			F.tail_rows(eqnCount) = solve(M, -Mp*E*agj + Q0);
			x0 = T*(x0 + F*h);
		}

		dsp = x0.head_rows(eqnCount);
		vel = x0.tail_rows(eqnCount);
		acc = solve(M, -Mp*E*agj - C*vel - K*dsp + Q0);

		setDofResponse();
		getElementResponse();
		recordResponse();
	}
	saveResponse();
}

void DynamicSystem::solveTimeDomainSeismicResponseStateSpaceNL(const int tsId, const double s, const int nsub)
{
	nsteps = Waves.at(tsId)->nsteps;
	dt = Waves.at(tsId)->dt;
	vec ag = s*Waves.at(tsId)->series;

	vec x0 = zeros<vec>(2*eqnCount);
	x0.head_rows(eqnCount) = dsp;
	x0.tail_rows(eqnCount) = vel;
	vec F = zeros<vec>(2*eqnCount);

	cstep = 0;
	ctime = 0.0;
	initRecorders();

	dt = dt/nsub;
	Element::dt = dt;

	mat O = zeros<mat>(eqnCount, eqnCount);
	mat I = eye<mat>(eqnCount, eqnCount);
	mat A = -solve(M, K0);
	mat B = -solve(M, C);
	mat H = join_cols(join_rows(O, I), join_rows(A, B));

	double h = dt;
	//mat T = expmat(H*h);
	mat T = expm(H, h);

	dsp = x0.head_rows(eqnCount);
	vel = x0.tail_rows(eqnCount);
	acc = solve(M, -Mp*E*ag(0) - C*vel);

	setDofResponse();
	getElementResponse();
	assembleNonlinearForceVector(true);
	recordResponse();

	double agd, agi, agj;
	for (int i = 0; i < nsteps - 1; i++)
	{
		cstep += 1;
		ctime += dt*nsub;
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
		acc = solve(M, -Mp*E*agj - q - C*vel - K0*dsp + Q0);
	}
	saveResponse();
}

void DynamicSystem::solveTimeDomainSeismicResponseRK4(const int tsId, const double s, const int nsub)
{
	int nstep = Waves.at(tsId)->nsteps;
	double dt = Waves.at(tsId)->dt;
	vec ag = s*Waves.at(tsId)->series;

	dt = dt / nsub;
	Element::dt = dt;
}

void DynamicSystem::setDofResponse()
{
	for (int i = 0; i < eqnCount; i++)
	{
		DOF *d = DOFs.at(eqnMapDof.at(i));
		d->setResponse(dsp(i), vel(i), acc(i));
	}
}

void DynamicSystem::getElementResponse()
{
	if (!(Elements.empty()))
	{
		for (auto it = Elements.begin(); it != Elements.end(); it++)
		{
			Element *e = it->second;
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
	if (!(DOFRecorders.empty()))
	{
		std::map<int, Recorder *>::iterator it;
		for (it = DOFRecorders.begin(); it != DOFRecorders.end(); it++)
		{
			Recorder *dr = it->second;
			dr->init(nsteps);
		}
	}

	if (!(ElementRecorders.empty()))
	{
		std::map<int, Recorder *>::iterator it;
		for (it = ElementRecorders.begin(); it != ElementRecorders.end(); it++)
		{
			Recorder *er = it->second;
			er->init(nsteps);
		}
	}
}

void DynamicSystem::recordResponse()
{
	if (!(DOFRecorders.empty()))
	{
		std::map<int, Recorder *>::iterator it;
		for (it = DOFRecorders.begin(); it != DOFRecorders.end(); it++)
		{
			Recorder *dr = it->second;
			dr->record(cstep, ctime);
		}
	}

	if (!(ElementRecorders.empty()))
	{
		std::map<int, Recorder *>::iterator it;
		for (it = ElementRecorders.begin(); it != ElementRecorders.end(); it++)
		{
			Recorder *er = it->second;
			er->record(cstep, ctime);
		}
	}
}

void DynamicSystem::saveResponse()
{
	if (!(DOFRecorders.empty()))
	{
		std::map<int, Recorder *>::iterator it;
		for (it = DOFRecorders.begin(); it != DOFRecorders.end(); it++)
		{
			Recorder *dr = it->second;
			dr->save();
		}
	}

	if (!(ElementRecorders.empty()))
	{
		std::map<int, Recorder *>::iterator it;
		for (it = ElementRecorders.begin(); it != ElementRecorders.end(); it++)
		{
			Recorder *er = it->second;
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

	cout << "Number of DOFs:" << DOFs.size() << endl;
	cout << "Number of Nodes:" << Nodes.size() << endl;
	cout << "Number of Elements:" << Elements.size() << endl;
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