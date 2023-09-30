#ifndef __GNUC__
#define DLLEXPORT
#endif

#include "rods.h"
#include "DynamicSystem.h"

DynamicSystem* ds = new DynamicSystem();

DLL_API void clear() {
	delete ds;
	ds = new DynamicSystem();
}

DLL_API void delete_model() {
	delete ds;
}

DLL_API void set_damping_ratio(const double zeta) {
	ds->zeta = zeta;
}

DLL_API void active_ground_motion(const int dir, const int waveId, const double waveScale)
{
	ds->activeGroundMotion(RODS::Direction(dir), waveId, waveScale);
}

DLL_API void fix_dof(const int id) {
	ds->fixDOF(id);
}

DLL_API void free_dof(const int id)
{
    ds->freeDOF(id);
}

DLL_API size_t add_dof_x(const int id, const double m) {
	ds->addDOF(id, m);
	return ds->DOFs.size();
}

DLL_API size_t add_dof(const int id, const int dir, const double m) {
	ds->addDOF(id, RODS::Direction(dir), m);
	return ds->DOFs.size();
}

DLL_API size_t add_spring(const int id, const int i, const int j, const double k) {
	ds->addSpring(id, i, j, k);
	return ds->Springs.size();
}

DLL_API size_t add_dashpot(const int id, const int i, const int j, const double c) {
	ds->addDashpot(id, i, j, c);
	return ds->Dashpots.size();
}

DLL_API size_t add_wave(const int id, const double dt, const char* fileName) {
	ds->addWave(id, dt, fileName);
	return ds->Waves.size();
}

DLL_API size_t assemble_matrix() {
	ds->assembleMatrix();
	return ds->eqnCount;
}

DLL_API size_t solve_eigen()
{
	ds->solveEigen();
	return ds->eqnCount;
}

DLL_API size_t add_dof_recorder_with_dofs(const int id, int *dofIds, const int n, const int rType, char * fileName) {
	ds->addDOFRecorder(id, dofIds, n, RODS::Response(rType), fileName);
	return ds->DOFRecorders.size();
}

DLL_API size_t add_ele_recorder_with_eles(const int id, int *eleIds, const int n, const int rType, char * fileName) {
	ds->addElementRecorder(id, eleIds, n, RODS::Response(rType), fileName);
	return ds->ElementRecorders.size();
}

DLL_API void set_dynamic_solver(const int s) {
	ds->setDynamicSolver(RODS::DynamicSolver(s));
}

DLL_API size_t solve_seismic_response(const int nsub) {
	ds->solveSeismicResponse(nsub);
	return ds->cstep;
}

DLL_API void print_info()
{
	ds->printInfo();
}

DLL_API void export_gmsh(char* fileName)
{
	ds->exportGmsh(fileName);
}

DLL_API void fix_node(const int id)
{
	ds->fixNode(id);
}

DLL_API void fix_node_dof(const int nodeId, const int dir)
{
	ds->fixNode(nodeId, RODS::Direction(dir));
}

DLL_API size_t add_node_1d(const int nodeId, const double x, const int dofId)
{
	ds->addNode(nodeId, x, dofId);
	return ds->Nodes.size();
}

DLL_API size_t add_point(const int pointId, const double x, const double y, const double z)
{
	ds->addPoint(pointId, x, y, z);
	return ds->Points.size();
}

DLL_API size_t add_line(const int lineId, const int i, const int j)
{
	ds->addLine(lineId, i, j);
	return ds->Lines.size();
}

DLL_API size_t add_node_2d(const int nodeId, const double x, const double z, const int dofXId, const int dofZId,
	const int dofRYId)
{
	ds->addNode(nodeId, x, z, dofXId, dofZId, dofRYId);
	return ds->Nodes.size();
}

DLL_API size_t add_node_3d(const int nodeId, const double x, const double y, const double z, const int dofXId, const int dofYId,
	const int dofZId, const int dofRXId, const int dofRYId, const int dofRZId)
{
	ds->addNode(nodeId, x, y, z, dofXId,  dofYId,  dofZId, dofRXId, dofRYId, dofRZId);
	return ds->Nodes.size();
}

