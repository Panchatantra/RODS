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

DLL_API void active_ground_motion(const int dir)
{
	ds->activeGroundMotion(Direction(dir));
}

DLL_API void fix_dof(const int id) {
	ds->fixDof(id);
}

DLL_API size_t add_dof(const int id, const double m) {
	ds->addDof(id, m);
	return ds->DOFs.size();
}

DLL_API size_t add_dof_fixed(const int id, const double m) {
	ds->addDof(id, m, FIXED);
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

DLL_API size_t add_timeseries(const int id, const double dt, char* fileName) {
	ds->addTimeSeries(id, dt, fileName);
	return ds->tss.size();
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

DLL_API size_t add_dof_recorder(const int id, int *dofIds, const int n, const int rtype, char * fileName) {
	ds->addDofRecorder(id, dofIds, n, response(rtype), fileName);
	return ds->drs.size();
}

DLL_API size_t add_ele_recorder(const int id, int *eleIds, const int n, const int rtype, char * fileName) {
	ds->addElementRecorder(id, eleIds, n, response(rtype), fileName);
	return ds->ers.size();
}

DLL_API size_t set_dynamic_solver(const int s) {
	ds->setDynamicSolver(dsolver(s));
	return ds->dynamicSolver;
}

DLL_API size_t solve_seismic_response(const int tsId, const double s, const int nsub) {
	ds->solveTimeDomainSeismicResponse(tsId, s, nsub);
	return ds->cstep;
}

DLL_API void print_info()
{
	ds->printInfo();
}
