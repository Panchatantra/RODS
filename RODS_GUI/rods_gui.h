#pragma once

#include <GLFW/glfw3.h>

static bool show_damping_window = false;
static bool show_dof_window = false;
static bool show_point_window = false;
static bool show_line_window = false;
static bool show_basic_info_window = false;
static bool show_wave_window = false;
static bool show_element1d_window = false;

static bool use_rayleigh_damping = true;
static bool use_mode_orthogonal_damping = false;

static int num_dof = 0;
static int num_point = 0;
static int num_line = 0;
static int num_ele = 0;

static int point_index = 0;

namespace RODS_GUI {
    void createShader(unsigned int &shaderProgram);
    void buildVertex(unsigned int &VBO, unsigned int &VAO, unsigned int &EBO);
    void mainMenu(GLFWwindow* window);
    void dampingWindow();
    void dofWindow();
    void pointWindow();
    void basicInfoWindow();
    void lineWindow();
    void waveWindow();
    void element1dWindow();

    void draw(unsigned int VBO, unsigned int VAO, unsigned int EBO);
    void updateDOFList();
}