DLL_API size_t add_inerter(const int id, const int i, const int j, const double m)
{
	ds->addInerter(id, i, j, m);
	return ds->Inerters.size();
}

DLL_API size_t add_truss_elastic_2d(const int id, const int ni, const int nj, const double EA)
{
	ds->addTrussElastic2D(id, ni, nj ,EA);
	return ds->TrussElastic2Ds.size();
}

DLL_API size_t add_truss_elastic_3d(const int id, const int ni, const int nj, const double EA)
{
	ds->addTrussElastic3D(id, ni, nj ,EA);
	return ds->TrussElastic3Ds.size();
}

DLL_API size_t add_frame_elastic_2d(const int id, const int ni, const int nj, const double EA, const double EI)
{
	ds->addFrameElastic2D(id, ni, nj ,EA, EI);
	return ds->FrameElastic2Ds.size();
}

DLL_API size_t add_frame_elastic_3d(const int id, const int ni, const int nj, const double EA, const double EIy, const double EIz, const double GIp)
{
	ds->addFrameElastic3D(id, ni, nj ,EA, EIy, EIz, GIp);
	return ds->FrameElastic3Ds.size();
}

DLL_API size_t add_quad4_elastic(const int id, const int nodeI, const int nodeJ, const int nodeP, const int nodeQ,
	const double E, const double nu, const double t)
{
	ds->addQuad4Elastic(id, nodeI, nodeJ, nodeP, nodeQ, E, nu, t);
	return ds->Quad4Elastics.size();
}

DLL_API size_t add_rect_shell4_elastic(const int id, const int nodeI, const int nodeJ, const int nodeP, const int nodeQ,
	const double E, const double nu, const double t)
{
	ds->addRectShell4Elastic(id, nodeI, nodeJ, nodeP, nodeQ, E, nu, t);
	return ds->RectShell4Elastics.size();
}

DLL_API size_t add_spring_2d(const int id, const int ni, const int nj, const double k, const int localAxis)
{
	ds->addSpring2D(id, ni, nj, k, RODS::LocalAxis(localAxis));
	return ds->Spring2Ds.size();
}

DLL_API size_t add_dashpot_2d(const int id, const int ni, const int nj, const double c, const int localAxis)
{
	ds->addDashpot2D(id, ni, nj, c, RODS::LocalAxis(localAxis));
	return ds->Dashpot2Ds.size();
}

DLL_API size_t add_inerter_2d(const int id, const int ni, const int nj, const double m, const int localAxis)
{
	ds->addInerter2D(id, ni, nj, m, RODS::LocalAxis(localAxis));
	return ds->Inerter2Ds.size();
}

DLL_API size_t add_spring_3d(const int id, const int ni, const int nj, const double k, const int localAxis)
{
	ds->addSpring3D(id, ni, nj, k, RODS::LocalAxis(localAxis));
	return ds->Spring3Ds.size();
}

DLL_API size_t add_dashpot_3d(const int id, const int ni, const int nj, const double c, const int localAxis)
{
	ds->addDashpot3D(id, ni, nj, c, RODS::LocalAxis(localAxis));
	return ds->Dashpot3Ds.size();
}

DLL_API size_t add_inerter_3d(const int id, const int ni, const int nj, const double m, const int localAxis)
{
	ds->addInerter3D(id, ni, nj, m, RODS::LocalAxis(localAxis));
	return ds->Inerter3Ds.size();
}

DLL_API size_t add_dof_recorder(const int id, const int rType, char* fileName)
{
	ds->addDOFRecorder(id, RODS::Response(rType), fileName);
	return ds->DOFRecorders.size();
}

DLL_API size_t add_dof_to_recorder(const int dofId, const int rId)
{
	ds->addDOFToRecorder(dofId, rId);
	return ds->DOFRecorders.size();
}

