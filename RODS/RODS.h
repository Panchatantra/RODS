#pragma once

#ifdef __GNUC__
#define DLL_API
#else
#ifdef DLLEXPORT
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif
#endif

namespace RODS {
	constexpr int X = 0;
	constexpr int Y = 1;
	constexpr int Z = 2;
	constexpr int RX = 3;
	constexpr int RY = 4;
	constexpr int RZ = 5;
	constexpr int DISP = 0;
	constexpr int VEL = 1;
	constexpr int ACC = 2;
	constexpr int FORCE = 3;
	constexpr int DEFORM = 4;
	constexpr int ALL = 5;
}

extern "C" {
	DLL_API void clear();
	DLL_API void set_damping_ratio(const double z);
	DLL_API void active_ground_motion(const int dir);
	DLL_API void fix_dof(const int id);
	DLL_API size_t add_dof(const int id, const double m);
	DLL_API size_t add_dof_fixed(const int id, const double m);
	DLL_API size_t add_spring(const int id, const int i, const int j, const double k);
	DLL_API size_t add_dashpot(const int id, const int i, const int j, const double c);
	DLL_API size_t add_timeseries(const int id, const double dt, char* fileName);
	DLL_API size_t assemble_matrix();
	DLL_API size_t solve_eigen();
	DLL_API size_t add_dof_recorder(const int id, int *dofIds, const int n, const int rtype, char * fileName);
	DLL_API size_t add_ele_recorder(const int id, int *eleIds, const int n, const int rtype, char * fileName);
	DLL_API size_t set_dynamic_solver(const int s);
	DLL_API size_t solve_seismic_response(const int tsId, const double s = 1.0, const int nsub = 1);
	DLL_API void print_info();
}
