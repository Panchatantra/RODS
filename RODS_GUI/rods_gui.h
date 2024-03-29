#pragma once

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static bool show_basic_info_window = true;
static bool show_damping_window = false;
static bool show_dof_window = false;
static bool show_point_window = false;
static bool show_line_window = false;
static bool show_wave_window = false;
static bool show_material_window = false;
static bool show_section_window = false;
static bool show_element1d_window = false;
static bool show_element2d_window = false;
static bool show_element3d_window = false;
static bool show_rigid_diagram_window = false;
static bool show_assemble_matrix_window = false;
static bool show_solve_eigen_window = false;
static bool show_solve_seismic_window = false;
static bool show_dof_table_window = false;
static bool show_node_table_window = false;
static bool show_element1d_table_window = false;
static bool show_element2d_table_window = false;
static bool show_wave_table_window = false;
static bool show_recorder_window = false;
static bool show_dof_recorder_window = false;
static bool show_ele_recorder_window = false;
static bool show_recorder_table_window = false;
static bool show_time_history_plot_window = false;
static bool show_dir_window = false;
static bool show_draw_mode_window = false;
static bool show_node_window = false;
static bool show_menu_window = true;
static bool show_main_menu = false;
static bool display_node_id = false;

// static bool use_rayleigh_damping = true;
// static bool use_mode_orthogonal_damping = false;

static int num_dof = 0;
static int num_point = 0;
static int num_line = 0;
static int num_ele = 0;
static int num_rod_1d = 0;
static int num_rod_2d = 0;
static int num_rod_3d = 0;
static int num_spring = 0;
static int num_dashpot = 0;
static int num_inerter = 0;
static int num_tvmd = 0;
static int num_spring_bilinear = 0;
static int num_truss_elastic_2d = 0;
static int num_frame_elastic_2d = 0;
static int num_spring_2d = 0;
static int num_dashpot_2d = 0;
static int num_inerter_2d = 0;
static int num_truss_elastic_3d = 0;
static int num_frame_elastic_3d = 0;
static int num_spring_3d = 0;
static int num_dashpot_3d = 0;
static int num_inerter_3d = 0;

static int num_mat_1d = 0;
static int num_eqn = 0;
static int num_wave = 0;
static int num_dof_recorder = 0;
static int num_ele_recorder = 0;
static int num_rigid_diagram = 0;

static int num_node = 0;
static int num_mat = 0;

static int num_dof_x = 0;
static int num_dof_y = 0;
static int num_dof_z = 0;
static int num_dof_rx = 0;
static int num_dof_ry = 0;
static int num_dof_rz = 0;

static int point_index = 0;
static int dof_index = 0;

static int draw_dim = 2;
static int draw_type = 1;

static int mode_order = 1;

const size_t C_STR_LEN = 100;
const size_t C_STR_LEN_S = 20;

static unsigned int shaderProgram, textShaderProgram;
static unsigned int VBO, VAO, EBO;
static unsigned int VBO_COLOR;
static unsigned int VAO_TEXT;
static unsigned int VBO_TEXT;

static int buffer_width, buffer_height;
static float scale_factor_dsp = 1.0f;

struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

namespace RODS_GUI {
    void createShader();
    void createTextShader();
    void buildVertex();
    void buildTextVertex();
    void readConfig();
    void saveConfig();
    void setCamera(GLFWwindow* window);
    void mainMenu(GLFWwindow* window);
    void menuWindow(GLFWwindow* window);
    void dirWindow();
    void dampingWindow();
    void dofWindow();
    void pointWindow();
    void basicInfoWindow();
    void lineWindow();
    void materialWindow();
    void waveWindow();
    void element1dWindow();
    void element2dWindow();
    void element3dWindow();
    void nodeWindow();
    void rigidDiagramWindow();
    void assembleMatrixWindow();
    void solveEigenWindow();
    void recorderWindow();
    void dofRecorderWindow();
    void eleRecorderWindow();
    void solveSeismicWindow();

    void dofTableWindow();
    void nodeTableWindow();
    void element1dTableWindow();
    void element2dTableWindow();
    void element3dTableWindow();

    void recorderTableWindow();
    void waveTableWindow();

    void timeHistoryPlotWindow();

    void drawModeWindow(GLFWwindow* window);

    void updateViewMatrix(glm::vec3 v);
    void updateProjectionMatrix(GLFWwindow* window);
    void setTextProjectionMatrix();
    void updateModelMatrix();
    void autoFitScreen();
    void captureScreen(const char* filePath, GLFWwindow* window);
    void draw_geo();
    void draw_1d_s();
    void draw_2d();
    void draw_3d();
    void draw_text();

    void getWindowBufferSize(GLFWwindow* window);
    void mapNormDevCoord2ScreenCoord(float &x, float &y);
    void render_text(const std::string text,
                    const float loc_x, const float loc_y,
                    const float scale);

    void drawNodeIds(GLFWwindow* window);

    void updateDOFList();
    void genDofList();
    void genPointList();
    void genNodeList();
    void updateDOFRecorderList();
    void genDofRecorderList();
    void updateEleRecorderList();
    void genEleRecorderList();
    void updateWaveList();
    void genWaveList();
    void updateEleList();
    void updatedofIdMapIndex();
    void updateNodeIdMapIndex();

    void glDeleteAll();

    void initVars();
    void updateVars();

    enum class DrawType
	{
		Model = 1,
		Mode_Static,
		Mode_Animate,
		Time_History
	};
}