DLL_API size_t add_ele_recorder(const int id, const int rType, char* fileName)
{
	ds->addElementRecorder(id, RODS::Response(rType), fileName);
	return ds->ElementRecorders.size(); 
}

DLL_API size_t add_ele_to_recorder(const int eleId, const int rId)
{
	ds->addElementToRecorder(eleId, rId);
	return ds->ElementRecorders.size();
}

DLL_API size_t add_dashpot_exp(const int id, const int i, const int j, const double c, const double alpha)
{
	ds->addDashpotExp(id, i, j, c, alpha);
	return ds->DashpotExps.size();
}

DLL_API size_t add_spring_bilinear(const int id, const int i, const int j, const double k0, const double uy,
	const double alpha)
{
	ds->addSpringBilinear(id, i, j, k0, uy, alpha);
	return ds->SpringBilinears.size();
}

DLL_API size_t add_spis2(const int id, const int i, const int j, const int in, const double m, const double c, const double k)
{
	ds->addSPIS2(id, i, j, in, m, c, k);
	return ds->SPIS2s.size();
}

DLL_API size_t add_tvmd_2d(const int id, const int ni, const int nj, const double m, const double c, const double k,
	const int localAxis)
{
	ds->addTVMD2D(id, ni, nj, m, c, k, RODS::LocalAxis(localAxis));
	return ds->TVMD2Ds.size();
}

DLL_API size_t add_tvmd_3d(const int id, const int ni, const int nj, const double m, const double c, const double k,
	const int localAxis)
{
	ds->addTVMD3D(id, ni, nj, m, c, k, RODS::LocalAxis(localAxis));
	return ds->TVMD3Ds.size();
}

DLL_API size_t add_dashpot_maxwell(const int id, const int i, const int j, const double k, const double c, const double alpha)
{
	ds->addDashpotMaxwell(id, i, j, k, c, alpha);
	return ds->DashpotMaxwells.size();
}

DLL_API size_t add_dashpot_maxwell_2d(const int id, const int ni, const int nj, const double k, const double c, const double alpha,
	const int localAxis)
{
	ds->addDashpotMaxwell2D(id, ni, nj, k, c, alpha, RODS::LocalAxis(localAxis));
	return ds->DashpotMaxwell2Ds.size();
}

DLL_API size_t add_spring_bilinear_2d(const int id, const int ni, const int nj, const double k0, const double uy,
	const double alpha, const int localAxis)
{
	ds->addSpringBilinear2D(id, ni, nj, k0, uy, alpha, RODS::LocalAxis(localAxis));
	return ds->SpringBilinear2Ds.size();
}

DLL_API size_t add_dashpot_maxwell_3d(const int id, const int ni, const int nj, const double k, const double c,
	const double alpha, const int localAxis)
{
	ds->addDashpotMaxwell3D(id, ni, nj, k, c, alpha, RODS::LocalAxis(localAxis));
	return ds->DashpotMaxwell3Ds.size();
}

DLL_API size_t add_spring_bilinear_3d(const int id, const int ni, const int nj, const double k0, const double uy,
	const double alpha, const int localAxis)
{
	ds->addSpringBilinear3D(id, ni, nj, k0, uy, alpha, RODS::LocalAxis(localAxis));
	return ds->SpringBilinear3Ds.size();
}

DLL_API size_t add_tvmd(const int id, const int i, const int j, const double m, const double c, const double k)
{
	ds->addTVMD(id, i, j, m, c, k);
	return ds->TVMDs.size();
}

DLL_API void set_rayleigh_damping(const double omg1, const double omg2)
{
	ds->setRayleighDamping(omg1, omg2);
}

DLL_API void set_rayleigh_damping_order(const int i, const int j)
{
	ds->setRayleighDamping(i, j);
}

DLL_API void set_num_modes_inherent_damping(const int n)
{
	ds->setNumModesInherentDamping(n);
}

DLL_API void export_modal_gmsh(char* fileName, const int order)
{
	ds->exportModalGmsh(fileName, order);
}

