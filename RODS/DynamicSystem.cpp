#include "DynamicSystem.h"
#include "numeric.h"

#include <fstream>
#include <vector>

#include "material/Elastic.h"
#include "material/Elastoplastic.h"
#include "material/SteelBilinear.h"
#include "material/ConcreteTrilinear.h"
#include "material/SMABilinear.h"
#include "material/CyclicHardenTrilinear.h"


DynamicSystem::DynamicSystem(const double z) :
	zeta(z), eqnCount(0), fixedDofCount(0), eigenVectorNormed(false),
	dynamicSolver(RODS::DynamicSolver::StateSpace), dt(0.02), ctime(0.0), nsteps(0), cstep(0),
	useRayleighDamping(true), RayleighOmg1(2 * PI / 0.3), RayleighOmg2(2 * PI / 0.1),
	NumModesInherentDamping(-1),
	XSeismicWaveId(-1), YSeismicWaveId(-1), ZSeismicWaveId(-1),
	XSeismicWaveScale(0.0), YSeismicWaveScale(0.0), ZSeismicWaveScale(0.0),
	dispControlDOFId(-1), dispControlLoadId(-1), dispControlEqn(-1),
	NumDynamicSubSteps(1), tol(1e-6), maxIter(20),
	exportGmshInterval(-1),
	name("RODS"), workDir("./"),
	xMax(0.0), xMin(0.0), yMax(0.0), yMin(0.0), zMax(0.0), zMin(0.0)
{
}

DynamicSystem::~DynamicSystem()
{
}

void DynamicSystem::addPoint(Point *p)
{
	auto it = Points.insert(std::make_pair(p->id, p));
	if (!it.second)
	{
		cout << "Point ID: " << p->id << " already exists! The point will not be added." << endl;
	}
	else
	{
		if (p->x > xMax) xMax = p->x;
		else if (p->x < xMin) xMin = p->x;

		if (p->y > yMax) yMax = p->y;
		else if (p->y < yMin) yMin = p->y;

		if (p->z > zMax) zMax = p->z;
		else if (p->z < zMin) zMin = p->z;
	}
}

void DynamicSystem::addPoint(const int pointId, const double x, const double y, const double z)
{
	Point *p = new Point(pointId, x, y, z);
	addPoint(p);
}

void DynamicSystem::addNode(Node * nd)
{
	auto it = Nodes.insert(std::make_pair(nd->id, nd));
	if (!it.second)
	{
		cout << "Node ID: " << nd->id << " already exists! The node will not be added." << endl;
	}
	else
	{
		if (nd->x > xMax) xMax = nd->x;
		else if (nd->x < xMin) xMin = nd->x;

		if (nd->y > yMax) yMax = nd->y;
		else if (nd->y < yMin) yMin = nd->y;

		if (nd->z > zMax) zMax = nd->z;
		else if (nd->z < zMin) zMin = nd->z;
	}
}

void DynamicSystem::addNode(const int nodeId, const double x, const int dofId)
{
	DOF *d = DOFs.at(dofId);
	Node *nd = new Node(nodeId, x);
	nd->setDof(d);
	d->setNodeId(nodeId);
	addNode(nd);
}

void DynamicSystem::addNode(const int nodeId, const double x, const double z, const int dofXId, const int dofZId, const int dofRYId)
{
	DOF *dx = DOFs.at(dofXId);
	DOF *dz = DOFs.at(dofZId);

	dx->setNodeId(nodeId);
	dz->setNodeId(nodeId);

	Node *nd = new Node(nodeId, x, 0.0, z);
	nd->setDof(dx);
	nd->setDof(dz);

	if (dofRYId >= 0)
	{
		nd->setDof(DOFs.at(dofRYId));
	}

	addNode(nd);
}

void DynamicSystem::addNode(const int nodeId, const double x, const double y, const double z,
				const int dofXId,  const int dofYId,  const int dofZId,
				const int dofRXId, const int dofRYId, const int dofRZId)
{
	Node *nd = new Node(nodeId, x, y, z);

	nd->setDof(DOFs.at(dofXId));
	nd->setDof(DOFs.at(dofYId));
	nd->setDof(DOFs.at(dofZId));

	DOFs.at(dofXId)->setNodeId(nodeId);
	DOFs.at(dofYId)->setNodeId(nodeId);
	DOFs.at(dofZId)->setNodeId(nodeId);
	
	if (dofRXId >= 0) {
		auto d = DOFs.at(dofRXId);
		nd->setDof(d);
		d->setNodeId(nodeId);
	}
	if (dofRYId >= 0) {
		auto d = DOFs.at(dofRYId);
		nd->setDof(d);
		d->setNodeId(nodeId);
	}
	if (dofRZId >= 0) {
		auto d = DOFs.at(dofRZId);
		nd->setDof(d);
		d->setNodeId(nodeId);
	}

	addNode(nd);
}

void DynamicSystem::addNodeWithDof(const int id, const double x, const int dofId)
{
	DOF *d = new DOF(dofId, RODS::Direction::X);
	Node *nd = new Node(id, x, 0.0, 0.0);
	nd->setDof(d);
	d->setNodeId(id);
	
	addDOF(d);
	addNode(nd);
}

void DynamicSystem::addNodePlate2D(const int nodeId, const double x, const double y, const int dofZId,
	const int dofRXId, const int dofRYId)
{
	Node *nd = new Node(nodeId, x, y, 0.0);

	nd->setDof(DOFs.at(dofZId));
	nd->setDof(DOFs.at(dofRXId));
	nd->setDof(DOFs.at(dofRYId));

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
	Line *l = new Line(id, Points.at(ni), Points.at(nj));
	addLine(l);
}

void DynamicSystem::fixDOF(const int dofId)
{
	DOFs.at(dofId)->setFixed();
}

void DynamicSystem::freeDOF(const int dofId)
{
	DOFs.at(dofId)->setFixed(false);
}

void DynamicSystem::fixNode(const int nodeId)
{
	Nodes.at(nodeId)->fixDOF();
}

