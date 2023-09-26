#pragma once

#include <GLFW/glfw3.h>

static bool show_basic_info_window = true;
static bool show_damping_window = false;
static bool show_dof_window = false;
static bool show_point_window = false;
static bool show_line_window = false;
static bool show_wave_window = false;
static bool show_element1d_window = false;
static bool show_assemble_matrix_window = false;
static bool show_solve_eigen_window = false;
static bool show_solve_seismic_window = false;
static bool show_dof_table_window = false;
static bool show_element1d_table_window = false;
static bool show_recorder_window = false;
static bool show_time_history_plot_window = false;
static bool show_dir_window = false;
static bool show_draw_mode_window = false;

static bool use_rayleigh_damping = true;
static bool use_mode_orthogonal_damping = false;

static int num_dof = 0;
static int num_point = 0;
static int num_line = 0;
static int num_ele = 0;
static int num_spring = 0;
static int num_dashpot = 0;
static int num_inerter = 0;
static int num_eqn = 0;
static int num_wave = 0;
static int num_dof_recorder = 0;
static int num_ele_recorder = 0;

static int point_index = 0;
static int dof_index = 0;

static int draw_dim = 1;
static int draw_type = 1;

static int mode_order = 1;

const size_t C_STR_LEN = 100;
const size_t C_STR_LEN_S = 20;

namespace RODS_GUI {
    void createShader(unsigned int &shaderProgram);
    void buildVertex(unsigned int &VBO, unsigned int &VAO, unsigned int &EBO);
    void mainMenu(GLFWwindow* window);
    void dirWindow();
    void dampingWindow();
    void dofWindow();
    void pointWindow();
    void basicInfoWindow();
    void lineWindow();
    void waveWindow();
    void element1dWindow();
    void assembleMatrixWindow();
    void solveEigenWindow();
    void recorderWindow();
    void solveSeismicWindow();

    void dofTableWindow();
    void element1dTableWindow();

    void timeHistoryPlotWindow();

    void drawModeWindow();

    void draw_geo(unsigned int VBO, unsigned int VAO, unsigned int EBO);
    void draw_1d(unsigned int VBO, unsigned int VAO, unsigned int EBO);
    void updateDOFList();
    void updateDOFRecorderList();
    void updateEleRecorderList();
    void updateWaveList();
    void updateEleList();
    void updatedofIdMapIndex();

    void pause(int dur);
    void sdof_model();

    enum class DrawType
	{
		Model = 1,
		Mode_Static,
		Mode_Animate,
		Time_History
	};
}