DLL_API void set_response_gmsh(char* fileName, const int interval)
{
	ds->setResponseGmsh(fileName, interval);
}

DLL_API void export_modal_matrix(const char * fileName)
{
	ds->exportModalMatrix(fileName);
}

DLL_API void export_modal_matrix_auto_name()
{
	ds->exportModalMatrix((ds->workDir + "/" + ds->name + "_modal_matrix.txt").c_str());
}

DLL_API size_t add_mat_sma_bilinear(const int id, const double E0, const double fy, const double alpha,
	const double sigma_shift)
{
	ds->addMaterialSMABilinear(id, E0, fy, alpha, sigma_shift);
	return ds->Material1Ds.size();
}

DLL_API size_t add_spring_nonlinear_2d(const int id, const int ni, const int nj, const int matId, const int localAxis)
{
	ds->addSpringNonlinear2D(id, ni, nj, matId, RODS::LocalAxis(localAxis));
	return ds->SpringNonlinear2Ds.size();
}

DLL_API size_t add_spring_nonlinear_3d(const int id, const int ni, const int nj, const int matId, const int localAxis)
{
	ds->addSpringNonlinear3D(id, ni, nj, matId, RODS::LocalAxis(localAxis));
	return ds->SpringNonlinear3Ds.size();
}

DLL_API size_t add_spring_nonlinear(const int id, const int ni, const int nj, const int matId)
{
	ds->addSpringNonlinear(id, ni, nj, matId);
	return ds->SpringNonlinears.size();
}

DLL_API size_t set_dof_mass(const int id, const double m)
{
	ds->DOFs.at(id)->setMass(m);
	return ds->DOFs.size();
}

DLL_API size_t set_node_mass(const int id, const double m)
{
	ds->Nodes.at(id)->setMass(m);
	return ds->Nodes.size();
}

DLL_API size_t set_dof_point_id(const int dof_id, const int point_id)
{
    ds->DOFs.at(dof_id)->setPointId(point_id);
	return ds->DOFs.size();
}

DLL_API size_t set_dof_node_id(const int dof_id, const int node_id)
{
    ds->DOFs.at(dof_id)->setNodeId(node_id);
	return ds->Nodes.size();
}

DLL_API size_t add_node_dof_to_recorder(const int nodeId, const int dir, const int rId)
{
	int dofId = -1;

	switch (RODS::Direction(dir))
	{
	case RODS::Direction::X:
		dofId = ds->Nodes.at(nodeId)->dofX->id;
		break;
	case RODS::Direction::Y:
		dofId = ds->Nodes.at(nodeId)->dofY->id;
		break;
	case RODS::Direction::Z:
		dofId = ds->Nodes.at(nodeId)->dofZ->id;
		break;
	case RODS::Direction::RX:
		dofId = ds->Nodes.at(nodeId)->dofRX->id;
		break;
	case RODS::Direction::RY:
		dofId = ds->Nodes.at(nodeId)->dofRY->id;
		break;
	case RODS::Direction::RZ:
		dofId = ds->Nodes.at(nodeId)->dofRZ->id;
		break;
	default:
		break;
	}
	ds->addDOFToRecorder(dofId, rId);
	return ds->DOFRecorders.size();
}

DLL_API size_t add_load(const int id, double* t, double* p, const int nP, const double arriveTime, const double scale)
{
	ds->addLoad(id, t, p, nP, arriveTime, scale);
	return ds->Loads.size();
}

DLL_API size_t add_dof_load(const int dofId, const int loadId)
{
	ds->addDofLoad(dofId, loadId);
	return ds->Loads.size();
}

DLL_API double get_damping_ratio()
{
	return ds->zeta;
}

DLL_API size_t get_num_dof()
{
	return ds->DOFs.size();
}

DLL_API size_t get_num_point()
{
    return ds->Points.size();
}

