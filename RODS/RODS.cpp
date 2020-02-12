#ifndef __GNUC__
#define DLLEXPORT
#endif

#include "dsystem.h"
#include "RODS.h"

dsystem *ds = new dsystem();

DLL_API void clear() {
	delete ds;
	ds = new dsystem();
}

DLL_API void set_damping_ratio(const double z) {
	ds->zeta = z;
}

DLL_API void active_ground_motion(const int dir)
{
	switch (dir)
	{
	case 1:
		ds->activeGroundMotion(X);
		break;
	case 2:
		ds->activeGroundMotion(Y);
		break;
	case 3:
		ds->activeGroundMotion(Z);
		break;
	default:
		break;
	}
}

DLL_API void fix_dof(const int id) {
	ds->fixDof(id);
}

DLL_API size_t add_dof(const int id, const double m) {
	ds->addDof(id, m);
	return ds->dofs.size();
}

DLL_API size_t add_dof_fixed(const int id, const double m) {
	ds->addDof(id, m, FIXED);
	return ds->dofs.size();
}

DLL_API size_t add_spring(const int id, const int i, const int j, const double k) {
	ds->addSpring(id, i, j, k);
	return ds->springs.size();
}

DLL_API size_t add_dashpot(const int id, const int i, const int j, const double c) {
	ds->addDashpot(id, i, j, c);
	return ds->dashpots.size();
}

DLL_API size_t add_timeseries(const int id, const double dt, char* fileName) {
	ds->addTimeseries(id, dt, fileName);
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