void DynamicSystem::fixNode(const int nodeId, RODS::Direction dir)
{
	Nodes.at(nodeId)->fixDOF(dir);
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

void DynamicSystem::addLoad(const int id, const double arriveTime, const double scale)
{
	if (Loads.count(id) == 0)
	{
		Load *load = new Load(id, arriveTime, scale);
		Loads[id] = load;
	}
	else
	{
		cout << "Load ID: " << id << " already exists! The load will not be added." << endl;
	}
}

void DynamicSystem::addLoadValue(const int id, const double time, const double value)
{
	auto load = Loads.at(id);
	if (time > load->finishTime) load->addValue(time, value);
	else cout << "Time series should be input in an ascending order. The value will not be added." << endl;
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

	if (ROD2Ds.size() > 1)
	{
		outFile << "$Elements" << std::endl;
		outFile << ROD2Ds.size() << std::endl;
		for (auto it = ROD2Ds.begin(); it != ROD2Ds.end(); ++it)
		{
			auto ele = it->second;
			outFile << it->first << " 1 1 1 " << ele->nodeI->id << " " << ele->nodeJ->id << std::endl;
		}
		outFile << "$EndElements" << std::endl;
	}

	if (ROD3Ds.size() > 1)
	{
		outFile << "$Elements" << std::endl;
		outFile << ROD3Ds.size() << std::endl;
		for (auto it = ROD3Ds.begin(); it != ROD3Ds.end(); ++it)
		{
			auto ele = it->second;
			outFile << it->first << " 1 1 1 " << ele->nodeI->id << " " << ele->nodeJ->id << std::endl;
		}
		outFile << "$EndElements" << std::endl;
	}

	if (Quad2Ds.size() > 1)
	{
		outFile << "$Elements" << std::endl;
		outFile << Quad2Ds.size() << std::endl;
		for (auto it = Quad2Ds.begin(); it != Quad2Ds.end(); it++)
		{
			auto ele = it->second;
			outFile << it->first << " 3 1 1 " << ele->nodeI->id << " " << ele->nodeJ->id << " "
				<< ele->nodeP->id << " " << ele->nodeQ->id << std::endl;
		}
		outFile << "$EndElements" << std::endl;
	}

	if (Tri2Ds.size() > 1)
	{
		outFile << "$Elements" << std::endl;
		outFile << Tri2Ds.size() << std::endl;
		for (auto it = Tri2Ds.begin(); it != Tri2Ds.end(); it++)
		{
			auto ele = it->second;
			outFile << it->first << " 2 1 1 " << ele->nodeI->id << " " << ele->nodeJ->id << " "
				<< ele->nodeP->id << std::endl;
		}
		outFile << "$EndElements" << std::endl;
	}

	if (RectShell4Elastics.size() > 1)
	{
		outFile << "$Elements" << std::endl;
		outFile << RectShell4Elastics.size() << std::endl;
		for (auto it = RectShell4Elastics.begin(); it != RectShell4Elastics.end(); it++)
		{
			auto ele = it->second;
			outFile << it->first << " 3 1 1 " << ele->nodeI->id << " " << ele->nodeJ->id << " "
				<< ele->nodeP->id << " " << ele->nodeQ->id << std::endl;
		}
		outFile << "$EndElements" << std::endl;
	}
}

void DynamicSystem::exportModalGmsh(char* fileName, const int order)
{
	std::ofstream outFile;
	outFile.open(fileName, ios::out|ios::app);

	outFile << "$NodeData" << std::endl;

	int od = order-1;
	if (order > eqnCount) od = eqnCount-1;

	vec modeshape = Phi.col(od);
	for (int i = 0; i < eqnCount; i++)
	{
		DOF *d = DOFs.at(eqnMapDof.at(i));
		d->dsp = modeshape(i);
	}

	outFile << "1" << std::endl;
	outFile << "\"Mode: " << od+1 << ", T = " << P(od) << "s\"" << std::endl;
	outFile << "1" << std::endl;
	outFile << "0.0" << std::endl;
	outFile << "3" << std::endl;
	outFile << "0" << std::endl;
	outFile << "3" << std::endl;
	outFile << Nodes.size() << std::endl;

	for (auto it = Nodes.begin(); it != Nodes.end(); it++)
	{
		auto nd = it->second;
		if (nd->isActivated(RODS::Direction::Y))
		{
			outFile << it->first << " " << nd->dofX->dsp << " " << nd->dofY->dsp << " " << nd->dofZ->dsp << std::endl;
		}
		else
		{
			outFile << it->first << " " << nd->dofX->dsp << " " << 0.0 << " " << nd->dofZ->dsp << std::endl;
		}
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

void DynamicSystem::exportResponseGmsh(char* fileName)
{
	std::ofstream outFile;
	outFile.open(fileName, ios::out|ios::app);

	outFile << "$NodeData" << std::endl;

	outFile << "1" << std::endl;
	outFile << "\"Step time: t = " << ctime << "s\"" << std::endl;
	outFile << "1" << std::endl;
	outFile << ctime << std::endl;
	outFile << "3" << std::endl;
	outFile << "0" << std::endl;
	outFile << "3" << std::endl;
	outFile << Nodes.size() << std::endl;

	for (auto it = Nodes.begin(); it != Nodes.end(); it++)
	{
		auto nd = it->second;
		if (nd->isActivated(RODS::Direction::Y))
		{
			outFile << it->first << " " << nd->dofX->dsp << " " << nd->dofY->dsp << " " << nd->dofZ->dsp << std::endl;
		}
		else
		{
			outFile << it->first << " " << nd->dofX->dsp << " " << 0.0 << " " << nd->dofZ->dsp << std::endl;
		}
	}
	
	outFile << "$EndNodeData" << std::endl;

	outFile.close();
}

void DynamicSystem::exportResponseGmsh()
{
	gmshFile << "$NodeData" << std::endl;

	gmshFile << "1" << std::endl;
	//gmshFile << "\"Step time: t = " << ctime << "s\"" << std::endl;
	gmshFile << "\"Response\"" << std::endl;
	gmshFile << "1" << std::endl;
	gmshFile << ctime << std::endl;
	gmshFile << "3" << std::endl;
	//gmshFile << "0" << std::endl;
	gmshFile << cstep << std::endl;
	gmshFile << "3" << std::endl;
	gmshFile << Nodes.size() << std::endl;

	for (auto it = Nodes.begin(); it != Nodes.end(); it++)
	{
		auto nd = it->second;
		if (nd->isActivated(RODS::Direction::Y))
		{
			gmshFile << it->first << " " << nd->dofX->dsp << " " << nd->dofY->dsp << " " << nd->dofZ->dsp << std::endl;
		}
		else
		{
			gmshFile << it->first << " " << nd->dofX->dsp << " " << 0.0 << " " << nd->dofZ->dsp << std::endl;
		}
	}

	gmshFile << "$EndNodeData" << std::endl;
}

void DynamicSystem::setResponseGmsh(char* fileName, const int interval)
{
	gmshFileName = fileName;
	exportGmshInterval = interval;
}

void DynamicSystem::getDofModalResponse(double *res, const int order)
{
	int od = order-1;
	if (order > eqnCount) od = 1;

	vec modeshape = Phi.col(od);

	for (int i = 0; i < eqnCount; i++)
	{
		DOF *d = DOFs.at(eqnMapDof.at(i));
		d->dsp = modeshape(i);
	}

	auto i = 0;
	for (auto it = DOFs.begin(); it != DOFs.end(); it++)
	{
		res[i++] = it->second->dsp;
	}
}

void DynamicSystem::addDOF(DOF * d)
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

void DynamicSystem::addDOF(const int id, const double m, const bool fixed)
{
	DOF *d = new DOF(id, RODS::Direction::X, m, fixed);
	addDOF(d);
}

void DynamicSystem::addDOF(const int id, RODS::Direction dir, const double m, const bool fixed)
{
	DOF *d = new DOF(id, dir, m, fixed);
	addDOF(d);
}

void DynamicSystem::removeDOF(const int id)
{
	DOFs.erase(id);
}

void DynamicSystem::setMass(const int id, const double m)
{
	DOFs.at(id)->setMass(m);
}

void DynamicSystem::setNodeMass(const int id, const double m)
{
	Node *nd = Nodes.at(id);
	if (nd->isActivated(RODS::Direction::X))  nd->dofX->setMass(m);
	if (nd->isActivated(RODS::Direction::Y))  nd->dofY->setMass(m);
	if (nd->isActivated(RODS::Direction::Z))  nd->dofZ->setMass(m);
	if (nd->isActivated(RODS::Direction::RX)) nd->dofRX->setMass(m);
	if (nd->isActivated(RODS::Direction::RY)) nd->dofRY->setMass(m);
	if (nd->isActivated(RODS::Direction::RZ)) nd->dofRZ->setMass(m);

}

void DynamicSystem::setNodeMass(const int id, const double m, const double J)
{
	Node *nd = Nodes.at(id);
	if (nd->isActivated(RODS::Direction::X))  nd->dofX->setMass(m);
	if (nd->isActivated(RODS::Direction::Y))  nd->dofY->setMass(m);
	if (nd->isActivated(RODS::Direction::Z))  nd->dofZ->setMass(m);
	if (nd->isActivated(RODS::Direction::RX)) nd->dofRX->setMass(J);
	if (nd->isActivated(RODS::Direction::RY)) nd->dofRY->setMass(J);
	if (nd->isActivated(RODS::Direction::RZ)) nd->dofRZ->setMass(J);
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

bool DynamicSystem::checkDuplicateMaterial1D(const int matId)
{
	if (Material1Ds.count(matId) == 0)
	{
		return true;
	}
	else
	{
		cout << "Material1D ID: " << matId << " already exists! The material will not be added." << endl;
		return false;
	}
}

void DynamicSystem::addMaterialElastic(const int id, const double E0)
{
	if ( checkDuplicateMaterial1D(id) )
	{
		Material1D *mt = new Elastic(id, E0);
		Material1Ds[id] = mt;
	}
}

void DynamicSystem::addMaterialElastoplastic(const int id, const double E0, const double fy, const double alpha)
{
	if ( checkDuplicateMaterial1D(id) )
	{
		Material1D *mt = new Elastoplastic(id, E0, fy, alpha);
		Material1Ds[id] = mt;
	}
}

void DynamicSystem::addMaterialSteelBilinear(const int id, const double E0, const double fy, const double alpha, const double beta)
{
	if ( checkDuplicateMaterial1D(id) )
	{
		Material1D *mt = new SteelBilinear(id, E0, fy, alpha, beta);
		Material1Ds[id] = mt;
	}
}

void DynamicSystem::addMaterialConcreteTrilinear(const int id, const double E0, const double fc, const double epsilon_c, const double sigma_cr, const double sigma_u, const double epsilon_u)
{
	if ( checkDuplicateMaterial1D(id) )
	{
		Material1D *mt = new ConcreteTrilinear(id, E0, fc, epsilon_c, sigma_cr, sigma_u, epsilon_u);
		Material1Ds[id] = mt;
	}
}

void DynamicSystem::addMaterialSMABilinear(const int id, const double E0, const double fy, const double alpha, const double sigma_shift)
{
	if ( checkDuplicateMaterial1D(id) )
	{
		Material1D *mt = new SMABilinear(id, E0, fy, alpha, sigma_shift);
		Material1Ds[id] = mt;
	}
}

void DynamicSystem::addMaterialCyclicHardenTrilinear(const int id, const double E, const double sigma1,
	const double alpha1, const double sigma2, const double alpha2)
{
	if ( checkDuplicateMaterial1D(id) )
	{
		Material1D *mt = new CyclicHardenTrilinear(id, E, sigma1, alpha1, sigma2, alpha2);
		Material1Ds[id] = mt;
	}
}

void DynamicSystem::addFiber(const int id, const int matId, const double A, const double y, const double z)
{
	if (Fibers.count(id) == 0)
	{
		Fiber *fiber = new Fiber(id, Material1Ds.at(matId), A, y, z);
		Fibers[id] = fiber;
	}
	else
	{
		cout << "Fiber ID: " << id << " already exists! The fiber will not be added." << endl;
	}
}

void DynamicSystem::addSectionTruss(const int id, int* fiberIds, const int nFibers)
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
	}
	else
	{
		cout << "Section ID: " << id << " already exists! The section will not be added." << endl;
	}
}

void DynamicSystem::addSectionFrame2D(const int id, int* fiberIds, const int nFibers)
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
	}
	else
	{
		cout << "Section ID: " << id << " already exists! The section will not be added." << endl;
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

bool DynamicSystem::checkDuplicateElement(const int eleId)
{
	if (Elements.count(eleId) == 0)
	{
		return true;
	}
	else
	{
		cout << "Element ID: " << eleId << " already exists! The element will not be added." << endl;
		return false;
	}
}

void DynamicSystem::addSpring(const int id, const int ni, const int nj, const double k)
{
	if ( !checkDuplicateElement(id) ) return;
	Spring *s = new Spring(id, DOFs.at(ni), DOFs.at(nj), k);
	Elements[id] = s;
	Springs[id] = s;
	ROD1Ds[id] = s;
	linearElasticElements[id] = s;
}

void DynamicSystem::removeSpring(const int id)
{
	Springs.erase(id);
	Elements.erase(id);
	ROD1Ds.erase(id);
    linearElasticElements.erase(id);
}

void DynamicSystem::addSpringBilinear(const int id, const int ni, const int nj, const double k0, const double uy, const double alpha)
{
	if ( !checkDuplicateElement(id) ) return;

	SpringBilinear *s = new SpringBilinear(id, DOFs.at(ni), DOFs.at(nj), k0, uy, alpha);
	Elements[id] = s;
	SpringBilinears[id] = s;
	ROD1Ds[id] = s;
	nonlinearTangentElements[id] = s;
}

void DynamicSystem::addSpringNonlinear(const int id, const int ni, const int nj, const int matId)
{
	if ( !checkDuplicateElement(id) ) return;

	SpringNonlinear *s = new SpringNonlinear(id, DOFs.at(ni), DOFs.at(nj), Material1Ds.at(matId));
	Elements[id] = s;
	SpringNonlinears[id] = s;
	ROD1Ds[id] = s;
	nonlinearTangentElements[id] = s;
}

void DynamicSystem::addSpringNonlinear2D(const int id, const int ni, const int nj, const int matId, RODS::LocalAxis U)
{
	if ( !checkDuplicateElement(id) ) return;

	SpringNonlinear2D *s = new SpringNonlinear2D(id, Nodes.at(ni), Nodes.at(nj), Material1Ds.at(matId), U);
	Elements[id] = s;
	SpringNonlinear2Ds[id] = s;
	ROD2Ds[id] = s;
	nonlinearTangentElements[id] = s;
}

void DynamicSystem::addSpringNonlinear3D(const int id, const int ni, const int nj, const int matId, RODS::LocalAxis U)
{
	if ( !checkDuplicateElement(id) ) return;

	SpringNonlinear3D *s = new SpringNonlinear3D(id, Nodes.at(ni), Nodes.at(nj), Material1Ds.at(matId), U);
	Elements[id] = s;
	SpringNonlinear3Ds[id] = s;
	ROD3Ds[id] = s;
	nonlinearTangentElements[id] = s;
}

void DynamicSystem::addSpringBoucWen(const int id, const int ni, const int nj, const double k0, const double uy, const double alpha, const double beta, const double n)
{
	if ( !checkDuplicateElement(id) ) return;

	SpringBoucWen *s = new SpringBoucWen(id, DOFs.at(ni), DOFs.at(nj), k0, uy, alpha);
	Elements[id] = s;
	SpringBoucWens[id] = s;
	ROD1Ds[id] = s;
	nonlinearTangentElements[id] = s;
}

void DynamicSystem::addDashpot(const int id, const int ni, const int nj, const double c)
{
	if ( !checkDuplicateElement(id) ) return;

	Dashpot *d = new Dashpot(id, DOFs.at(ni), DOFs.at(nj), c);
	Elements[id] = d;
	Dashpots[id] = d;
	ROD1Ds[id] = d;
	linearDampingElements[id] = d;
}

void DynamicSystem::removeDashpot(const int id)
{
	Dashpots.erase(id);
	Elements.erase(id);
	ROD1Ds.erase(id);
	linearDampingElements.erase(id);
}

void DynamicSystem::addDashpotExp(const int id, const int ni, const int nj, const double c, const double alpha)
{
	if ( !checkDuplicateElement(id) ) return;

	DashpotExp *d = new DashpotExp(id, DOFs.at(ni), DOFs.at(nj), c, alpha);
	Elements[id] = d;
	DashpotExps[id] = d;
	ROD1Ds[id] = d;
	nonlinearElements[id] = d;
}

void DynamicSystem::addDashpotMaxwell(const int id, const int ni, const int nj, const double k, const double c, const double alpha)
{
	if ( !checkDuplicateElement(id) ) return;

	DashpotMaxwell *d = new DashpotMaxwell(id, DOFs.at(ni), DOFs.at(nj), c, alpha);
	Elements[id] = d;
	DashpotMaxwells[id] = d;
	ROD1Ds[id] = d;
	nonlinearElements[id] = d;
}

void DynamicSystem::addInerter(const int id, const int i, const int j, const double m)
{
	if ( !checkDuplicateElement(id) ) return;
	Inerter *in = new Inerter(id, DOFs.at(i), DOFs.at(j), m);
	Elements[id] = in;
	Inerters[id] = in;
	ROD1Ds[id] = in;
	inertialMassElements[id] = in;
}

void DynamicSystem::removeInerter(const int id)
{
	Inerters.erase(id);
	Elements.erase(id);
	ROD1Ds.erase(id);
	inertialMassElements.erase(id);
}

void DynamicSystem::addSlider(const int id, const int i, const int j, const double muN)
{
	if ( !checkDuplicateElement(id) ) return;

	Slider *s = new Slider(id, DOFs.at(i), DOFs.at(j), muN);
	Elements[id] = s;
	Sliders[id] = s;
	ROD1Ds[id] = s;
	nonlinearElements[id] = s;
}

void DynamicSystem::addSPIS2(const int id, const int i, const int j, const int in, const double m, const double c, const double k)
{
	if ( !checkDuplicateElement(id) ) return;

	SPIS2 *s = new SPIS2(id, DOFs.at(i), DOFs.at(j), DOFs.at(in), m, c, k);
	Elements[id] = s;
	SPIS2s[id] = s;
	ROD1Ds[id] = s;
	linearElasticElements[id] = s;
	linearDampingElements[id] = s;
	inertialMassElements[id] = s;
}

void DynamicSystem::addTVMD(const int id, const int ni, const int nj, const double m, const double c, const double k)
{
	if ( !checkDuplicateElement(id) ) return;

	TVMD *d = new TVMD(id, DOFs.at(ni), DOFs.at(nj), m, c, k);
	Elements[id] = d;
	TVMDs[id] = d;
	ROD1Ds[id] = d;
	nonlinearElements[id] = d;
}

void DynamicSystem::addTVMD2D(const int id, const int ni, const int nj, const double m, const double c, const double k, RODS::LocalAxis U)
{
	if (!checkDuplicateElement(id)) return;

	TVMD2D *d = new TVMD2D(id, Nodes.at(ni), Nodes.at(nj), m, c, k, U);
	Elements[id] = d;
	ROD2Ds[id] = d;
	TVMD2Ds[id] = d;
	nonlinearElements[id] = d;
}

void DynamicSystem::addTVMD3D(const int id, const int ni, const int nj, const double m, const double c, const double k,
	RODS::LocalAxis U)
{
	if (!checkDuplicateElement(id)) return;

	TVMD3D *d = new TVMD3D(id, Nodes.at(ni), Nodes.at(nj), m, c, k, U);
	Elements[id] = d;
	ROD3Ds[id] = d;
	TVMD3Ds[id] = d;
	nonlinearElements[id] = d;
}

void DynamicSystem::addSpring2D(const int id, const int ni, const int nj, const double k, RODS::LocalAxis U)
{
	if (!checkDuplicateElement(id)) return;

	Spring2D *s = new Spring2D(id, Nodes.at(ni), Nodes.at(nj), k, U);
	Elements[id] = s;
	ROD2Ds[id] = s;
	Spring2Ds[id] = s;
	linearElasticElements[id] = s;
}

void DynamicSystem::addSpring3D(const int id, const int ni, const int nj, const double k, RODS::LocalAxis U)
{
	if (!checkDuplicateElement(id)) return;

	Spring3D *s = new Spring3D(id, Nodes.at(ni), Nodes.at(nj), k, U);
	Elements[id] = s;
	ROD3Ds[id] = s;
	Spring3Ds[id] = s;
	linearElasticElements[id] = s;
}

void DynamicSystem::addSpringBoucWen2D(const int id, const int ni, const int nj, const double k0, const double uy, const double alpha, const double beta, const double n, RODS::LocalAxis U /*= RODS::U1*/)
{
	if (!checkDuplicateElement(id)) return;

	SpringBoucWen2D *s = new SpringBoucWen2D(id, Nodes.at(ni), Nodes.at(nj), k0, uy, alpha, beta, n, U);
	Elements[id] = s;
	ROD2Ds[id] = s;
	SpringBoucWen2Ds[id] = s;
	nonlinearTangentElements[id] = s;
}

void DynamicSystem::addDashpot2D(const int id, const int ni, const int nj, const double c, RODS::LocalAxis U)
{
	if (!checkDuplicateElement(id)) return;

	Dashpot2D *d = new Dashpot2D(id, Nodes.at(ni), Nodes.at(nj), c, U);
	Elements[id] = d;
	ROD2Ds[id] = d;
	Dashpot2Ds[id] = d;
	linearDampingElements[id] = d;
}

void DynamicSystem::addInerter2D(const int id, const int ni, const int nj, const double m, RODS::LocalAxis U)
{
	if (!checkDuplicateElement(id)) return;

	Inerter2D *in = new Inerter2D(id, Nodes.at(ni), Nodes.at(nj), m, U);
	Elements[id] = in;
	ROD2Ds[id] = in;
	Inerter2Ds[id] = in;
	inertialMassElements[id] = in;
}

void DynamicSystem::addSpringBilinear2D(const int id, const int ni, const int nj, const double k0, const double uy,
	const double alpha, RODS::LocalAxis U)
{
	if ( !checkDuplicateElement(id) ) return;

	SpringBilinear2D *s = new SpringBilinear2D(id, Nodes.at(ni), Nodes.at(nj), k0, uy, alpha, U);
	Elements[id] = s;
	ROD2Ds[id] = s;
	SpringBilinear2Ds[id] = s;
	nonlinearTangentElements[id] = s;
}

void DynamicSystem::addSpringBilinear3D(const int id, const int ni, const int nj, const double k0, const double uy,
	const double alpha, RODS::LocalAxis U)
{
	if ( !checkDuplicateElement(id) ) return;

	SpringBilinear3D *s = new SpringBilinear3D(id, Nodes.at(ni), Nodes.at(nj), k0, uy, alpha, U);
	Elements[id] = s;
	ROD3Ds[id] = s;
	SpringBilinear3Ds[id] = s;
	nonlinearTangentElements[id] = s;
}

void DynamicSystem::addDashpot3D(const int id, const int ni, const int nj, const double c, RODS::LocalAxis U)
{
	if (!checkDuplicateElement(id)) return;

	Dashpot3D *d = new Dashpot3D(id, Nodes.at(ni), Nodes.at(nj), c, U);
	Elements[id] = d;
	ROD3Ds[id] = d;
	Dashpot3Ds[id] = d;
	linearDampingElements[id] = d;
}

void DynamicSystem::addInerter3D(const int id, const int ni, const int nj, const double m, RODS::LocalAxis U)
{
	Inerter3D *in = new Inerter3D(id, Nodes.at(ni), Nodes.at(nj), m, U);
	Elements[id] = in;
	ROD3Ds[id] = in;
	Inerter3Ds[id] = in;
	inertialMassElements[id] = in;
}

void DynamicSystem::addDashpotExp2D(const int id, const int ni, const int nj, const double c, const double alpha, RODS::LocalAxis U /*= RODS::U1*/)
{
	if (!checkDuplicateElement(id)) return;

	DashpotExp2D *d = new DashpotExp2D(id, Nodes.at(ni), Nodes.at(nj), c, alpha, U);
	Elements[id] = d;
	ROD2Ds[id] = d;
	DashpotExp2Ds[id] = d;
	nonlinearElements[id] = d;
}

void DynamicSystem::addDashpotMaxwell2D(const int id, const int ni, const int nj, const double k, const double c, const double alpha, RODS::LocalAxis U)
{
	if (!checkDuplicateElement(id)) return;
	DashpotMaxwell2D *d = new DashpotMaxwell2D(id, Nodes.at(ni), Nodes.at(nj), k, c, alpha, U);
	Elements[id] = d;
	ROD2Ds[id] = d;
	DashpotMaxwell2Ds[id] = d;
	nonlinearElements[id] = d;
}

void DynamicSystem::addDashpotMaxwell3D(const int id, const int ni, const int nj, const double k, const double c,
	const double alpha, RODS::LocalAxis U)
{
	if (!checkDuplicateElement(id)) return;
	DashpotMaxwell3D *d = new DashpotMaxwell3D(id, Nodes.at(ni), Nodes.at(nj), k, c, alpha, U);
	Elements[id] = d;
	ROD3Ds[id] = d;
	DashpotMaxwell3Ds[id] = d;
	nonlinearElements[id] = d;
}

void DynamicSystem::addTrussElastic2D(const int id, const int ni, const int nj, const double EA)
{
	if (!checkDuplicateElement(id)) return;

	TrussElastic2D *truss = new TrussElastic2D(id, Nodes.at(ni), Nodes.at(nj), EA);
	Elements[id] = truss;
	ROD2Ds[id] = truss;
	TrussElastic2Ds[id] = truss;
	linearElasticElements[id] = truss;
}

void DynamicSystem::addTrussElastic3D(const int id, const int ni, const int nj, const double EA)
{
	if (!checkDuplicateElement(id)) return;

	TrussElastic3D *truss = new TrussElastic3D(id, Nodes.at(ni), Nodes.at(nj), EA);
	Elements[id] = truss;
	ROD3Ds[id] = truss;
	TrussElastic3Ds[id] = truss;
	linearElasticElements[id] = truss;
}

void DynamicSystem::addBeamElastic2D(const int id, const int ni, const int nj, const double EI)
{
	if (!checkDuplicateElement(id)) return;

	BeamElastic2D *beam = new BeamElastic2D(id, Nodes.at(ni), Nodes.at(nj), EI);
	Elements[id] = beam;
	ROD2Ds[id] = beam;
	BeamElastic2Ds[id] = beam;
	linearElasticElements[id] = beam;
}

void DynamicSystem::addFrameElastic2D(const int id, const int ni, const int nj, const double EA, const double EI)
{
	if (!checkDuplicateElement(id)) return;

	FrameElastic2D *frame = new FrameElastic2D(id, Nodes.at(ni), Nodes.at(nj), EA, EI);
	Elements[id] = frame;
	ROD2Ds[id] = frame;
	FrameElastic2Ds[id] = frame;
	linearElasticElements[id] = frame;
}

void DynamicSystem::addFrameElastic3D(const int id, const int ni, const int nj, const double EA, const double EIy,
	const double EIz, const double GIp)
{
	if (!checkDuplicateElement(id)) return;

	FrameElastic3D *frame = new FrameElastic3D(id, Nodes.at(ni), Nodes.at(nj), EA, EIy, EIz, GIp);
	Elements[id] = frame;
	ROD3Ds[id] = frame;
	FrameElastic3Ds[id] = frame;
	linearElasticElements[id] = frame;
}

void DynamicSystem::addTri3Elastic(const int id, const int nodeI, const int nodeJ, const int nodeP, const double E,
	const double nu, const double t)
{
	if (!checkDuplicateElement(id)) return;

	Tri3Elastic *tri = new Tri3Elastic(id, Nodes.at(nodeI), Nodes.at(nodeJ), Nodes.at(nodeP), E, nu, t);
	Elements[id] = tri;
	Tri3Elastics[id] = tri;
	Tri2Ds[id] = tri;
	linearElasticElements[id] = tri;
}

void DynamicSystem::addRect4Elastic(const int id, const int nodeI, const int nodeJ, const int nodeP, const int nodeQ,
	const double E, const double nu, const double t)
{
	if (!checkDuplicateElement(id)) return;
	Rect4Elastic *rect = new Rect4Elastic(id, Nodes.at(nodeI), Nodes.at(nodeJ), Nodes.at(nodeP), Nodes.at(nodeQ), E, nu, t);
	Elements[id] = rect;
	Rect4Elastics[id] = rect;
	Quad2Ds[id] = rect;
	linearElasticElements[id] = rect;
}

void DynamicSystem::addPlate4Elastic(const int id, const int nodeI, const int nodeJ, const int nodeP, const int nodeQ,
	const double E, const double nu, const double t)
{
	if (!checkDuplicateElement(id)) return;
	Plate4Elastic *plate = new Plate4Elastic(id, Nodes.at(nodeI), Nodes.at(nodeJ), Nodes.at(nodeP), Nodes.at(nodeQ), E, nu, t);
	Elements[id] = plate;
	Plate4Elastics[id] = plate;
	Quad2Ds[id] = plate;
	linearElasticElements[id] = plate;
}

void DynamicSystem::addQuad4Elastic(const int id, const int nodeI, const int nodeJ, const int nodeP, const int nodeQ,
                                    const double E, const double nu, const double t)
{
	if (!checkDuplicateElement(id)) return;

	Quad4Elastic *quad = new Quad4Elastic(id, Nodes.at(nodeI), Nodes.at(nodeJ), Nodes.at(nodeP), Nodes.at(nodeQ), E, nu, t);
	Elements[id] = quad;
	Quad4Elastics[id] = quad;
	Quad2Ds[id] = quad;
	linearElasticElements[id] = quad;
}

void DynamicSystem::addRectShell4Elastic(const int id, const int nodeI, const int nodeJ, const int nodeP,
	const int nodeQ, const double E, const double nu, const double t)
{
	if (!checkDuplicateElement(id)) return;
	RectShell4Elastic *rect = new RectShell4Elastic(id, Nodes.at(nodeI), Nodes.at(nodeJ), Nodes.at(nodeP), Nodes.at(nodeQ), E, nu, t);
	Elements[id] = rect;
	RectShell4Elastics[id] = rect;
	linearElasticElements[id] = rect;
}

void DynamicSystem::addTruss2D(const int id, const int ni, const int nj, const int secId)
{
	if (!checkDuplicateElement(id)) return;

	Truss2D *truss = new Truss2D(id, Nodes.at(ni), Nodes.at(nj), SectionTrusss.at(secId));
	Elements[id] = truss;
	ROD2Ds[id] = truss;
	Truss2Ds[id] = truss;
	nonlinearInitialTangentElements[id] = truss;
}

void DynamicSystem::addFrame2D(const int id, const int ni, const int nj, const int secId, const int nIntP)
{
	if (!checkDuplicateElement(id)) return;

	Frame2D *frame = new Frame2D(id, Nodes.at(ni), Nodes.at(nj), SectionFrame2Ds.at(secId), nIntP);
	Elements[id] = frame;
	ROD2Ds[id] = frame;
	Frame2Ds[id] = frame;
	nonlinearInitialTangentElements[id] = frame;
}

void DynamicSystem::addFramePDelta2D(const int id, const int ni, const int nj, const int secId, const int nIntP)
{
	if (!checkDuplicateElement(id)) return;

	FramePDelta2D *frame = new FramePDelta2D(id, Nodes.at(ni), Nodes.at(nj), SectionFrame2Ds.at(secId), nIntP);
	Elements[id] = frame;
	ROD2Ds[id] = frame;
	FramePDelta2Ds[id] = frame;
	nonlinearInitialTangentElements[id] = frame;
}

void DynamicSystem::addWave(const int id, const double dt, const vec &s)
{
	if (Waves.count(id) > 0)
	{
		cout << "Wave ID: " << id << " already exists! The wave will not be added." << endl;
		return;
	}

	Wave *ts = new Wave(id, dt, s);
	Waves[ts->id] = ts;
}

void DynamicSystem::addWave(const int id, const double dt, const char* fileName)
{
	if (Waves.count(id) > 0)
	{
		cout << "Wave ID: " << id << " already exists! The wave will not be added." << endl;
		return;
	}
	
	Wave *ts = new Wave(id, dt, fileName);
	Waves[ts->id] = ts;
}

void DynamicSystem::addDOFRecorder(const int id, int *dofIds, const int n, RODS::Response rType, const char * fileName)
{
	if (DOFRecorders.count(id) > 0)
	{
		cout << "DOFRecorder ID: " << id << " already exists! The recorder will not be added." << endl;
		return;
	}

	std::vector<DOF *> rdofs(n);

	for (int i = 0; i < n; i++)
	{
		rdofs[i] = DOFs.at(dofIds[i]);
	}

	DOFRecorder *dr = new DOFRecorder(id, rdofs, rType, fileName);
	DOFRecorders[dr->id] = dr;
}

void DynamicSystem::addDOFRecorder(const int id, RODS::Response rType, const char* fileName)
{
	if (DOFRecorders.count(id) > 0)
	{
		cout << "DOFRecorder ID: " << id << " already exists! The recorder will not be added." << endl;
		return;
	}

	DOFRecorder *dr = new DOFRecorder(id, rType, fileName);
	DOFRecorders[dr->id] = dr;
}

void DynamicSystem::addDOFToRecorder(const int dofId, const int rId)
{
	DOFRecorder *dr = DOFRecorders.at(rId);
	dr->add_dof(DOFs.at(dofId));
}

void DynamicSystem::addElementRecorder(const int id, int * eleIds, const int n, RODS::Response rType, const char * fileName)
{
	if (ElementRecorders.count(id) > 0)
	{
		cout << "ElementRecorder ID: " << id << " already exists! The recorder will not be added." << endl;
		return;
	}

	std::vector<Element *> reles(n);

	for (int i = 0; i < n; i++)
	{
		reles[i] = Elements.at(eleIds[i]);
	}

	ElementRecorder *er = new ElementRecorder(id, reles, rType, fileName);
	ElementRecorders[er->id] = er;
}

void DynamicSystem::addElementRecorder(const int id, RODS::Response rType, const char* fileName)
{
	if (ElementRecorders.count(id) > 0)
	{
		cout << "ElementRecorder ID: " << id << " already exists! The recorder will not be added." << endl;
		return;
	}

	ElementRecorder *er = new ElementRecorder(id, rType, fileName);
	ElementRecorders[er->id] = er;
}

void DynamicSystem::addElementToRecorder(const int eleId, const int rId)
{
	ElementRecorder *er = ElementRecorders.at(rId);
	er->add_ele(Elements.at(eleId));
}

void DynamicSystem::setDofRecorderFileName(const int id, char* fileName)
{
	DOFRecorders.at(id)->setFileName(fileName);
}

void DynamicSystem::setElementRecorderFileName(const int id, char* fileName)
{
	ElementRecorders.at(id)->setFileName(fileName);
}

void DynamicSystem::setRayleighDamping(const double omg1, const double omg2)
{
	useRayleighDamping = true;
	RayleighOmg1 = omg1;
	RayleighOmg2 = omg2;
}

void DynamicSystem::setRayleighDamping(const int i, const int j)
{
	useRayleighDamping = true;
	RayleighOmg1 = omg(i-1);
	RayleighOmg2 = omg(j-1);
}

void DynamicSystem::activeGroundMotion(RODS::Direction dir, const int waveId, const double waveScale)
{
	if (Waves.count(waveId) == 0)
	{
		cout << "Wave " << waveId << "do not exist! The groud motion is not activated." << endl;
		return;
	}

	DOF *dof = nullptr;
	switch (dir)
	{
		case RODS::Direction::X:
			XSeismicWaveId = waveId;
			XSeismicWaveScale = waveScale;
			for (int i = 0; i < eqnCount; i++)
			{
				dof = DOFs.at(eqnMapDof.at(i));
				if (dof->dir == dir)
				{
					EX(i) = 1.0;
				}
			}
			break;
		case RODS::Direction::Y:
			YSeismicWaveId = waveId;
			YSeismicWaveScale = waveScale;
			for (int i = 0; i < eqnCount; i++)
			{
				dof = DOFs.at(eqnMapDof.at(i));
				if (dof->dir == dir)
				{
					EY(i) = 1.0;
				}
			}
			break;
		case RODS::Direction::Z:
			ZSeismicWaveId = waveId;
			ZSeismicWaveScale = waveScale;
			for (int i = 0; i < eqnCount; i++)
			{
				dof = DOFs.at(eqnMapDof.at(i));
				if (dof->dir == dir)
				{
					EZ(i) = 1.0;
				}
			}
			break;
		default:
			break;
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
	assembleDampingMatrix();
	applyRestraint();

	if (useRayleighDamping)
	{
		buildRayleighDampingMatrix(RayleighOmg1, RayleighOmg2);
	}
	else
	{
		buildInherentDampingMatrix();
	}

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

	EX = zeros<vec>(eqnCount); // Ground motion vector
	EY = zeros<vec>(eqnCount); // Ground motion vector
	EZ = zeros<vec>(eqnCount); // Ground motion vector
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
	std::cout << eigenNum << std::endl;
	if (zeta == 0.0)
	{
		return;
	}
	else
	{
		if (P.n_rows == 0)
		{
			solveEigen();
		}
		mat Phi_ = Phi.head_cols(eigenNum);
		vec omg_ = omg.head_rows(eigenNum);
		mat MPhi = Mp * Phi_;
		mat C_;
		
		if (eigenVectorNormed)
		{
			C_ = diagmat(2.0*zeta*omg_);
			C += MPhi * C_ * MPhi.t();
		}
		else
		{
			C_ = diagmat(2.0*zeta*omg_ / diagvec(Phi_.t()*MPhi));
			C += MPhi * C_ * MPhi.t();
		}
	}
}

void DynamicSystem::buildRayleighDampingMatrix(const double omg1, const double omg2)
{
	if (zeta == 0.0)
	{
		return;
	}
	else
	{
		double a0 = zeta * 2.0*omg1*omg2 / (omg1 + omg2);
		double a1 = zeta * 2.0 / (omg1 + omg2);
		C += a0 * Mp + a1 * K;
	}
}

void DynamicSystem::buildRayleighDampingMatrix(const int md1, const int md2)
{
	if (zeta == 0.0)
	{
		return;
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
	C = zeros<mat>(eqnCount, eqnCount);
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

	//mat L = chol(M, "lower");
	//mat L_i = L.i();
	//mat A = L_i*K*L_i.t();

	//vec omg_;
	//mat Phi_;
	//
	//eig_sym(omg_, Phi_, A);
	//omg = sqrt(omg_);
	//Phi = solve(L.t(), Phi_);

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

void DynamicSystem::recordAllDofResponse(const int id)
{
	string fileName = workDir + "/"  + name + "_dof_response.txt";
	addDOFRecorder(id, RODS::Response::ALL, fileName.c_str());

	for (auto it = DOFs.begin(); it != DOFs.end(); it++)
	{
		addDOFToRecorder(it->second->id, id);
	}
}

void DynamicSystem::recordAllEleResponse(const int id)
{
	string fileName = workDir + "/"  + name + "_ele_response.txt";
	addElementRecorder(id, RODS::Response::ALL, fileName.c_str());

	for (auto it = ROD1Ds.begin(); it != ROD1Ds.end(); it++)
	{
		addElementToRecorder(it->second->id, id);
	}
}

void DynamicSystem::solveStochasticSeismicResponse(const double f_h, const int nf, const char method)
{
	double omg_h = 2.0*PI*f_h;
	vec omg = linspace(0, omg_h, nf + 1);
	cx_double cx_I(0.0, 1.0);

	if (method == 'c')
	{
		vec F = zeros<vec>(2 * eqnCount);
		F.head(eqnCount) = -Mp * EX;

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
		cx_mat P = cx_vec(-Mp * EX, zeros<vec>(eqnCount));

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

void DynamicSystem::setConvergeParameter(const double tol, const int maxIter)
{
	this->tol = tol;
	this->maxIter = maxIter;
}

void DynamicSystem::solveLinearStaticResponse()
{
	Element::isStatic = true;

	ctime = 1.0;
	applyLoad();
	dsp = solve(K, Q);
	dsp0 = dsp;
	nsteps = 1;
	cstep = 1;
	//ctime = 0;
	initRecorders();
	setDofResponse();
	getElementResponse();
	recordResponse();
	saveResponse();
}

void DynamicSystem::solveNonlinearStaticResponse(const int nsub)
{
	Element::isStatic = true;

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

void DynamicSystem::solveNonlinearStaticResponse(const double loadedTime, const int nsub)
{
	Element::isStatic = true;

	int nIter = 0;
	double error = 1.0;

	vec du = zeros<vec>(eqnCount);

	nsteps = nsub;
	dt = loadedTime/nsub;

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
	Element::isStatic = true;

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
	Element::isStatic = true;

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

void DynamicSystem::solveSeismicResponse(const int nsub)
{
	bool isMultiDirectionExcitation = false;

	if (XSeismicWaveId < 0)
	{
		cout << "Ground Motion in the X Direction is not activated!" << endl;
		return;
	}

	if (YSeismicWaveId >= 0 || ZSeismicWaveId >= 0)
	{
		isMultiDirectionExcitation = true;
	}

	Element::isStatic = false;
	reassembleStiffnessMatrix();

	switch (dynamicSolver)
	{
	case RODS::DynamicSolver::Newmark:
		if (isMultiDirectionExcitation) solveSeismicResponseNewmarkMD(nsub);
		else solveSeismicResponseNewmark(nsub);
		break;
	case RODS::DynamicSolver::Newmark_NL:
		if (isMultiDirectionExcitation) solveSeismicResponseNewmarkNLMD(nsub);
		else solveSeismicResponseNewmarkNL(nsub);
		break;
	case RODS::DynamicSolver::StateSpace:
		if (isMultiDirectionExcitation) solveSeismicResponseStateSpaceMD(nsub);
		else solveSeismicResponseStateSpace(nsub);
		break;
	case RODS::DynamicSolver::StateSpace_NL:
		if (isMultiDirectionExcitation) solveSeismicResponseStateSpaceNLMD(nsub);
		else solveSeismicResponseStateSpaceNL(nsub);
		break;
	default:
		break;
	}

}

void DynamicSystem::solveSeismicResponseNewmark(const int nsub)
{
	nsteps = Waves.at(XSeismicWaveId)->nsteps;
	dt = Waves.at(XSeismicWaveId)->dt;
	vec agX = XSeismicWaveScale*Waves.at(XSeismicWaveId)->series;
	
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
	a0 = solve(M, -Mp*EX*agX(0) - C*v0);

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
		agd = (agX(i + 1) - agX(i)) / nsub;
		agi = agX(i);
		for (int j = 1; j < nsub + 1; j++)
		{
			agj = agi + agd * j;
			p_h = -Mp*EX*agj + c7*u0 + c8*v0 + c9*a0 + Q0;
			u_p = u0*1.0;
			u0 = solve(K_h, p_h);
			v0 = c3*(u0 - u_p) + c4*v0 + dt*c5*a0;
			a0 = solve(M, -Mp*EX*agj - C*v0 - K0*u0 + Q0);
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

void DynamicSystem::solveSeismicResponseNewmarkNL(const int nsub)
{
	nsteps = Waves.at(XSeismicWaveId)->nsteps;
	dt = Waves.at(XSeismicWaveId)->dt;
	vec agX = XSeismicWaveScale*Waves.at(XSeismicWaveId)->series;

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

	a0 = solve(M, -Mp*EX*agX(0) - C*v0);
	
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
		ctime += dt*nsub;
		agd = (agX(i + 1) - agX(i)) / nsub;
		agi = agX(i);
		for (int j = 1; j < nsub + 1; j++)
		{
			agj = agi + agd * j;
			p_h = -Mp*EX*agj + c7*u0 + c8*v0 + c9*a0;
			u_p = u0*1.0;
			v_p = v0*1.0;
			a_p = a0*1.0;
			error = 1.0;
			for (int l = 0; l < maxIter; l++)
			{
				du = solve(K_h_ + K, -K_h*u0 - q + p_h + Q0);
				u0 = u0 + du;
				v0 = c3*(u0 - u_p) + c4*v_p + dt*c5*a_p;
				a0 = solve(M, -Mp*EX*agj - q - C*v0 - K0*u0 + Q0);

				dsp = u0;
				vel = v0;
				acc = a0;

				setDofResponse();
				error = norm(du)/norm(u0);
				if (error > tol)
				{
					if (l < maxIter - 1)
					{
						assembleNonlinearForceVector(false);
						reassembleStiffnessMatrix();
					}
					else
					{
						cout << "Fail to Converge after " << maxIter << " iterations, "
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

void DynamicSystem::solveSeismicResponseStateSpace(const int nsub)
{
	nsteps = Waves.at(XSeismicWaveId)->nsteps;
	dt = Waves.at(XSeismicWaveId)->dt;
	vec agX = XSeismicWaveScale*Waves.at(XSeismicWaveId)->series;

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
	mat I_ = eye<mat>(eqnCount, eqnCount);
	mat A = -solve(M, K0);
	mat B = -solve(M, C);
	mat H = join_cols(join_rows(O, I_), join_rows(A, B));

	double h = dt;
	//mat T = expmat(H*h);
	mat T = expm(H, h);

	dsp = x0.head_rows(eqnCount);
	vel = x0.tail_rows(eqnCount);
	acc = solve(M, -Mp*EX*agX(0) - C*vel);

	setDofResponse();
	getElementResponse();
	recordResponse();

	double agd, agi, agj=0.0;
	for (int i = 0; i < nsteps - 1; i++)
	{
		cstep += 1;
		ctime += dt*nsub;
		agd = (agX(i + 1) - agX(i)) / nsub;
		agi = agX(i);
		for (int j = 0; j < nsub; j++)
		{
			agj = agi + agd*j;
			F.tail_rows(eqnCount) = solve(M, -Mp*EX*agj + Q0);
			x0 = T*(x0 + F*h);
		}

		dsp = x0.head_rows(eqnCount);
		vel = x0.tail_rows(eqnCount);
		acc = solve(M, -Mp*EX*agj - C*vel - K*dsp + Q0);

		setDofResponse();
		getElementResponse();
		recordResponse();
	}
	saveResponse();
}

void DynamicSystem::solveSeismicResponseStateSpaceNL(const int nsub)
{
	nsteps = Waves.at(XSeismicWaveId)->nsteps;
	dt = Waves.at(XSeismicWaveId)->dt;
	vec agX = XSeismicWaveScale*Waves.at(XSeismicWaveId)->series;

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
	mat I_ = eye<mat>(eqnCount, eqnCount);
	mat A = -solve(M, K0);
	mat B = -solve(M, C);
	mat H = join_cols(join_rows(O, I_), join_rows(A, B));

	double h = dt;
	//mat T = expmat(H*h);
	mat T = expm(H, h);

	dsp = x0.head_rows(eqnCount);
	vel = x0.tail_rows(eqnCount);
	acc = solve(M, -Mp*EX*agX(0) - C*vel);

	setDofResponse();
	getElementResponse();
	assembleNonlinearForceVector(true);
	recordResponse();

	double agd = 0.0, agi = 0.0, agj = 0.0;
	for (auto i = 0; i < nsteps - 1; i++)
	{
		cstep += 1;
		ctime += dt*nsub;
		agd = (agX(i + 1) - agX(i)) / nsub;
		agi = agX(i);
		for (auto j = 0; j < nsub; j++)
		{
			agj = agi + agd*j;
			F.tail_rows(eqnCount) = solve(M, -Mp*EX*agj - q + Q0);
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
		acc = solve(M, -Mp*EX*agj - q - C*vel - K0*dsp + Q0);
	}
	saveResponse();
}

void DynamicSystem::solveSeismicResponseRK4(const int nsub)
{
	nsteps = Waves.at(XSeismicWaveId)->nsteps;
	dt = Waves.at(XSeismicWaveId)->dt;
	vec agX = XSeismicWaveScale*Waves.at(XSeismicWaveId)->series;

	dt = dt / nsub;
	Element::dt = dt;
}

void DynamicSystem::solveSeismicResponseNewmarkMD(const int nsub)
{
	nsteps = Waves.at(XSeismicWaveId)->nsteps;
	dt = Waves.at(XSeismicWaveId)->dt;
	vec agX = XSeismicWaveScale*Waves.at(XSeismicWaveId)->series;
	vec agY = zeros<vec>(nsteps);
	vec agZ = zeros<vec>(nsteps);

	if (YSeismicWaveId >= 0)
	{
		agY = YSeismicWaveScale*Waves.at(YSeismicWaveId)->series;
	}

	if (ZSeismicWaveId >= 0)
	{
		agZ = ZSeismicWaveScale*Waves.at(ZSeismicWaveId)->series;
	}

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

	a0 = solve(M, -Mp*EX*agX(0) - C*v0);

	dsp = u0;
	vel = v0;
	acc = a0;

	setDofResponse();
	getElementResponse();
	recordResponse();

	vec p_h;
	vec u_p;
	double agdX, agiX, agjX;
	double agdY, agiY, agjY;
	double agdZ, agiZ, agjZ;
	for (int i = 0; i < nsteps - 1; i++)
	{
		cstep += 1;
		ctime += dt*nsub;
		agdX = (agX(i + 1) - agX(i))/nsub;
		agiX = agX(i);
		agdY = (agY(i + 1) - agY(i))/nsub;
		agiY = agY(i);
		agdZ = (agZ(i + 1) - agZ(i))/nsub;
		agiZ = agZ(i);
		for (int j = 1; j < nsub + 1; j++)
		{
			agjX = agiX + agdX*j;
			agjY = agiY + agdY*j;
			agjZ = agiZ + agdZ*j;
			p_h = -Mp*(EX*agjX + EY*agjY + EZ*agjZ) + c7*u0 + c8*v0 + c9*a0 + Q0;
			u_p = u0*1.0;
			u0 = solve(K_h, p_h);
			v0 = c3*(u0 - u_p) + c4*v0 + dt*c5*a0;
			a0 = solve(M, -Mp*(EX*agjX + EY*agjY + EZ*agjZ) - C*v0 - K0*u0 + Q0);
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

void DynamicSystem::solveSeismicResponseNewmarkNLMD(const int nsub)
{
	nsteps = Waves.at(XSeismicWaveId)->nsteps;
	dt = Waves.at(XSeismicWaveId)->dt;
	vec agX = XSeismicWaveScale*Waves.at(XSeismicWaveId)->series;
	vec agY = zeros<vec>(nsteps);
	vec agZ = zeros<vec>(nsteps);

	if (YSeismicWaveId >= 0)
	{
		agY = YSeismicWaveScale*Waves.at(YSeismicWaveId)->series;
	}

	if (ZSeismicWaveId >= 0)
	{
		agZ = ZSeismicWaveScale*Waves.at(ZSeismicWaveId)->series;
	}

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

	a0 = solve(M, -Mp*EX*agX(0) - C*v0);

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
	double agdX, agiX, agjX;
	double agdY, agiY, agjY;
	double agdZ, agiZ, agjZ;
	double error = 1.0;
	for (int i = 0; i < nsteps - 1; i++)
	{
		cstep += 1;
		ctime += dt*nsub;
		agdX = (agX(i + 1) - agX(i))/nsub;
		agiX = agX(i);
		agdY = (agY(i + 1) - agY(i))/nsub;
		agiY = agY(i);
		agdZ = (agZ(i + 1) - agZ(i))/nsub;
		agiZ = agZ(i);
		for (int j = 1; j < nsub + 1; j++)
		{
			agjX = agiX + agdX*j;
			agjY = agiY + agdY*j;
			agjZ = agiZ + agdZ*j;
			p_h = -Mp*(EX*agjX + EY*agjY + EZ*agjZ) + c7*u0 + c8*v0 + c9*a0;
			u_p = u0*1.0;
			v_p = v0*1.0;
			a_p = a0*1.0;
			error = 1.0;
			for (int l = 0; l < maxIter; l++)
			{
				du = solve(K_h_ + K, -K_h*u0 - q + p_h + Q0);
				u0 = u0 + du;
				v0 = c3*(u0 - u_p) + c4*v_p + dt*c5*a_p;
				a0 = solve(M, -Mp*(EX*agjX + EY*agjY + EZ*agjZ) - q - C*v0 - K0*u0 + Q0);

				dsp = u0;
				vel = v0;
				acc = a0;

				setDofResponse();
				error = norm(du)/norm(u0);
				if (error > tol)
				{
					if (l < maxIter - 1)
					{
						assembleNonlinearForceVector(false);
						reassembleStiffnessMatrix();
					}
					else
					{
						cout << "Fail to Converge after " << maxIter << " iterations, "
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

void DynamicSystem::solveSeismicResponseStateSpaceMD(const int nsub)
{
	nsteps = Waves.at(XSeismicWaveId)->nsteps;
	dt = Waves.at(XSeismicWaveId)->dt;
	vec agX = XSeismicWaveScale*Waves.at(XSeismicWaveId)->series;
	vec agY = zeros<vec>(nsteps);
	vec agZ = zeros<vec>(nsteps);

	if (YSeismicWaveId >= 0)
	{
		agY = YSeismicWaveScale*Waves.at(YSeismicWaveId)->series;
	}

	if (ZSeismicWaveId >= 0)
	{
		agZ = ZSeismicWaveScale*Waves.at(ZSeismicWaveId)->series;
	}

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
	mat I_ = eye<mat>(eqnCount, eqnCount);
	mat A = -solve(M, K0);
	mat B = -solve(M, C);
	mat H = join_cols(join_rows(O, I_), join_rows(A, B));

	double h = dt;
	//mat T = expmat(H*h);
	mat T = expm(H, h);

	dsp = x0.head_rows(eqnCount);
	vel = x0.tail_rows(eqnCount);
	acc = solve(M, -Mp*EX*agX(0) - C*vel);

	setDofResponse();
	getElementResponse();
	recordResponse();

	double agdX, agiX, agjX=0.0;
	double agdY, agiY, agjY=0.0;
	double agdZ, agiZ, agjZ=0.0;
	for (int i = 0; i < nsteps - 1; i++)
	{
		cstep += 1;
		ctime += dt*nsub;
		agdX = (agX(i + 1) - agX(i))/nsub;
		agiX = agX(i);
		agdY = (agY(i + 1) - agY(i))/nsub;
		agiY = agY(i);
		agdZ = (agZ(i + 1) - agZ(i))/nsub;
		agiZ = agZ(i);
		for (int j = 0; j < nsub; j++)
		{
			agjX = agiX + agdX*j;
			agjY = agiY + agdY*j;
			agjZ = agiZ + agdZ*j;
			F.tail_rows(eqnCount) = solve(M, -Mp*(EX*agjX + EY*agjY + EZ*agjZ) + Q0);
			x0 = T*(x0 + F*h);
		}

		dsp = x0.head_rows(eqnCount);
		vel = x0.tail_rows(eqnCount);
		acc = solve(M, -Mp*(EX*agjX + EY*agjY + EZ*agjZ) - C*vel - K*dsp + Q0);

		setDofResponse();
		getElementResponse();
		recordResponse();
	}
	saveResponse();
}

void DynamicSystem::solveSeismicResponseStateSpaceNLMD(const int nsub)
{
	nsteps = Waves.at(XSeismicWaveId)->nsteps;
	dt = Waves.at(XSeismicWaveId)->dt;
	vec agX = XSeismicWaveScale*Waves.at(XSeismicWaveId)->series;
	vec agY = zeros<vec>(nsteps);
	vec agZ = zeros<vec>(nsteps);

	if (YSeismicWaveId >= 0)
	{
		agY = YSeismicWaveScale*Waves.at(YSeismicWaveId)->series;
	}

	if (ZSeismicWaveId >= 0)
	{
		agZ = ZSeismicWaveScale*Waves.at(ZSeismicWaveId)->series;
	}

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
	mat I_ = eye<mat>(eqnCount, eqnCount);
	mat A = -solve(M, K0);
	mat B = -solve(M, C);
	mat H = join_cols(join_rows(O, I_), join_rows(A, B));

	double h = dt;
	//mat T = expmat(H*h);
	mat T = expm(H, h);

	dsp = x0.head_rows(eqnCount);
	vel = x0.tail_rows(eqnCount);
	acc = solve(M, -Mp*EX*agX(0) - C*vel);

	setDofResponse();
	getElementResponse();
	assembleNonlinearForceVector(true);
	recordResponse();

	double agdX, agiX, agjX=0.0;
	double agdY, agiY, agjY=0.0;
	double agdZ, agiZ, agjZ=0.0;
	for (int i = 0; i < nsteps - 1; i++)
	{
		cstep += 1;
		ctime += dt*nsub;
		agdX = (agX(i + 1) - agX(i))/nsub;
		agiX = agX(i);
		agdY = (agY(i + 1) - agY(i))/nsub;
		agiY = agY(i);
		agdZ = (agZ(i + 1) - agZ(i))/nsub;
		agiZ = agZ(i);
		for (int j = 0; j < nsub; j++)
		{
			agjX = agiX + agdX*j;
			agjY = agiY + agdY*j;
			agjZ = agiZ + agdZ*j;
			F.tail_rows(eqnCount) = solve(M, -Mp*(EX*agjX + EY*agjY + EZ*agjZ) - q + Q0);
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
		acc = solve(M, -Mp*(EX*agjX + EY*agjY + EZ*agjZ) - q - C*vel - K0*dsp + Q0);
	}
	saveResponse();
}

void DynamicSystem::setDofResponse()
{
	for (int i = 0; i < eqnCount; i++)
	{
		DOF *d = DOFs.at(eqnMapDof.at(i));
		d->setResponse(dsp(i), vel(i), acc(i));
	}
}

void DynamicSystem::setDofStaticResponse()
{
	for (int i = 0; i < eqnCount; i++)
	{
		DOF *d = DOFs.at(eqnMapDof.at(i));
		d->setResponse(dsp(i));
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
		std::map<int, DOFRecorder *>::iterator it;
		for (it = DOFRecorders.begin(); it != DOFRecorders.end(); it++)
		{
			auto *dr = it->second;
			dr->init(nsteps);
		}
	}

	if (!(ElementRecorders.empty()))
	{
		std::map<int, ElementRecorder *>::iterator it;
		for (it = ElementRecorders.begin(); it != ElementRecorders.end(); it++)
		{
			auto *er = it->second;
			er->init(nsteps);
		}
	}

	if (exportGmshInterval > 0)
	{
		gmshFile.open(gmshFileName, ios::out|ios::app);
	}
}

void DynamicSystem::recordResponse()
{
	if (!(DOFRecorders.empty()))
	{
		std::map<int, DOFRecorder *>::iterator it;
		for (it = DOFRecorders.begin(); it != DOFRecorders.end(); it++)
		{
			auto *dr = it->second;
			dr->record(cstep, ctime);
		}
	}

	if (!(ElementRecorders.empty()))
	{
		std::map<int, ElementRecorder *>::iterator it;
		for (it = ElementRecorders.begin(); it != ElementRecorders.end(); it++)
		{
			auto *er = it->second;
			er->record(cstep, ctime);
		}
	}

	if (exportGmshInterval > 0)
	{
		if (cstep % exportGmshInterval == 0)
		{
			exportResponseGmsh();
		}
	}
}

void DynamicSystem::saveResponse()
{
	if (!(DOFRecorders.empty()))
	{
		std::map<int, DOFRecorder *>::iterator it;
		for (it = DOFRecorders.begin(); it != DOFRecorders.end(); it++)
		{
			auto *dr = it->second;
			dr->save();
		}
	}

	if (!(ElementRecorders.empty()))
	{
		std::map<int, ElementRecorder *>::iterator it;
		for (it = ElementRecorders.begin(); it != ElementRecorders.end(); it++)
		{
			auto *er = it->second;
			er->save();
		}
	}
	if (exportGmshInterval > 0)
	{
		gmshFile.close();
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

	cout << "Model Name: " << name << endl;
	cout << "Number of DOFs: " << DOFs.size() << endl;
	cout << "Number of Nodes: " << Nodes.size() << endl;
	cout << "Number of Elements: " << Elements.size() << endl;
	cout << "Number of Equations: " << eqnCount << endl;

	int nModes = (int)P.n_rows;
	if (nModes > 0)
	{
		cout << "Number of Modes:" << nModes << endl;
		for (int i = 0; i < nModes; i++)
		{
			cout << "Mode " << i + 1 << ", T = " << P(i) << endl;
		}
	}

	cout << "=================================================" << endl;
}

void DynamicSystem::setCurrentTime(double ctime)
{
	this->ctime = ctime;
}

void DynamicSystem::setName(const char *name)
{
	this->name = name;
}

void DynamicSystem::setWorkDir(const char *dir)
{
	workDir = dir;
}