DLL_API size_t get_point_id(int *id)
{
    auto np = ds->Points.size();
	size_t i = 0;
	if (np>0)
	{
		for (auto it = ds->Points.begin(); it != ds->Points.end(); it++)
		{
			id[i] = it->second->id;
			i++;
		}
	}
	return np;
}

DLL_API size_t get_dof_id(int *id)
{
    auto nd = ds->DOFs.size();
	size_t i = 0;
	if (nd>0)
	{
		for (auto it = ds->DOFs.begin(); it != ds->DOFs.end(); it++)
		{
			id[i] = it->second->id;
			i++;
		}
	}
	return nd;
}

DLL_API size_t get_ele_id(int * id)
{
	auto ne = ds->Elements.size();
	size_t i = 0;
	if (ne>0)
	{
		for (auto it = ds->Elements.begin(); it != ds->Elements.end(); it++)
		{
			id[i] = it->second->id;
			i++;
		}
	}
	return ne;
}

DLL_API size_t get_wave_id(int * id)
{
	auto nw = ds->Waves.size();
	size_t i = 0;
	if (nw>0)
	{
		for (auto it = ds->Waves.begin(); it != ds->Waves.end(); it++)
		{
			id[i] = it->second->id;
			i++;
		}
	}
	return nw;
}

DLL_API size_t get_dof_recorder_id(int * id)
{
	auto n = ds->DOFRecorders.size();
	size_t i = 0;
	if (n>0)
	{
		for (auto it = ds->DOFRecorders.begin(); it != ds->DOFRecorders.end(); it++)
		{
			id[i] = it->second->id;
			i++;
		}
	}
	return n;
}

DLL_API size_t get_ele_recorder_id(int * id)
{
	auto n = ds->ElementRecorders.size();
	size_t i = 0;
	if (n>0)
	{
		for (auto it = ds->ElementRecorders.begin(); it != ds->ElementRecorders.end(); it++)
		{
			id[i] = it->second->id;
			i++;
		}
	}
	return n;
}

DLL_API int get_next_dof_id()
{
	return ds->getNextDofId();
}

DLL_API size_t get_num_line()
{
	return ds->Lines.size();
}

DLL_API size_t get_num_ele()
{
	return ds->Elements.size();
}

DLL_API size_t get_num_spring()
{
    return ds->Springs.size();
}

DLL_API size_t get_num_dashpot()
{
    return ds->Dashpots.size();
}

DLL_API size_t get_num_inerter()
{
    return ds->Inerters.size();
}

DLL_API size_t get_num_wave()
{
    return ds->Waves.size();
}

DLL_API size_t get_num_dof_recorder()
{
    return ds->DOFRecorders.size();
}

DLL_API size_t get_num_ele_recorder()
{
    return ds->ElementRecorders.size();
}

DLL_API size_t get_num_eqn()
{
	return ds->eqnCount;
}

DLL_API size_t get_point_coord(float *pt, const bool norm)
{
    auto np = ds->Points.size();
	size_t i = 0;
	if (np>0)
	{
		for (auto it = ds->Points.begin(); it != ds->Points.end(); it++)
		{
			auto x = it->second->x;
			auto y = it->second->y;
			auto z = it->second->z;
			if (norm) {
				if (ds->xMax > 0.0 || ds->xMin < 0.0) {
					x /= ds->xMax > -ds->xMin ? ds->xMax : -ds->xMin ;
					x *= 0.9;
				}
				if (ds->yMax > 0.0 || ds->yMin < 0.0) {
					y /= ds->yMax > -ds->yMin ? ds->yMax : -ds->yMin ;
					y *= 0.9;
				}
				if (ds->zMax > 0.0 || ds->zMin < 0.0) {
					z /= ds->zMax > -ds->zMin ? ds->zMax : -ds->zMin ;
					z *= 0.9;
				}
			}
			pt[3*i] = (float)x;
			pt[3*i+1] = (float)y;
			pt[3*i+2] = (float)z;
			i++;
		}
	}
	return np*3;
}

