#ifndef __GNUC__
#define DLLEXPORT
#endif

#include "DynamicSystem.h"
#include "RODS.h"

DynamicSystem *ds = new DynamicSystem();

DLL_API void clear() {
	delete ds;
	ds = new DynamicSystem();
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

DLL_API size_t add_wave(const int id, const double dt, char* fileName) {
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

DLL_API size_t add_dof_recorder(const int id, int *dofIds, const int n, const int rType, char * fileName) {
	ds->addDOFRecorder(id, dofIds, n, RODS::Response(rType), fileName);
	return ds->DOFRecorders.size();
}

DLL_API size_t add_ele_recorder(const int id, int *eleIds, const int n, const int rType, char * fileName) {
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

void fix_node(const int id)
{
	ds->fixNode(id);
}

void fix_node_dof(const int nodeId, const int dir)
{
	ds->fixNode(nodeId, RODS::Direction(dir));
}

size_t add_node_1d(const int nodeId, const double x, const int dofId)
{
	ds->addNode(nodeId, x, dofId);
	return ds->Nodes.size();
}

size_t add_node_2d(const int nodeId, const double x, const double z, const int dofXId, const int dofZId,
	const int dofRYId)
{
	ds->addNode(nodeId, x, z, dofXId, dofZId, dofRYId);
	return ds->Nodes.size();
}

size_t add_node_3d(const int nodeId, const double x, const double y, const double z, const int dofXId, const int dofYId,
	const int dofZId, const int dofRXId, const int dofRYId, const int dofRZId)
{
	ds->addNode(nodeId, x, y, z, dofXId,  dofYId,  dofZId, dofRXId, dofRYId, dofRZId);
	return ds->Nodes.size();
}

size_t add_inerter(const int id, const int i, const int j, const double m)
{
	ds->addInerter(id, i, j, m);
	return ds->Inerters.size();
}

DLL_API size_t add_truss_elastic_2d(const int id, const int ni, const int nj, const double EA)
{
	ds->addTrussElastic2D(id, ni, nj ,EA);
	return ds->TrussElastic2Ds.size();
}

DLL_API size_t add_frame_elastic_2d(const int id, const int ni, const int nj, const double EA, const double EI)
{
	ds->addFrameElastic2D(id, ni, nj ,EA, EI);
	return ds->FrameElastic2Ds.size();
}

size_t add_quad4_elastic(const int id, const int nodeI, const int nodeJ, const int nodeP, const int nodeQ,
	const double E, const double nu, const double t)
{
	ds->addQuad4Elastic(id, nodeI, nodeJ, nodeP, nodeQ, E, nu, t);
	return ds->Quad4Elastics.size();
}

size_t add_spring_2d(const int id, const int ni, const int nj, const double k, const int localAxis)
{
	ds->addSpring2D(id, ni, nj, k, RODS::LocalAxis(localAxis));
	return ds->Spring2Ds.size();
}

size_t add_dashpot_2d(const int id, const int ni, const int nj, const double c, const int localAxis)
{
	ds->addDashpot2D(id, ni, nj, c, RODS::LocalAxis(localAxis));
	return ds->Dashpot2Ds.size();
}

size_t add_inerter_2d(const int id, const int ni, const int nj, const double m, const int localAxis)
{
	ds->addInerter2D(id, ni, nj, m, RODS::LocalAxis(localAxis));
	return ds->Inerter2Ds.size();
}
