#pragma once

#ifdef __GNUC__
#define DLL_API
#include <cstddef>
#else
#ifdef DLLEXPORT
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif
#endif


extern "C" {

	DLL_API void clear();
	DLL_API void delete_model();
	DLL_API void set_damping_ratio(const double zeta);
	DLL_API void set_rayleigh_damping(const double omg1, const double omg2);
	DLL_API void set_rayleigh_damping_order(const int i, const int j);
	DLL_API void set_num_modes_inherent_damping(const int n);
	DLL_API void active_ground_motion(const int dir, const int waveId, const double waveScale);
	DLL_API void fix_dof(const int id);
	DLL_API void free_dof(const int id);
	DLL_API void fix_node(const int id);
	DLL_API void fix_node_dof(const int nodeId, const int dir);
	DLL_API size_t add_dof_x(const int id, const double m);
	DLL_API size_t add_dof(const int id, const int dir, const double m);
	DLL_API size_t add_node_1d(const int nodeId, const double x, const int dofId);
	DLL_API size_t add_point(const int pointId, const double x, const double y, const double z);
	DLL_API size_t add_line(const int lineId, const int i, const int j);
	DLL_API size_t add_node_2d(const int nodeId, const double x, const double z,
								const int dofXId, const int dofZId, const int dofRYId);
	DLL_API size_t add_node_3d(const int nodeId, const double x, const double y, const double z,
								const int dofXId,  const int dofYId,  const int dofZId,
								const int dofRXId, const int dofRYId, const int dofRZId);
	DLL_API size_t set_dof_mass(const int id, const double m);
	DLL_API size_t set_node_mass(const int id, const double m);
	DLL_API size_t set_dof_point_id(const int dof_id, const int point_id);
	DLL_API size_t set_dof_node_id(const int dof_id, const int node_id);

	DLL_API size_t add_spring(const int id, const int i, const int j, const double k);
	DLL_API size_t add_dashpot(const int id, const int i, const int j, const double c);
	DLL_API size_t add_dashpot_exp(const int id, const int i, const int j, const double c, const double alpha);
	DLL_API size_t add_dashpot_maxwell(const int id, const int i, const int j, const double k, const double c, const double alpha);
	DLL_API size_t add_spring_bilinear(const int id, const int i, const int j, const double k0, const double uy, const double alpha);
	DLL_API size_t add_spring_nonlinear(const int id, const int ni, const int nj, const int matId);
	DLL_API size_t add_inerter(const int id, const int i, const int j, const double m);
	DLL_API size_t add_tvmd(const int id, const int i, const int j, const double m, const double c, const double k);
	DLL_API size_t add_truss_elastic_2d(const int id, const int ni, const int nj, const double EA);
	DLL_API size_t add_truss_elastic_3d(const int id, const int ni, const int nj, const double EA);
	DLL_API size_t add_frame_elastic_2d(const int id, const int ni, const int nj, const double EA, const double EI);
	DLL_API size_t add_frame_elastic_3d(const int id, const int ni, const int nj, const double EA, const double EIy, const double EIz, const double GIp);
	DLL_API size_t add_quad4_elastic(const int id, const int nodeI, const int nodeJ,
										const int nodeP, const int nodeQ,
										 const double E, const double nu, const double t);
	DLL_API size_t add_rect_shell4_elastic(const int id, const int nodeI, const int nodeJ,
											const int nodeP, const int nodeQ,
											const double E, const double nu, const double t);

	DLL_API size_t add_spring_2d(const int id, const int ni, const int nj, const double k, const int localAxis);
	DLL_API size_t add_dashpot_2d(const int id, const int ni, const int nj, const double c, const int localAxis);
	DLL_API size_t add_dashpot_maxwell_2d(const int id, const int ni, const int nj, const double k, const double c, const double alpha, const int localAxis);
	DLL_API size_t add_spring_bilinear_2d(const int id, const int ni, const int nj, const double k0, const double uy, const double alpha, const int localAxis);
	DLL_API size_t add_spring_nonlinear_2d(const int id, const int ni, const int nj, const int matId, const int localAxis);
	DLL_API size_t add_inerter_2d(const int id, const int ni, const int nj, const double m, const int localAxis);
	DLL_API size_t add_tvmd_2d(const int id, const int ni, const int nj, const double m, const double c, const double k, const int localAxis);

	DLL_API size_t add_spring_3d(const int id, const int ni, const int nj, const double k, const int localAxis);
	DLL_API size_t add_dashpot_3d(const int id, const int ni, const int nj, const double c, const int localAxis);
	DLL_API size_t add_dashpot_maxwell_3d(const int id, const int ni, const int nj, const double k, const double c, const double alpha, const int localAxis);
	DLL_API size_t add_spring_bilinear_3d(const int id, const int ni, const int nj, const double k0, const double uy, const double alpha, const int localAxis);
	DLL_API size_t add_spring_nonlinear_3d(const int id, const int ni, const int nj, const int matId, const int localAxis);
	DLL_API size_t add_inerter_3d(const int id, const int ni, const int nj, const double m, const int localAxis);
	DLL_API size_t add_tvmd_3d(const int id, const int ni, const int nj, const double m, const double c, const double k, const int localAxis);

	DLL_API size_t add_spis2(const int id, const int i, const int j, const int in, const double m, const double c, const double k);

	DLL_API size_t add_load(const int id, double* t, double* p, const int nP, const double arriveTime,
		const double scale);
	DLL_API size_t add_dof_load(const int dofId, const int loadId);

	DLL_API size_t add_wave(const int id, const double dt, const char* fileName);
	DLL_API size_t assemble_matrix();
	DLL_API size_t solve_eigen();
	DLL_API size_t add_dof_recorder_with_dofs(const int id, int *dofIds, const int n, const int rType, char * fileName);
	DLL_API size_t add_dof_recorder(const int id, const int rType, char * fileName);
	DLL_API size_t add_dof_to_recorder(const int dofId, const int rId);
	DLL_API size_t add_node_dof_to_recorder(const int nodeId, const int dir, const int rId);
	DLL_API size_t add_ele_recorder_with_eles(const int id, int *eleIds, const int n, const int rType, char * fileName);
	DLL_API size_t add_ele_recorder(const int id, const int rType, char * fileName);
	DLL_API size_t add_ele_to_recorder(const int eleId, const int rId);
	DLL_API void set_dynamic_solver(const int s);
	DLL_API size_t solve_seismic_response(const int nsub = 1);
	DLL_API void print_info();
	DLL_API void export_gmsh(char * fileName);
	DLL_API void export_modal_gmsh(char * fileName, const int order);
	DLL_API void set_response_gmsh(char * fileName, const int interval);
	DLL_API void export_modal_matrix(const char * fileName);
	DLL_API void export_modal_matrix_auto_name();

	DLL_API size_t add_mat_sma_bilinear(const int id, const double E0, const double fy, const double alpha, const double sigma_shift);

	DLL_API double get_damping_ratio();
	DLL_API size_t get_num_dof();
	DLL_API size_t get_num_point();
	DLL_API size_t get_point_id(int *id);
	DLL_API size_t get_dof_id(int *id);
	DLL_API size_t get_ele_id(int *id);
	DLL_API size_t get_wave_id(int *id);
	DLL_API size_t get_dof_recorder_id(int *id);
	DLL_API size_t get_ele_recorder_id(int *id);
	DLL_API size_t get_num_line();
	DLL_API size_t get_num_ele();
	DLL_API size_t get_num_eqn();
	DLL_API size_t get_point_coord(float* pt, const bool norm=true);
	DLL_API size_t get_line_point_id(int* id);
	DLL_API size_t get_rod1d_dof_id(int* id);
	DLL_API void get_dof_modal_response(double *res, const int order=1);

	DLL_API size_t get_period(double* P);

	DLL_API void get_dof_info(const int id, int &dir, double &mass, bool &is_fixed);
	DLL_API void get_spring_info(const int id, int &i, int &j, double &k);
	DLL_API void get_dashpot_info(const int id, int &i, int &j, double &c);
	DLL_API void get_inerter_info(const int id, int &i, int &j, double &m);

	DLL_API bool get_use_rayleigh_damping();

	DLL_API size_t remove_dof(const int id);
	DLL_API size_t remove_spring(const int id);
	DLL_API size_t remove_dashpot(const int id);
	DLL_API size_t remove_inerter(const int id);
	DLL_API size_t remove_wave(const int id);
	DLL_API size_t remove_point(const int id);
	DLL_API size_t remove_line(const int id);

	DLL_API void record_all_dof_response(const int id);
	DLL_API void record_all_ele_response(const int id);

	DLL_API void set_name(const char * name);
	DLL_API void get_name(char * name, const size_t size);
	DLL_API void set_work_dir(const char * workDir);
	DLL_API void get_work_dir(char * workDir, const size_t size);

	DLL_API void load_from_json(const char * file_name);
	DLL_API void save_to_json(const char * file_name);
}