DLL_API size_t get_line_point_id(int* id)
{
	auto nl = ds->Lines.size();
	size_t i = 0;
	if (nl > 0)
	{
		for (auto it = ds->Lines.begin(); it != ds->Lines.end(); it++)
		{
			id[i*2] = it->second->pointI->id;
			id[i*2+1] = it->second->pointJ->id;
			i++;
		}
	}
	return nl*2;
}

DLL_API size_t get_rod1d_dof_id(int * id)
{
	auto n = ds->ROD1Ds.size();
	size_t i = 0;
	if (n > 0)
	{
		for (auto it = ds->ROD1Ds.begin(); it != ds->ROD1Ds.end(); it++)
		{
			id[i*2] = it->second->dofI->id;
			id[i*2+1] = it->second->dofJ->id;
			i++;
		}
	}
	return n*2;
}

DLL_API void get_dof_modal_response(double * res, const int order)
{
	ds->getDofModalResponse(res, order);
}

DLL_API size_t get_period(double* P)
{
	for (auto i = 0; i < ds->eqnCount; i++)
	{
		P[i] = ds->P(i);
	}
	return ds->eqnCount;
}

DLL_API void get_dof_info(const int id, int & dir, double & mass, bool & is_fixed)
{
	auto dof = ds->DOFs.at(id);
	dir = (int)dof->dir;
	mass = dof->mass;
	is_fixed = dof->isFixed;
}

DLL_API void get_spring_info(const int id, int & i, int & j, double & k)
{
	auto ele = ds->Springs.at(id);
	i = ele->dofI->id;
	j = ele->dofJ->id;
	k = ele->k;
}

DLL_API void get_dashpot_info(const int id, int & i, int & j, double & c)
{
	auto ele = ds->Dashpots.at(id);
	i = ele->dofI->id;
	j = ele->dofJ->id;
	c = ele->c;
}

DLL_API void get_inerter_info(const int id, int & i, int & j, double & m)
{
	auto ele = ds->Inerters.at(id);
	i = ele->dofI->id;
	j = ele->dofJ->id;
	m = ele->m;
}

DLL_API bool get_use_rayleigh_damping()
{
    return ds->useRayleighDamping;
}

DLL_API size_t remove_dof(const int id)
{
    ds->removeDOF(id);
	return ds->DOFs.size();
}

DLL_API size_t remove_spring(const int id)
{
    ds->removeSpring(id);
	return ds->Springs.size();
}

DLL_API size_t remove_dashpot(const int id)
{
    ds->removeDashpot(id);
	return ds->Dashpots.size();
}

DLL_API size_t remove_inerter(const int id)
{
    ds->removeInerter(id);
	return ds->Inerters.size();
}

DLL_API size_t remove_wave(const int id)
{
    ds->Waves.erase(id);
	return ds->Waves.size();
}

DLL_API size_t remove_point(const int id)
{
    ds->Points.erase(id);
	return ds->Points.size();
}

DLL_API size_t remove_line(const int id)
{
    ds->Lines.erase(id);
	return ds->Lines.size();
}

DLL_API void record_all_dof_response(const int id)
{
	ds->recordAllDofResponse(id);
}

DLL_API void record_all_ele_response(const int id)
{
	ds->recordAllEleResponse(id);
}

DLL_API void set_name(const char * name)
{
	ds->setName(name);
}

DLL_API void get_name(char * name, const size_t size)
{
#ifdef __GNUC__
	strcpy(name, ds->name.c_str());
#else
	strcpy_s(name, size, ds->name.c_str());
#endif
}

DLL_API void set_work_dir(const char * workDir)
{
	ds->setWorkDir(workDir);
}

DLL_API void get_work_dir(char * workDir, const size_t size)
{
#ifdef __GNUC__
	strcpy(workDir, ds->workDir.c_str());
#else
	strcpy_s(workDir, size, ds->workDir.c_str());
#endif
}

DLL_API void load_from_json(const char *file_name)
{
    ds->loadFromJSON(file_name);
}

DLL_API void save_to_json(const char *file_name)
{
	ds->saveToJSON(file_name);
}