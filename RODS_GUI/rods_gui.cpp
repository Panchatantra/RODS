#include <stdio.h>

#include "imgui.h"
#include "implot.h"
#include "ImGuiFileDialog.h"

#include "rods_gui.h"
#include "rods.h"

#include <string>
#include <iostream>
#include <sstream>
#include <math.h>
#include <algorithm>
#include <map>
#include <vector>
std::map<int, int> pointIdMapIndex;
std::map<int, int> dofIdMapIndex;
std::map<int, int> nodeIdMapIndex;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float * wave_t_data = nullptr;
float * wave_a_data = nullptr;

float* t_data = nullptr;
float* r_data = nullptr;

double* dof_response = nullptr;
double* node_response = nullptr;

double * period = nullptr;

int * pointList = nullptr;

int * dofList = nullptr;
const char** dofStrList = nullptr;

int * eleList = nullptr;
const char** eleStrList = nullptr;

int * dofRecorderList = nullptr;
const char** dofRecorderStrList = nullptr;

int * eleRecorderList = nullptr;
const char** eleRecorderStrList = nullptr;

int * waveList = nullptr;
const char** waveStrList = nullptr;

int * element1dList = nullptr;

int *dofs;
int *dofs_x;
int *dofs_y;
int *dofs_z;
int *dofs_rx;
int *dofs_ry;
int *dofs_rz;
std::string dofs_str;
std::string dofs_x_str;
std::string dofs_y_str;
std::string dofs_z_str;
std::string dofs_rx_str;
std::string dofs_ry_str;
std::string dofs_rz_str;

int* points;
std::string points_str;
int* nodes;
std::string nodes_str;
int* dof_recorders;
std::string dof_recorders_str;
int* ele_recorders;
std::string ele_recorders_str;

const int dimension_dof_count[5] = {1, 3, 6, 2, 3};
const char* dimension[5] = {"1D", "2D", "3D", "2D (W/O Rotate)", "3D (W/O Rotate)" };
const char* direction[6] = {"X", "Y", "Z", "RX", "RY", "RZ"};
const char* dofResponse[4] = {"Displacement", "Velocity", "Acceleration", "ALL"};
const char* eleResponse[3] = {"Force", "Deformation", "Force and Deformation"};
const char* dynamicSolver[4] = {"Newmark", "Newmark_NL",
                                "StateSpace", "StateSpace_NL"};


void RODS_GUI::createShader()
{
    std::ifstream vFile, fFile;
    std::stringstream vShaderStream, fShaderStream;

    vFile.open("vert.glsl");
    vShaderStream << vFile.rdbuf();
    vFile.close();

    fFile.open("frag.glsl");
    fShaderStream << fFile.rdbuf();
    fFile.close();

    std::string vertexShaderSource_ = vShaderStream.str();
    std::string fragmentShaderSource_ = fShaderStream.str();
    
    const char* vertexShaderSource = vertexShaderSource_.c_str();
    const char* fragmentShaderSource = fragmentShaderSource_.c_str();

    int success;
    char infoLog[512];
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // delete shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glUseProgram(shaderProgram);
}

void RODS_GUI::buildVertex()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VBO_COLOR);
    glGenBuffers(1, &EBO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_COLOR);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void RODS_GUI::setCamera(GLFWwindow* window)
{
    glfwGetFramebufferSize(window, &buffer_width, &buffer_height);
    // glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)buffer_width / (float)buffer_height, 0.1f, 100.0f);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);
    glm::mat4 view = glm::mat4(1.0f);
    // view = glm::lookAt( glm::vec3(0.0f, 0.0f, 2.5f),
    //                     glm::vec3(0.0f, 0.0f, 0.0f),
    //                     glm::vec3(0.0f, 1.0f, 0.0f) );
    view = glm::lookAt( glm::vec3(0.0f, -2.5f, 0.0f),
                        glm::vec3(0.0f, 0.0f, 0.0f),
                        glm::vec3(0.0f, 0.0f, 1.0f) );
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f,0.0f,-0.5f));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
}

void RODS_GUI::mainMenu(GLFWwindow* window)
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New")) {
                clear();
                initVars();
            }

            char workDir[C_STR_LEN];
            get_work_dir(workDir, C_STR_LEN);
#ifdef __GNUC__
            strcat(workDir, "/");
#else
            strcat_s(workDir, "/");
#endif
            if (ImGui::MenuItem("Open")) {
                ImGuiFileDialog::Instance()->OpenDialog("Open Model", "Select File", ".json", workDir);
            }

            if (ImGui::MenuItem("Save")) {
                ImGuiFileDialog::Instance()->OpenDialog("Save Model", "Select File Path",
                                    ".json", workDir, "", 1, nullptr, ImGuiFileDialogFlags_ConfirmOverwrite);
            }

            if (ImGui::MenuItem("Exit"))
                glfwSetWindowShouldClose(window, true);

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Set"))
        {
            if (ImGui::MenuItem("Work Directory && Name"))
                show_dir_window = true;

            if (ImGui::MenuItem("Inherent Damping"))
                show_damping_window = true;

            if (ImGui::MenuItem("Drawing Mode"))
                show_draw_mode_window = true;
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Geometry"))
        {
            if (ImGui::MenuItem("Point"))
                show_point_window = true;

            if (ImGui::MenuItem("Line"))
                show_line_window = true;

            if (ImGui::MenuItem("Triangle")) {}
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Model"))
        {
            if (ImGui::MenuItem("DOF"))
                show_dof_window = true;

            if (ImGui::MenuItem("Node"))
                show_node_window = true;

            if (ImGui::MenuItem("Element1D"))
                show_element1d_window = true;

            if (ImGui::MenuItem("Element2D"))
                show_element2d_window = true;

            if (ImGui::MenuItem("Element3D")) {}

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Input/Output"))
        {
            if (ImGui::MenuItem("Wave"))
                show_wave_window = true;

            if (ImGui::MenuItem("Recorder"))
                show_recorder_window = true;

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Basic Info"))
                show_basic_info_window = true;

            if (ImGui::MenuItem("DOF Table"))
                show_dof_table_window = true;

            if (ImGui::MenuItem("Time History Curve"))
                show_time_history_plot_window = true;

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Solve"))
        {
            if (ImGui::MenuItem("Assemble Matrix"))
                show_assemble_matrix_window = true;

            if (ImGui::MenuItem("Solve Eigen"))
                show_solve_eigen_window = true;

            if (ImGui::MenuItem("Solve Seisimc"))
                show_solve_seismic_window = true;

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    if (ImGuiFileDialog::Instance()->Display("Open Model"))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            auto fileName = ImGuiFileDialog::Instance()->GetFilePathName();
            load_from_json(fileName.c_str());
            updateVars();
        }
        ImGuiFileDialog::Instance()->Close();
    }

    if (ImGuiFileDialog::Instance()->Display("Save Model"))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            auto fileName = ImGuiFileDialog::Instance()->GetFilePathName();
            save_to_json(fileName.c_str());
            updateVars();
        }
        ImGuiFileDialog::Instance()->Close();
    }
}

void RODS_GUI::dirWindow()
{
    static char workDir[C_STR_LEN] = "./";
    if (show_dir_window)
    {
        ImGui::Begin("Work Directory && Name");

        if (ImGui::Button("Select Directory"))
            ImGuiFileDialog::Instance()->OpenDialog("ChooseDirDlgKey", "Choose a Directory", nullptr, "./");

        if (ImGuiFileDialog::Instance()->Display("ChooseDirDlgKey"))
        {
            if (ImGuiFileDialog::Instance()->IsOk())
            {
                auto pathName = ImGuiFileDialog::Instance()->GetCurrentPath();
#ifdef __GNUC__
                strcpy(workDir, pathName.c_str());
#else
                strcpy_s(workDir, C_STR_LEN, pathName.c_str());
#endif
            }
            ImGuiFileDialog::Instance()->Close();
        }

        ImGui::InputText("Work Directory", workDir, C_STR_LEN);
        if (ImGui::Button("Set Work Directory"))
            set_work_dir(workDir);

        static char name[C_STR_LEN] = "RODS";
        ImGui::InputText("Name", name, C_STR_LEN);
        if (ImGui::Button("Set Name"))
            set_name(name);

        ImGui::SameLine();
        if (ImGui::Button("Close"))
            show_dir_window = false;

        ImGui::End();
    }
}

void RODS_GUI::dampingWindow()
{
    if (show_damping_window)
    {
        ImGui::Begin("Inherent Damping");
        static float zeta = 0.02f;
        ImGui::InputFloat("Damping Ratio", &zeta);
        if (ImGui::Button("Set Damping Ratio"))
            set_damping_ratio(zeta);

        ImGui::Text("Inherent Damping Model: ");
        static int inherent_damping_model = 0;
        ImGui::RadioButton("Rayleigh Damping", &inherent_damping_model, 0); ImGui::SameLine();
        ImGui::RadioButton("Mode Orthogonal Damping", &inherent_damping_model, 1);

        if (inherent_damping_model == 0)
        {
            static float rayleigh_damping_freqs[2] = { 10.0f, 20.0f };
            ImGui::InputFloat2("Circular frequencies", rayleigh_damping_freqs);
            if (ImGui::Button("Set Rayleigh Damping"))
                set_rayleigh_damping(rayleigh_damping_freqs[0], rayleigh_damping_freqs[1]);
        }
        else
        {
            static int num_modes_for_damping = 1;
            ImGui::InputInt("Number of Modes", &num_modes_for_damping);
            if (ImGui::Button("Set Mode Orthogonal Damping"))
                set_num_modes_inherent_damping(num_modes_for_damping);
        }

        ImGui::SameLine();
        if (ImGui::Button("Close"))
            show_damping_window = false;

        ImGui::End();
    }
}

void RODS_GUI::dofWindow()
{
    if (show_dof_window)
    {
        ImGui::Begin("DOF");
        static int dof_id = 1;
        ImGui::InputInt("DOF ID", &dof_id);
        static double mass = 1.0;
        ImGui::InputDouble("DOF Mass", &mass);
        static int dir = 0;
        const char * dirItems[3] = {"X", "Y", "Z"};
        ImGui::Combo("Direction", &dir, dirItems, 3);
        if (ImGui::Button("Add DOF"))
        {
            if (dofIdMapIndex.count(dof_id) == 0)
                dofIdMapIndex[dof_id] = dof_index++;
            num_dof = add_dof(dof_id++, dir, mass);
        }

        if (num_dof > 0)
        {
            ImGui::SameLine();
            if (ImGui::Button("Edit DOF"))
                ImGui::OpenPopup("Select DOF to Edit");
            if (ImGui::BeginPopup("Select DOF to Edit"))
            {
                RODS_GUI::updateDOFList();
                static int dof_index = 0;
                ImGui::Combo("DOF", &dof_index, dofStrList, num_dof);
                int selected_dof_id = dofList[dof_index];
                if (ImGui::Button("Fix"))
                    fix_dof(selected_dof_id);

                ImGui::SameLine();
                if (ImGui::Button("Free"))
                    free_dof(selected_dof_id);

                ImGui::SameLine();
                if (ImGui::Button("Remove"))
                {
                    num_dof = remove_dof(selected_dof_id);
                    dofIdMapIndex.erase(selected_dof_id);
                }
                ImGui::EndPopup();
            }
        }

        static int dof_id_bat[3] = {1,11,1};
        ImGui::InputInt3("DOF ID (Start, Stop, Step)", dof_id_bat);
        if (ImGui::Button("Batch Add DOF"))
        {
            for (auto id = dof_id_bat[0]; id <= dof_id_bat[1]; id+=dof_id_bat[2])
            {
                num_dof = add_dof(id, dir, mass);
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Close"))
            show_dof_window = false;
        ImGui::End();
    }
}

void RODS_GUI::pointWindow()
{
    if (show_point_window)
    {
        ImGui::Begin("Point");
        static int pt_id = 1;
        ImGui::InputInt("Point ID", &pt_id);
        static float coord[3] = {0.0, 0.0, 0.0};
        ImGui::InputFloat3("Coords (X,Y,Z)", coord);
        if (ImGui::Button("Add Point")) {
            if (pointIdMapIndex.count(pt_id) == 0)
            {
                pointIdMapIndex[pt_id] = point_index;
                point_index++;
            }
            num_point = add_point(pt_id++, coord[0], coord[1], coord[2]);
        }

        ImGui::SameLine();
        if (ImGui::Button("Close"))
            show_point_window = false;
        ImGui::End();
    }
}

void RODS_GUI::basicInfoWindow()
{
    if (show_basic_info_window)
    {
        ImGui::Begin("Basic Information");
        char name[C_STR_LEN];
        get_name(name, C_STR_LEN);
        ImGui::Text("Model Name: %s", name);
        ImGui::Text("Inherent Damping Ratio: %.3f", get_damping_ratio());
        ImGui::Text("Use RayleighDamping: %s", get_use_rayleigh_damping() ? "True" : "False");
        ImGui::Separator();
        ImGui::Text("Number of Points: %d", num_point);
        ImGui::Text("Number of Lines: %d", num_line);
        ImGui::Separator();
        ImGui::Text("Number of DOFs: %d", num_dof);
        ImGui::Text("Number of Nodes: %d", num_node);
        ImGui::Text("Number of Elements: %d", num_ele);
        ImGui::Text("Number of Equations: %d", num_eqn);
        ImGui::Separator();
        ImGui::Text("Number of Waves: %d", num_wave);
        ImGui::Separator();
        if (ImGui::Button("Close"))
            show_basic_info_window = false;
        ImGui::End();
    }

}

void RODS_GUI::lineWindow()
{
    if (show_line_window)
    {
        ImGui::Begin("Line");
        static int l_id = 1;
        static int p_id_i = 0;
        static int p_id_j = 0;
        ImGui::InputInt("Line ID", &l_id);
        if (ImGui::Button("Select Points"))
            ImGui::OpenPopup("Select Points for Line");
        if (ImGui::BeginPopup("Select Points for Line"))
        {
            pointList = new int [num_point];
            get_point_id(pointList);
            const char** pointItems = new const char* [num_point];
            for (int i = 0; i < num_point; i++)
            {
                auto item_str = new char[C_STR_LEN_S];
                snprintf(item_str, C_STR_LEN_S, "%d", pointList[i]);
                pointItems[i] = item_str;
            }
            static int point_index_i = 0;
            static int point_index_j = 0;
            ImGui::Combo("Point I", &point_index_i, pointItems, num_point);
            ImGui::Combo("Point J", &point_index_j, pointItems, num_point);
            p_id_i = pointList[point_index_i];
            p_id_j = pointList[point_index_j];
            ImGui::EndPopup();
        }
        ImGui::Text("Selected Points: I: %d, J: %d", p_id_i, p_id_j);

        if (ImGui::Button("Add Line")) {
            if (p_id_i == p_id_j)
                ImGui::OpenPopup("Error");
            else
                num_line = add_line(l_id++, p_id_i, p_id_j);
        }

        if (ImGui::BeginPopupModal("Error"))
        {
            ImGui::Text("Points are repetitive!");
            if (ImGui::Button("Confirm"))
                ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }

        ImGui::SameLine();
        if (ImGui::Button("Close"))
            show_line_window = false;
        ImGui::End();
    }
}

void RODS_GUI::waveWindow()
{
    if (show_wave_window)
    {
        ImGui::Begin("Wave");
        static int wave_id = 1;
        ImGui::InputInt("Wave ID", &wave_id);
        static double dt = 0.02;
        ImGui::InputDouble("Time Interval", &dt);
        static int num_wave_steps;
        static std::string waveFilePathName;
        static std::string waveFileName;

        char workDir[C_STR_LEN];
        get_work_dir(workDir, C_STR_LEN);
#ifdef __GNUC__
        strcat(workDir, "/");
#else
        strcat_s(workDir, "/");
#endif
        if (ImGui::Button("Select File"))
            ImGuiFileDialog::Instance()->OpenDialog("SelectFileDlgKey", "Select File", ".txt,.dat,.*", workDir);

        if (ImGuiFileDialog::Instance()->Display("SelectFileDlgKey"))
        {
            if (ImGuiFileDialog::Instance()->IsOk())
            {
                waveFilePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                // std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
                waveFileName = ImGuiFileDialog::Instance()->GetCurrentFileName();

                std::string line;
                std::ifstream wf(waveFilePathName);
                num_wave_steps = 0;
                while (std::getline(wf, line))
                    ++num_wave_steps;
                wf.close();

                wf.open(waveFilePathName);
                wave_a_data = new float[num_wave_steps];

                for (int i = 0; i < num_wave_steps; i++)
                {
                    wf >> wave_a_data[i];
                }
                wf.close();
            }
            ImGuiFileDialog::Instance()->Close();
        }
        ImGui::SameLine();
        ImGui::Text("Wave File: %s", waveFileName.c_str());
        ImGui::SameLine();
        ImGui::Text("Number of Steps: %d", num_wave_steps);

        if (num_wave_steps > 0)
        {
            if (dt > 0.0)
            {
                wave_t_data = new float[num_wave_steps];
                for (int i = 0; i < num_wave_steps; i++)
                {
                    wave_t_data[i] = (float)dt*i;
                }
                if (ImPlot::BeginPlot("Wave", ImVec2(-1,500)))
                {
                    ImPlot::SetupAxes("Time", "Acc", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
                    ImPlot::PlotLine(waveFileName.c_str(), wave_t_data, wave_a_data, num_wave_steps);
                    ImPlot::EndPlot();
                }
            }
        }
        if (ImGui::Button("Add Wave")) {
            char * waveFilePathName_ = &waveFilePathName[0];
            num_wave = add_wave(wave_id++, dt, waveFilePathName_);
        }

        ImGui::SameLine();
        if (ImGui::Button("Close"))
            show_wave_window = false;

        ImGui::End();
    }
}

void RODS_GUI::element1dWindow()
{
    if (show_element1d_window)
    {
        ImGui::Begin("Element1D");
        static int ele_id = 1;
        ImGui::InputInt("Element ID", &ele_id);
        static int ele_type = 0;
        const char * Element1DTypes[3] = {"Spring", "Dashpot", "Inerter"};
        ImGui::Combo("Element Type", &ele_type, Element1DTypes, 3);

        static int dof_index_i = 0;
        static int dof_index_j = 0;
        static double param[3] = {0.0, 0.0, 0.0};

        static int dof_id_i = 0;
        static int dof_id_j = 0;

        static int node_index_i = 0;
        static int node_index_j = 0;

        static int node_id_i = 0;
        static int node_id_j = 0;

        static int ele_1d_ends_type = 1;
        ImGui::Text("Type of Ends: ");
        ImGui::RadioButton("DOF", &ele_1d_ends_type, 1); ImGui::SameLine();
        ImGui::RadioButton("Node", &ele_1d_ends_type, 2);

        if (ele_1d_ends_type == 1)
        {
            if (num_dof > 1)
            {
                if (ImGui::Button("Select DOFs"))
                    ImGui::OpenPopup("Select DOFs for Element1D");
                if (ImGui::BeginPopup("Select DOFs for Element1D"))
                {
                    genDofList();
                    ImGui::Combo("dof I", &dof_index_i, dofs_str.c_str());
                    ImGui::Combo("dof J", &dof_index_j, dofs_str.c_str());
                    dof_id_i = dofs[dof_index_i];
                    dof_id_j = dofs[dof_index_j];
                    ImGui::EndPopup();
                }
                ImGui::Text("Selected DOFs: I: %d, J: %d", dof_id_i, dof_id_j);
            }
        }
        else
        {
            std::string node_dirs_str_i, node_dirs_str_j;
            static int node_dof_index_i = 0;
            static int node_dof_index_j = 0;
            if (num_node > 1)
            {
                if (ImGui::Button("Select Nodes"))
                    ImGui::OpenPopup("Select Nodes for Element1D");
                if (ImGui::BeginPopup("Select Nodes for Element1D"))
                {
                    genNodeList();
                    ImGui::Combo("node I", &node_index_i, nodes_str.c_str()); ImGui::SameLine();
                    node_id_i = nodes[node_index_i];
                    node_dirs_str_i.clear();
                    for (int i = 0; i < 6; i++)
                    {
                        if (check_node_dof(node_id_i, i))
                        {
                            node_dirs_str_i.append(direction[i]);
                            node_dirs_str_i.push_back('\0');
                        }
                    }
                    ImGui::Combo("DOF I", &node_dof_index_i, node_dirs_str_i.c_str());
                    dof_id_i = get_id_node_dof(node_id_i, node_dof_index_i);

                    ImGui::Combo("node J", &node_index_j, nodes_str.c_str()); ImGui::SameLine();
                    node_id_j = nodes[node_index_j];
                    node_dirs_str_j.clear();
                    for (int i = 0; i < 6; i++)
                    {
                        if (check_node_dof(node_id_j, i))
                        {
                            node_dirs_str_j.append(direction[i]);
                            node_dirs_str_j.push_back('\0');
                        }
                    }
                    ImGui::Combo("DOF J", &node_dof_index_j, node_dirs_str_j.c_str());
                    dof_id_j = get_id_node_dof(node_id_j, node_dof_index_j);
                    ImGui::EndPopup();
                }
                ImGui::Text("Selected Nodes: I: %d, J: %d", node_id_i, node_id_j);
                ImGui::Text("Selected DOFs: I: %d, J: %d", dof_id_i, dof_id_j);
            }
        }

        if ((ele_1d_ends_type==1 && num_dof>1) || (ele_1d_ends_type==2 && num_node>1))
        {
            switch (ele_type)
            {
            case 0:
            {
                ImGui::InputDouble("Stiffness", param);
                if (ImGui::Button("Add Element1D")) {
                    if (dof_id_i == dof_id_j)
                        ImGui::OpenPopup("Error");
                    else
                    {
                        num_spring = add_spring(ele_id++, dofList[dof_index_i], dofList[dof_index_j], param[0]);
                        num_ele = get_num_ele();
                    }
                }
            }
                break;
            case 1:
            {
                ImGui::InputDouble("Damping Coefficient", param);
                if (ImGui::Button("Add Element1D")) {
                    if (dof_id_i == dof_id_j)
                        ImGui::OpenPopup("Error");
                    else
                    {
                        num_dashpot = add_dashpot(ele_id++, dofList[dof_index_i], dofList[dof_index_j], param[0]);
                        num_ele = get_num_ele();
                    }
                }
            }
                break;
            case 2:
            {
                ImGui::InputDouble("Inertance", param);
                if (ImGui::Button("Add Element1D")) {
                    if (dof_id_i == dof_id_j)
                        ImGui::OpenPopup("Error");
                    else
                    {
                        num_inerter = add_inerter(ele_id++, dofList[dof_index_i], dofList[dof_index_j], param[0]);
                        num_ele = get_num_ele();
                    }
                }
            }
                break;
            default:
                break;
            }

            if (ImGui::BeginPopupModal("Error"))
            {
                ImGui::Text("DOFs are repetitive!");
                if (ImGui::Button("Confirm"))
                    ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }

            if (ele_1d_ends_type == 1)
            {
                static int ele_id_bat[3] = {1,10,1};
                static int dof_i_id_bat[2] = {1,1};
                static int dof_j_id_bat[2] = {2,1};
                ImGui::InputInt3("Element ID (Start, Stop, Step)", ele_id_bat);
                ImGui::InputInt2("DOF I ID (Start, Step)", dof_i_id_bat);
                ImGui::InputInt2("DOF J ID (Start, Step)", dof_j_id_bat);

                if (ImGui::Button("Batch Add Element1D"))
                {
                    auto dof_i_id = dof_i_id_bat[0];
                    auto dof_j_id = dof_j_id_bat[0];
                    for (auto id = ele_id_bat[0]; id <= ele_id_bat[1]; id+=ele_id_bat[2])
                    {
                        switch (ele_type)
                        {
                        case 0:
                            num_spring = add_spring(id, dof_i_id, dof_j_id, param[0]);
                            num_ele = get_num_ele();
                            break;
                        case 1:
                            num_dashpot = add_dashpot(id, dof_i_id, dof_j_id, param[0]);
                            num_ele = get_num_ele();
                            break;
                        case 2:
                            num_inerter = add_inerter(id, dof_i_id, dof_j_id, param[0]);
                            num_ele = get_num_ele();
                            break;
                        default:
                            break;
                        }
                        dof_i_id += dof_i_id_bat[1];
                        dof_j_id += dof_j_id_bat[1];
                    }
                }
            }
        }

        if (ImGui::Button("Close"))
            show_element1d_window = false;

        ImGui::End();
    }
}

void RODS_GUI::element2dWindow()
{
    if (show_element2d_window)
    {
        ImGui::Begin("Element 2D");
        static int ele_id = 1;
        ImGui::InputInt("Element ID", &ele_id);
        static int ele_type = 0;
        const char * Element2DTypes[5] = {
                    "TrussElastic2D",
                    "FrameElastic2D",
                    "Spring2D",
                    "Dashpot2D",
                    "Inerter2D",
                    };
        ImGui::Combo("Element Type", &ele_type, Element2DTypes, 5);

        static int node_item_index_i = 0;
        static int node_item_index_j = 0;
        static int node_id_i = 0;
        static int node_id_j = 0;
        if (ImGui::Button("Select Nodes"))
            ImGui::OpenPopup("Select Nodes");
        if (ImGui::BeginPopup("Select Nodes"))
        {
            genNodeList();
            if (num_node > 1)
            {
                ImGui::Combo("Node I", &node_item_index_i, nodes_str.c_str());
                ImGui::Combo("Node J", &node_item_index_j, nodes_str.c_str());
                node_id_i = nodes[node_item_index_i];
                node_id_j = nodes[node_item_index_j];
            }
            ImGui::EndPopup();
        }
        ImGui::Text("Selected Nodes: %d, %d", node_id_i, node_id_j);

        static float * params = new float[3]();

        if (ele_type == 0)
        {
            ImGui::InputFloat("EA", params);
            if (ImGui::Button("Add Element"))
            {
                num_truss_elastic_2d = add_truss_elastic_2d(ele_id++, node_id_i, node_id_j, params[0]);
                num_ele = get_num_ele();
            }
        }

        if (ele_type == 1)
        {
            ImGui::InputFloat2("EA and EI", params);
            if (ImGui::Button("Add Element"))
            {
                num_frame_elastic_2d = add_frame_elastic_2d(ele_id++, node_id_i, node_id_j, params[0], params[1]);
                num_ele = get_num_ele();
            }
        }

        if (ImGui::Button("Close"))
            show_element2d_window = false;

        ImGui::End();
    }
}

void RODS_GUI::nodeWindow()
{
    if (show_node_window)
    {
        ImGui::Begin("Node");
        static int node_id = 1;
        ImGui::InputInt("Node ID", &node_id);
        static int node_dim = 0;
        ImGui::Combo("Dimension", &node_dim, dimension, 5);
        ImGui::Text("DOFs: "); ImGui::SameLine();
        static int dof_method = 2;
        ImGui::RadioButton("Select", &dof_method, 1); ImGui::SameLine();
        ImGui::RadioButton("Auto Generate", &dof_method, 2);
        ImGui::Text("Coordinates: "); ImGui::SameLine();
        static int coord_method = 1;
        ImGui::RadioButton("Directly Input", &coord_method, 1); ImGui::SameLine();
        ImGui::RadioButton("From a Point", &coord_method, 2);
        int dof_count = dimension_dof_count[node_dim];
        static int* dof_item_index = new int[dof_count] {0};
        static int dof_item_index_x = 0;
        static int dof_item_index_y = 0;
        static int dof_item_index_z = 0;
        static int dof_item_index_rx = 0;
        static int dof_item_index_ry = 0;
        static int dof_item_index_rz = 0;
        static int* dof_id = new int[dof_count] {0};
        static int point_item_index = 0;
        static int point_id = 0;
        static double mass = 1e-6;
        ImGui::InputDouble("Mass", &mass);
        static double I = 1e-6;
        if (node_dim == 1 || node_dim == 2)
            ImGui::InputDouble("Moment of Inertia", &I);

        if (dof_method == 1)
        {
            if (ImGui::Button("Select DOF") && num_dof>1)
                ImGui::OpenPopup("Select DOF");
            if (ImGui::BeginPopup("Select DOF"))
            {
                genDofList();
                switch (node_dim)
                {
                case 0:
                    if (num_dof_x > 0)
                    {
                        ImGui::Combo("DOF X", &dof_item_index_x, dofs_x_str.c_str());
                        dof_id[0] = dofs_x[dof_item_index_x];
                    }
                    break;
                case 1:
                    if (num_dof_x*num_dof_z*num_dof_ry > 0)
                    {
                        ImGui::Combo("DOF X",  &dof_item_index_x, dofs_x_str.c_str(), num_dof_x);
                        ImGui::Combo("DOF Z",  &dof_item_index_z, dofs_z_str.c_str(), num_dof_z);
                        ImGui::Combo("DOF RY", &dof_item_index_ry, dofs_ry_str.c_str(), num_dof_ry);
                        dof_id[0] = dofs_x[dof_item_index_x];
                        dof_id[1] = dofs_z[dof_item_index_z];
                        dof_id[2] = dofs_ry[dof_item_index_ry];
                    }
                    break;
                case 2:
                    if (num_dof_x*num_dof_y*num_dof_z*num_dof_rx*num_dof_ry*num_dof_rz > 0)
                    {
                        ImGui::Combo("DOF X",  &dof_item_index_x, dofs_x_str.c_str(), num_dof_x);
                        ImGui::Combo("DOF Y",  &dof_item_index_y, dofs_y_str.c_str(), num_dof_y);
                        ImGui::Combo("DOF Z",  &dof_item_index_z, dofs_z_str.c_str(), num_dof_z);
                        ImGui::Combo("DOF RX", &dof_item_index_rx, dofs_rx_str.c_str(), num_dof_rx);
                        ImGui::Combo("DOF RY", &dof_item_index_ry, dofs_ry_str.c_str(), num_dof_ry);
                        ImGui::Combo("DOF RZ", &dof_item_index_rz, dofs_rz_str.c_str(), num_dof_rz);
                        dof_id[0] = dofs_x[dof_item_index_x];
                        dof_id[1] = dofs_y[dof_item_index_y];
                        dof_id[2] = dofs_z[dof_item_index_z];
                        dof_id[3] = dofs_rx[dof_item_index_rx];
                        dof_id[4] = dofs_ry[dof_item_index_ry];
                        dof_id[5] = dofs_rz[dof_item_index_rz];
                    }
                    break;
                case 3:
                    if (num_dof_x*num_dof_z > 0)
                    {
                        ImGui::Combo("DOF X",  &dof_item_index_x, dofs_x_str.c_str(), num_dof_x);
                        ImGui::Combo("DOF Z",  &dof_item_index_z, dofs_z_str.c_str(), num_dof_z);
                        dof_id[0] = dofs_x[dof_item_index_x];
                        dof_id[1] = dofs_z[dof_item_index_z];
                    }
                    break;
                case 4:
                    if (num_dof_x*num_dof_y*num_dof_z > 0)
                    {
                        ImGui::Combo("DOF X",  &dof_item_index_x, dofs_x_str.c_str(), num_dof_x);
                        ImGui::Combo("DOF Y",  &dof_item_index_y, dofs_y_str.c_str(), num_dof_y);
                        ImGui::Combo("DOF Z",  &dof_item_index_z, dofs_z_str.c_str(), num_dof_z);
                        dof_id[0] = dofs_x[dof_item_index_x];
                        dof_id[1] = dofs_y[dof_item_index_y];
                        dof_id[2] = dofs_z[dof_item_index_z];
                    }
                    break;
                default:
                    break;
                }
                ImGui::EndPopup();
            }
            ImGui::Text("Selected DOFs: ");
            for (int i = 0; i < dof_count; i++)
            {
                ImGui::SameLine();
                ImGui::Text("%d\t", dof_id[i]);
            }

            if (coord_method == 1)
            {
                static float coord[3] = {0.0, 0.0, 0.0};
                switch (node_dim)
                {
                    case 0:
                        ImGui::InputFloat("Coords (X)", coord);
                        break;
                    case 1:
                        ImGui::InputFloat2("Coords (X,Z)", coord);
                        break;
                    case 2:
                        ImGui::InputFloat3("Coords (X,Y,Z)", coord);
                        break;
                    case 3:
                        ImGui::InputFloat2("Coords (X,Z)", coord);
                        break;
                    case 4:
                        ImGui::InputFloat3("Coords (X,Y,Z)", coord);
                        break;
                    default:
                        break;
                }
                if (ImGui::Button("Add Node"))
                {
                    switch (node_dim)
                    {
                    case 0:
                        num_node = add_node_1d(node_id, coord[0], dof_id[0]);
                        set_node_mass(node_id, mass);
                        break;
                    case 1:
                        num_node = add_node_2d(node_id, coord[0], coord[1], dof_id[0], dof_id[1], dof_id[2]);
                        set_node_mass_and_moment_of_inertia(node_id, mass, I);
                        break;
                    case 2:
                        num_node = add_node_3d(node_id, coord[0], coord[1], coord[2],
                                                dof_id[0], dof_id[1], dof_id[2],
                                                dof_id[3], dof_id[4], dof_id[5]);
                        set_node_mass_and_moment_of_inertia(node_id, mass, I);
                        break;
                    case 3:
                        num_node = add_node_2d(node_id, coord[0], coord[1], dof_id[0], dof_id[1], -1);
                        set_node_mass(node_id, mass);
                        break;
                    case 4:
                        num_node = add_node_3d(node_id, coord[0], coord[1], coord[2],
                                                dof_id[0], dof_id[1], dof_id[2],
                                                -1, -1, -1);
                        set_node_mass(node_id, mass);
                        break;
                    default:
                        break;
                    }
                    node_id++;
                }
            }
            else
            {
                if (num_point>0)
                {
                    if (ImGui::Button("Select Point"))
                        ImGui::OpenPopup("Select Point");

                    if (ImGui::BeginPopup("Select Point"))
                    {
                        genPointList();
                        ImGui::Combo("Point",  &point_item_index, points_str.c_str(), num_point);
                        point_id = points[point_item_index];
                        ImGui::EndPopup();
                    }
                    ImGui::Text("Selected Point: %d", point_id);
                    if (ImGui::Button("Add Node"))
                    {
                        switch (node_dim)
                        {
                        case 0:
                            num_node = add_node_1d_via_point(node_id, point_id, dof_id[0]);
                            set_node_mass(node_id, mass);
                            break;
                        case 1:
                            num_node = add_node_2d_via_point(node_id, point_id, dof_id[0], dof_id[1], dof_id[2]);
                            set_node_mass_and_moment_of_inertia(node_id, mass, I);
                            break;
                        case 2:
                            num_node = add_node_3d_via_point(node_id, point_id,
                                dof_id[0], dof_id[1], dof_id[2],
                                dof_id[3], dof_id[4], dof_id[5]);
                            set_node_mass_and_moment_of_inertia(node_id, mass, I);
                            break;
                        case 3:
                            num_node = add_node_2d_via_point(node_id, point_id, dof_id[0], dof_id[1], -1);
                            set_node_mass(node_id, mass);
                            break;
                        case 4:
                            num_node = add_node_3d_via_point(node_id, point_id,
                                dof_id[0], dof_id[1], dof_id[2],
                                -1, -1, -1);
                            set_node_mass(node_id, mass);
                            break;
                        default:
                            break;
                        }
                        node_id++;
                    }
                }
            }
        }
        else
        {
            if (coord_method == 1)
            {
                static float coord[3] = {0.0, 0.0, 0.0};
                switch (node_dim)
                {
                    case 0:
                        ImGui::InputFloat("Coords (X)", coord);
                        break;
                    case 1:
                        ImGui::InputFloat2("Coords (X,Z)", coord);
                        break;
                    case 2:
                        ImGui::InputFloat3("Coords (X,Y,Z)", coord);
                        break;
                    case 3:
                        ImGui::InputFloat2("Coords (X,Z)", coord);
                        break;
                    case 4:
                        ImGui::InputFloat3("Coords (X,Y,Z)", coord);
                        break;
                    default:
                        break;
                }
                if (ImGui::Button("Add Node"))
                {
                    switch (node_dim)
                    {
                    case 0:
                        num_node = add_node_1d_auto_dof(node_id, coord[0]);
                        set_node_mass(node_id, mass);
                        break;
                    case 1:
                        num_node = add_node_2d_auto_dof(node_id, coord[0], coord[1]);
                        set_node_mass_and_moment_of_inertia(node_id, mass, I);
                        break;
                    case 2:
                        num_node = add_node_3d_auto_dof(node_id, coord[0], coord[1], coord[2]);
                        set_node_mass_and_moment_of_inertia(node_id, mass, I);
                        break;
                    case 3:
                        num_node = add_node_2d_auto_dof(node_id, coord[0], coord[1], false);
                        set_node_mass(node_id, mass);
                        break;
                    case 4:
                        num_node = add_node_3d_auto_dof(node_id, coord[0], coord[1], coord[2], false);
                        set_node_mass(node_id, mass);
                        break;
                    default:
                        break;
                    }
                node_id++;
                }

                ImGui::SameLine();
                if (ImGui::Button("Edit"))
                    ImGui::OpenPopup("Edit Node");

                if (ImGui::BeginPopup("Edit Node"))
                {
                    genNodeList();
                    if (num_node > 0)
                    {
                        static int node_item_index = 0;
                        ImGui::Combo("Node", &node_item_index, nodes_str.c_str());
                        if (ImGui::Button("Remove"))
                        {
                            num_node = remove_node(nodes[node_item_index]);
                            node_item_index--;
                        }
                        ImGui::SameLine();
                        if (ImGui::Button("Fix"))
                        {
                            fix_node(nodes[node_item_index]);
                        }
                        ImGui::SameLine();
                        if (ImGui::Button("Free"))
                        {
                            free_node(nodes[node_item_index]);
                        }
                    }
                    ImGui::EndPopup();
                }

                ImGui::Separator();

                static float x_grid[3] = {0.0, 0.0, 1.0};
                static float y_grid[3] = {0.0, 0.0, 1.0};
                static float z_grid[3] = {0.0, 0.0, 1.0};
                ImGui::Text("Init, Incr, Count of Coords:");
                switch (node_dim)
                {
                    case 0:
                        ImGui::InputFloat3("X", x_grid);
                        break;
                    case 1:
                        ImGui::InputFloat3("X", x_grid);
                        ImGui::InputFloat3("Z", z_grid);
                        break;
                    case 2:
                        ImGui::InputFloat3("X", x_grid);
                        ImGui::InputFloat3("Y", y_grid);
                        ImGui::InputFloat3("Z", z_grid);
                        break;
                    case 3:
                        ImGui::InputFloat3("X", x_grid);
                        ImGui::InputFloat3("Z", z_grid);
                        break;
                    case 4:
                        ImGui::InputFloat3("X", x_grid);
                        ImGui::InputFloat3("Y", y_grid);
                        ImGui::InputFloat3("Z", z_grid);
                        break;
                    default:
                        break;
                }

                if (ImGui::Button("Batch Add Node"))
                {
                    int x_grid_count = (int)x_grid[2];
                    int y_grid_count = (int)y_grid[2];
                    int z_grid_count = (int)z_grid[2];
                    int id;
                    switch (node_dim)
                    {
                    case 0:
                        for (int i = 0; i < x_grid_count; i++)
                        {
                            id = node_id+i;
                            num_node = add_node_1d_auto_dof(id, x_grid[0]+x_grid[1]*i);
                            set_node_mass(id, mass);
                        }
                        node_id += x_grid_count;
                        break;
                    case 1:
                        id = node_id;
                        for (int i = 0; i < x_grid_count; i++)
                        {
                            for (int j = 0; j < z_grid_count; j++)
                            {
                                auto coord_x = x_grid[0]+x_grid[1]*i;
                                auto coord_z = z_grid[0]+z_grid[1]*j;
                                num_node = add_node_2d_auto_dof(id, coord_x, coord_z);
                                set_node_mass_and_moment_of_inertia(id++, mass, I);
                            }
                        }
                        node_id = id;
                        break;
                    case 2:
                        id = node_id;
                        for (int i = 0; i < x_grid_count; i++)
                        {
                            for (int j = 0; j < y_grid_count; j++)
                            {
                                for (int k = 0; k < z_grid_count; k++)
                                {
                                    auto coord_x = x_grid[0]+x_grid[1]*i;
                                    auto coord_y = y_grid[0]+y_grid[1]*j;
                                    auto coord_z = z_grid[0]+z_grid[1]*k;
                                    num_node = add_node_3d_auto_dof(id, coord_x, coord_y, coord_z);
                                    set_node_mass_and_moment_of_inertia(id++, mass, I);
                                }
                            }
                        }
                        node_id = id;
                        break;
                    case 3:
                        id = node_id;
                        for (int i = 0; i < x_grid_count; i++)
                        {
                            for (int j = 0; j < z_grid_count; j++)
                            {
                                auto coord_x = x_grid[0]+x_grid[1]*i;
                                auto coord_z = z_grid[0]+z_grid[1]*j;
                                num_node = add_node_2d_auto_dof(id, coord_x, coord_z);
                                set_node_mass_and_moment_of_inertia(id++, mass, I);
                            }
                        }
                        node_id = id;
                        break;
                    case 4:
                        id = node_id;
                        for (int i = 0; i < x_grid_count; i++)
                        {
                            for (int j = 0; j < y_grid_count; j++)
                            {
                                for (int k = 0; k < z_grid_count; k++)
                                {
                                    auto coord_x = x_grid[0]+x_grid[1]*i;
                                    auto coord_y = y_grid[0]+y_grid[1]*j;
                                    auto coord_z = z_grid[0]+z_grid[1]*k;
                                    num_node = add_node_3d_auto_dof(id, coord_x, coord_y, coord_z);
                                    set_node_mass_and_moment_of_inertia(id++, mass, I);
                                }
                            }
                        }
                        node_id = id;
                        break;
                    default:
                        break;
                    }
                }
            }
            else
            {
                if (num_point>0)
                {
                    if (ImGui::Button("Select Point"))
                        ImGui::OpenPopup("Select Point");

                    if (ImGui::BeginPopup("Select Point"))
                    {
                        genPointList();
                        ImGui::Combo("Point",  &point_item_index, points_str.c_str(), num_point);
                        point_id = points[point_item_index];
                        ImGui::EndPopup();
                    }
                    ImGui::Text("Selected Point: %d", point_id);
                    if (ImGui::Button("Add Node"))
                    {
                        switch (node_dim)
                        {
                        case 0:
                            num_node = add_node_1d_via_point_auto_dof(node_id, point_id);
                            set_node_mass(node_id, mass);
                            break;
                        case 1:
                            num_node = add_node_2d_via_point_auto_dof(node_id, point_id);
                            set_node_mass_and_moment_of_inertia(node_id, mass, I);
                            break;
                        case 2:
                            num_node = add_node_3d_via_point_auto_dof(node_id, point_id);
                            set_node_mass_and_moment_of_inertia(node_id, mass, I);
                            break;
                        case 3:
                            num_node = add_node_2d_via_point_auto_dof(node_id, point_id, false);
                            set_node_mass(node_id, mass);
                            break;
                        case 4:
                            num_node = add_node_3d_via_point_auto_dof(node_id, point_id, false);
                            set_node_mass(node_id, mass);
                            break;
                        default:
                            break;
                        }
                        node_id++;
                    }

                    ImGui::SameLine();
                    if (ImGui::Button("Edit"))
                        ImGui::OpenPopup("Edit Node");

                    if (ImGui::BeginPopup("Edit Node"))
                    {
                        genNodeList();
                        if (num_node > 0)
                        {
                            static int node_item_index = 0;
                            ImGui::Combo("Node", &node_item_index, nodes_str.c_str());
                            if (ImGui::Button("Remove"))
                            {
                                num_node = remove_node(nodes[node_item_index]);
                                node_item_index--;
                            }
                            ImGui::SameLine();
                            if (ImGui::Button("Fix"))
                            {
                                fix_node(nodes[node_item_index]);
                            }
                            ImGui::SameLine();
                            if (ImGui::Button("Free"))
                            {
                                free_node(nodes[node_item_index]);
                            }
                        }
                        ImGui::EndPopup();
                    }
                }
            }
        }

        if (ImGui::Button("Close"))
            show_node_window = false;

        ImGui::End();
    }
}

void RODS_GUI::assembleMatrixWindow()
{
    if (show_assemble_matrix_window)
    {
        ImGui::Begin("Assemble");
        static bool is_assembled = false;

        if (ImGui::Button("Assemble Matrix"))
        {
            if (num_ele>0)
            {
                num_eqn = assemble_matrix();
                is_assembled = true;
            }
            else
            {
                is_assembled = false;
            }
        }

        if (is_assembled)
        {
            ImGui::Text("Matrices are built!");
            ImGui::Text("Number of Equations: %d", num_eqn);
        }
        else
        {
            ImGui::Text("Matrices are not built!");
        }

        if (ImGui::Button("Close"))
            show_assemble_matrix_window = false;

        ImGui::End();
    }
}

void RODS_GUI::solveEigenWindow()
{
    if (show_solve_eigen_window)
    {
        ImGui::Begin("Solve Eigen");

        static bool eigen_solved = false;

        if (ImGui::Button("Solve Eigen"))
        {
            if (num_dof>0 && num_ele>0)
            {
                solve_eigen();
                period = new double[num_eqn];
                get_period(period);
                eigen_solved = true;
            }
            else
            {
                eigen_solved = false;
            }
        }

        if (eigen_solved)
        {
            if (ImGui::BeginTable("Period Table", 2))
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Order");
                ImGui::TableNextColumn();
                ImGui::Text("Period");
                for (int row = 0; row < num_eqn; row++)
                {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("%d", row + 1);
                    ImGui::TableNextColumn();
                    ImGui::Text("%.6f", period[row]);
                }
                ImGui::EndTable();
            }

            ImGui::SliderInt("Order", &mode_order, 1, num_eqn);
            ImGui::InputFloat("Scale Factor", &scale_factor_dsp);

            if (ImGui::Button("Draw Mode"))
            {
                draw_type = 2;
            }
            ImGui::SameLine();
            if (ImGui::Button("Resume"))
            {
                draw_type = 1;
            }

            if (ImGui::Button("Start Animation"))
            {
                draw_type = 22;
            }
            ImGui::SameLine();
            if (ImGui::Button("Stop"))
            {
                draw_type = 2;
            }
        }

        if (ImGui::Button("Close"))
        {
            show_solve_eigen_window = false;
            draw_type = 1;
        }
        ImGui::End();
    }
}

void RODS_GUI::recorderWindow()
{
    if (show_recorder_window)
    {
        ImGui::Begin("Recorder");

        static int recorder_type = 0;
        ImGui::Text("Recorder Type"); ImGui::SameLine();
        ImGui::RadioButton("DOF", &recorder_type, 0); ImGui::SameLine();
        ImGui::RadioButton("Element", &recorder_type, 1);

        if (recorder_type == 0)
        {
            static bool record_all = 0;
            ImGui::Checkbox("Record All", &record_all);

            static int dof_recorder_id = 1;
            ImGui::InputInt("Recorder ID", &dof_recorder_id);

            if (!record_all)
            {
                static int response_type = 0;
                ImGui::Combo("Response Type", &response_type, dofResponse, 4);

                char workDir[C_STR_LEN];
                get_work_dir(workDir, C_STR_LEN);
#ifdef __GNUC__
                strcat(workDir, "/");
#else
                strcat_s(workDir, "/");
#endif
                if (ImGui::Button("Set File for Recorder"))
                    ImGuiFileDialog::Instance()->OpenDialog("RecorderFileDlgKey", "Select File Path",
                                    ".*", workDir, "", 1, nullptr, ImGuiFileDialogFlags_ConfirmOverwrite);
                static std::string recorderFilePathName = "";
                static char _recorderFilePathName[C_STR_LEN] = "r.txt";
                ImGui::InputText("Recorder File", _recorderFilePathName, C_STR_LEN);
                if (ImGuiFileDialog::Instance()->Display("RecorderFileDlgKey"))
                {
                    if (ImGuiFileDialog::Instance()->IsOk())
                    {
                        recorderFilePathName = ImGuiFileDialog::Instance()->GetFilePathName();
#ifdef __GNUC__
                        strcpy(_recorderFilePathName, recorderFilePathName.c_str());
#else
                        strcpy_s(_recorderFilePathName, C_STR_LEN, recorderFilePathName.c_str());
#endif
                    }
                    ImGuiFileDialog::Instance()->Close();
                }
                if (ImGui::Button("Add DOF Recorder"))
                {
                    if (response_type < 3)
                        num_dof_recorder = add_dof_recorder(dof_recorder_id++, response_type, _recorderFilePathName);
                    else
                        num_dof_recorder = add_dof_recorder(dof_recorder_id++, 5, _recorderFilePathName);
                }

                if (num_dof_recorder > 0 && num_dof > 0)
                {
                    ImGui::Separator();

                    if (ImGui::Button("Select DOF"))
                        ImGui::OpenPopup("Select DOF");

                    if (ImGui::BeginPopup("Select DOF"))
                    {
                        genDofList();
                        updateDOFRecorderList();
                        static int dof_index = 0;
                        static int dof_recorder_index = 0;
                        ImGui::Combo("DOF", &dof_index, dofs_str.c_str());
                        ImGui::Combo("DOF Recorder", &dof_recorder_index, dofRecorderStrList, num_dof_recorder);
                        if (ImGui::Button("Add"))
                            add_dof_to_recorder(dofList[dof_index], dofRecorderList[dof_recorder_index]);
                        ImGui::EndPopup();
                    }
                }
            }
            else
            {
                if (ImGui::Button("Add DOF Recorder"))
                    record_all_dof_response(dof_recorder_id);
            }
        }
        else
        {
            static bool record_all = 0;
            ImGui::Checkbox("Record All", &record_all);
            static int ele_recorder_id = 1;
            ImGui::InputInt("Recorder ID", &ele_recorder_id);

            if (!record_all)
            {
                static int response_type = 0;
                ImGui::Combo("Response Type", &response_type, eleResponse, 3);

                char workDir[C_STR_LEN];
                get_work_dir(workDir, C_STR_LEN);
#ifdef __GNUC__
                strcat(workDir, "/");
#else
                strcat_s(workDir, "/");
#endif
                if (ImGui::Button("Set File for Recorder"))
                    ImGuiFileDialog::Instance()->OpenDialog("RecorderFileDlgKey", "Select File Path",
                                    ".*", workDir, "", 1, nullptr, ImGuiFileDialogFlags_ConfirmOverwrite);
                static std::string recorderFilePathName = "";
                static char _recorderFilePathName[C_STR_LEN] = "r.txt";
                ImGui::InputText("Recorder File", _recorderFilePathName, C_STR_LEN);
                if (ImGuiFileDialog::Instance()->Display("RecorderFileDlgKey"))
                {
                    if (ImGuiFileDialog::Instance()->IsOk())
                    {
                        recorderFilePathName = ImGuiFileDialog::Instance()->GetFilePathName();
#ifdef __GNUC__
                        strcpy(_recorderFilePathName, recorderFilePathName.c_str());
#else
                        strcpy_s(_recorderFilePathName, C_STR_LEN, recorderFilePathName.c_str());
#endif
                    }
                    ImGuiFileDialog::Instance()->Close();
                }
                if (ImGui::Button("Add Element Recorder"))
                    num_ele_recorder = add_ele_recorder(ele_recorder_id++, response_type+3, _recorderFilePathName);

                if (num_ele_recorder > 0 && num_ele > 0)
                {
                    ImGui::Separator();

                    if (ImGui::Button("Select Element"))
                        ImGui::OpenPopup("Select Element");

                    if (ImGui::BeginPopup("Select Element"))
                    {
                        updateEleList();
                        updateEleRecorderList();
                        static int ele_index = 0;
                        static int ele_recorder_index = 0;
                        ImGui::Combo("Element", &ele_index, dofStrList, num_dof);
                        ImGui::Combo("Element Recorder", &ele_recorder_index, dofRecorderStrList, num_ele_recorder);
                        if (ImGui::Button("Add"))
                        {
                            add_ele_to_recorder(eleList[ele_index], eleRecorderList[ele_recorder_index]);
                            ImGui::SameLine();
                            ImGui::Text("Element %d is added to Recorder %d.",
                                        eleList[ele_index], eleRecorderList[ele_recorder_index]);
                        }
                        ImGui::EndPopup();
                    }
                }
            }
            else
            {
                if (ImGui::Button("Add Element Recorder"))
                    record_all_ele_response(ele_recorder_id);
            }
        }

        if (ImGui::Button("Close"))
            show_recorder_window = false;

        ImGui::End();
    }
}

void RODS_GUI::solveSeismicWindow()
{
    if (show_solve_seismic_window)
    {
        ImGui::Begin("Solve Seismic");
        static int dy_solver_index = 0;
        ImGui::Combo("Dynamic Solver", &dy_solver_index, dynamicSolver, 4);
        if (ImGui::Button("Set Solver"))
            set_dynamic_solver(dy_solver_index);

        static double scale_factor = 1.0;
        ImGui::InputDouble("Scale Factor", &scale_factor);
        static int dir = 0;
        const char * dirs[3] = {"X", "Y", "Z"};
        ImGui::Combo("Active Direction", &dir, dirs, 3);

        if (num_wave > 0)
        {
            if (ImGui::Button("Select Wave") && num_wave>0)
                ImGui::OpenPopup("Select Wave");

            static int wave_index = 0;
            static int wave_id = 0;
            if (ImGui::BeginPopup("Select Wave"))
            {
                updateWaveList();
                ImGui::Combo("Wave", &wave_index, waveStrList, num_wave);
                wave_id = waveList[wave_index];
                ImGui::EndPopup();
            }

            ImGui::SameLine();
            ImGui::Text("Wave %d is Selected.", wave_id);

            if (ImGui::Button("Activate Groud Motion"))
                active_ground_motion(dir, wave_id, scale_factor);

            static int num_sub_steps = 1;
            ImGui::InputInt("Number of Substeps", &num_sub_steps);

            if (ImGui::Button("Solve Seismic Respones"))
                solve_seismic_response(num_sub_steps);

            ImGui::SameLine();
        }

        if (ImGui::Button("Close"))
            show_solve_seismic_window = false;

        ImGui::End();
    }
}

void RODS_GUI::dofTableWindow()
{
    if (show_dof_table_window)
    {
        ImGui::Begin("DOF Table");

        if (ImGui::BeginTable("DOF Table", 4))
        {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("ID");
            ImGui::TableNextColumn();
            ImGui::Text("Direction");
            ImGui::TableNextColumn();
            ImGui::Text("Mass");
            ImGui::TableNextColumn();
            ImGui::Text("Fixed");
            updateDOFList();
            for (int row = 0; row < num_dof; row++)
            {
                int id = dofList[row];
                int dir;
                double mass;
                bool is_fixed;
                get_dof_info(id, dir, mass, is_fixed);
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("%d", id);
                ImGui::TableNextColumn();
                ImGui::Text("%s", direction[dir]);
                ImGui::TableNextColumn();
                ImGui::Text("%.3f", mass);
                ImGui::TableNextColumn();
                ImGui::Text(is_fixed ? "Yes":"No");
            }
            ImGui::EndTable();
        }

        if (ImGui::Button("Close"))
            show_dof_table_window = false;

        ImGui::End();
    }
}

void RODS_GUI::element1dTableWindow()
{
    if (show_element1d_table_window)
    {
        ImGui::Begin("Element1D Table");

        if (ImGui::BeginTable("Element1D Table", 4))
        {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("ID");
            ImGui::TableNextColumn();
            ImGui::Text("Type");
            ImGui::TableNextColumn();
            ImGui::Text("DOFI");
            ImGui::TableNextColumn();
            ImGui::Text("DOFJ");
            updateDOFList();
            for (int row = 0; row < num_dof; row++)
            {
                int id = dofList[row];
                int dir;
                double mass;
                bool is_fixed;
                get_dof_info(id, dir, mass, is_fixed);
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("%d", id);
                ImGui::TableNextColumn();
                ImGui::Text("%s", direction[dir]);
                ImGui::TableNextColumn();
                ImGui::Text("%.3f", mass);
                ImGui::TableNextColumn();
                ImGui::Text(is_fixed ? "Yes":"No");
            }
            ImGui::EndTable();
        }

        if (ImGui::Button("Close"))
            show_dof_table_window = false;

        ImGui::End();
    }
}

void RODS_GUI::timeHistoryPlotWindow()
{
    if (show_time_history_plot_window)
    {
        ImGui::Begin("Time History Plot");

        static int num_res_steps = 0;
        static std::string resFilePathName;
        static std::string resFileName;

        static int dof_response_type = 0;
        ImGui::Combo("Response Type", &dof_response_type, dofResponse, 3);

        if (ImGui::Button("Select DOF"))
            ImGui::OpenPopup("Select DOF");

        static int dof_item_index = 0;
        static int dof_id = 0;
        static int dof_id_ = 0;
        if (ImGui::BeginPopup("Select DOF"))
        {
            genDofList();
            ImGui::Combo("DOF", &dof_item_index, dofs_str.c_str());
            dof_id = dofs[dof_item_index];
            ImGui::EndPopup();
        }
        ImGui::SameLine();
        ImGui::Text("Selected DOF: %d", dof_id);

        char workDir[C_STR_LEN];
        get_work_dir(workDir, C_STR_LEN);
#ifdef __GNUC__
        strcat(workDir, "/");
#else
        strcat_s(workDir, "/");
#endif
        if (ImGui::Button("Select File"))
            ImGuiFileDialog::Instance()->OpenDialog("SelectFileDlgKey", "Select File", ".txt,.dat,.*", workDir);

        if (ImGuiFileDialog::Instance()->Display("SelectFileDlgKey"))
        {
            if (ImGuiFileDialog::Instance()->IsOk())
            {
                resFilePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                // std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
                resFileName = ImGuiFileDialog::Instance()->GetCurrentFileName();

                std::string line;
                std::ifstream wf(resFilePathName);
                num_res_steps = 0;
                while (std::getline(wf, line))
                    ++num_res_steps;
                wf.close();

                wf.open(resFilePathName);
                t_data = new float[num_res_steps];
                r_data = new float[num_res_steps];
                float tmp;

                for (int i = 0; i < num_res_steps; i++)
                {
                    wf >> t_data[i];
                    for (int j = 0; j < dof_item_index*3; j++)
                        wf >> tmp;
                    if (dof_response_type == 0)
                    {
                        wf >> r_data[i];
                        wf >> tmp;
                        wf >> tmp;
                    }
                    else if (dof_response_type == 1)
                    {
                        wf >> tmp;
                        wf >> r_data[i];
                        wf >> tmp;
                    }
                    else
                    {
                        wf >> tmp;
                        wf >> tmp;
                        wf >> r_data[i];
                    }
                    for (int j = dof_item_index*3+3; j < num_dof*3; j++)
                        wf >> tmp;
                }
                wf.close();
                dof_id_ = dof_id;
            }
            ImGuiFileDialog::Instance()->Close();
        }

        ImGui::SameLine();
        ImGui::Text("Response File: %s", resFileName.c_str());

        if (ImGui::Button("Update Plot"))
        {
            std::ifstream wf;
            wf.open(resFilePathName);
            float tmp;

            for (int i = 0; i < num_res_steps; i++)
            {
                wf >> t_data[i];
                for (int j = 0; j < dof_item_index*3; j++)
                    wf >> tmp;
                if (dof_response_type == 0)
                {
                    wf >> r_data[i];
                    wf >> tmp;
                    wf >> tmp;
                }
                else if (dof_response_type == 1)
                {
                    wf >> tmp;
                    wf >> r_data[i];
                    wf >> tmp;
                }
                else
                {
                    wf >> tmp;
                    wf >> tmp;
                    wf >> r_data[i];
                }
                for (int j = dof_item_index*3+3; j < num_dof*3; j++)
                    wf >> tmp;
            }
            wf.close();
            dof_id_ = dof_id;
        }
        
        std::stringstream title;
        title << dofResponse[dof_response_type] << " response of DOF";
        std::stringstream legend;
        legend << "DOF " << dof_id_;
        if (num_res_steps > 0)
        {
            if (ImPlot::BeginPlot(title.str().c_str(), ImVec2(-1,500)))
            {
                ImPlot::SetupAxes("Time", "Response", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
                ImPlot::PlotLine(legend.str().c_str(), t_data, r_data, num_res_steps);
                ImPlot::EndPlot();
            }
        }

        if (ImGui::Button("Close"))
            show_time_history_plot_window = false;

        ImGui::End();
    }
}

void RODS_GUI::drawModeWindow()
{
    if (show_draw_mode_window)
    {
        ImGui::Begin("Drawing Mode");

        ImGui::Text("Dimension: "); ImGui::SameLine();
        ImGui::RadioButton("1D (Serial DOFs)", &draw_dim, 1); ImGui::SameLine();
        ImGui::RadioButton("1D", &draw_dim, 11); ImGui::SameLine();
        ImGui::RadioButton("2D", &draw_dim, 2); ImGui::SameLine();
        ImGui::RadioButton("3D", &draw_dim, 3);
        if (ImGui::Button("Update View"))
            updateViewMatrix();

        ImGui::Text("Draw: "); ImGui::SameLine();
        ImGui::RadioButton("Geometry", &draw_type, 0); ImGui::SameLine();
        ImGui::RadioButton("Model", &draw_type, 1);
        ImGui::RadioButton("Mode Shape", &draw_type, 2); ImGui::SameLine();
        ImGui::RadioButton("Mode Shape (Animated)", &draw_type, 22);
        ImGui::RadioButton("Static Response", &draw_type, 3);
        ImGui::RadioButton("Dynamic Response", &draw_type, 4);ImGui::SameLine();
        ImGui::RadioButton("Dynamic Response", &draw_type, 44);

        if (ImGui::Button("Close"))
            show_draw_mode_window = false;

        ImGui::End();
    }
}

void RODS_GUI::updateViewMatrix()
{
    glm::mat4 view = glm::mat4(1.0f);
    if (draw_dim == 1)
    {
        view = glm::lookAt( glm::vec3(0.0f, 0.0f, 2.5f),
                        glm::vec3(0.0f, 0.0f, 0.0f),
                        glm::vec3(0.0f, 1.0f, 0.0f) );
    }
    else if (draw_dim == 11 || draw_dim == 2)
    {
        view = glm::lookAt( glm::vec3(0.0f, -2.0f, 0.0f),
                        glm::vec3(0.0f, 0.0f, 0.0f),
                        glm::vec3(0.0f, 0.0f, 1.0f) );
    }
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
}

void RODS_GUI::draw_geo()
{
    if (draw_type == 0)
    {
        if (num_point > 0) {

            float* vertices = new float[(size_t)num_point*3];
            get_point_coord(vertices, true);

            // glUseProgram(shaderProgram);
            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, (size_t)num_point*3*sizeof(float), vertices, GL_DYNAMIC_DRAW);

            glDrawArrays(GL_POINTS, 0, num_point);

            if (num_line > 0) {

                int* indices = new int[(size_t)num_line*2];
                get_line_point_id(indices);

                for (int i = 0; i < num_line * 2; i++)
                {
                    //--indices[i];
                    indices[i] = pointIdMapIndex.at(indices[i]);
                }

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, (size_t)num_line*2*sizeof(int), indices, GL_DYNAMIC_DRAW);

                glDrawElements(GL_LINES, 2*num_line, GL_UNSIGNED_INT, (void*)0);

                delete[] indices;
            }
            delete[] vertices;
        }
    }
}

void RODS_GUI::draw_1d_s()
{
    if (draw_dim == 1)
    {
        if (num_dof > 0)
        {
            float H = 1.6f;
            float h = 0.0f;
            float H_0 = -0.8f;

            if (num_dof>1)
                h = H/(num_dof - 1);

            float* vertices = new float[(size_t)num_dof*3];

            if (draw_type == 2 || draw_type == 22)
            {
                dof_response = new double[(size_t)num_dof];
                get_dof_modal_response(dof_response, mode_order);
            }

            for (int i = 0; i < num_dof; i++)
            {
                vertices[3*i] = 0.0f;
                if (draw_type == 2) vertices[3*i] += dof_response[i];
                else if (draw_type == 22) vertices[3*i] += dof_response[i]*sinf(2.0*3.142/5.0*glfwGetTime());
                vertices[3*i+1] = H_0 + h*i;
                vertices[3*i+2] = 0.0f;
            }

            // glUseProgram(shaderProgram);
            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, (size_t)num_dof*3*sizeof(float), vertices, GL_DYNAMIC_DRAW);

            glDrawArrays(GL_POINTS, 0, num_dof);

            if (num_ele > 0) {
                updatedofIdMapIndex();
                int* indices = new int[(size_t)num_ele*2];
                get_rod1d_dof_id(indices);

                for (int i = 0; i < num_ele * 2; i++)
                {
                    indices[i] = dofIdMapIndex.at(indices[i]);
                }

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, (size_t)num_ele*2*sizeof(int), indices, GL_DYNAMIC_DRAW);

                glDrawElements(GL_LINES, 2*num_ele, GL_UNSIGNED_INT, (void*)0);

                delete[] indices;
            }
            delete[] vertices;
        }
    }
}

void RODS_GUI::draw_1d()
{
    if (draw_dim == 11)
    {
        if (num_dof > 0)
        {
            float* vertices = new float[(size_t)num_node*3];
            get_node_coords(vertices);

            if (draw_type == 2 || draw_type == 22)
            {
                node_response = new double[(size_t)num_node*3];
                get_node_modal_response(node_response, mode_order);
            }

            for (size_t i = 0; i < (size_t)num_node*3; i++)
            {
                if (draw_type == 2) vertices[i] += node_response[i]*scale_factor_dsp;
                else if (draw_type == 22) vertices[i] += node_response[i]*scale_factor_dsp*sinf(2.0*3.142/5.0*glfwGetTime());
            }

            // glUseProgram(shaderProgram);
            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, (size_t)num_dof*3*sizeof(float), vertices, GL_DYNAMIC_DRAW);

            glDrawArrays(GL_POINTS, 0, num_node);

            if (num_ele > 0) {
                updateNodeIdMapIndex();
                int* indices = new int[(size_t)num_ele*2];
                get_rod2d_node_id(indices);

                for (int i = 0; i < num_ele * 2; i++)
                {
                    indices[i] = nodeIdMapIndex.at(indices[i]);
                }

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, (size_t)num_ele*2*sizeof(int), indices, GL_DYNAMIC_DRAW);

                glDrawElements(GL_LINES, 2*num_ele, GL_UNSIGNED_INT, (void*)0);

                delete[] indices;
            }
            delete[] vertices;
        }
    }
}

void RODS_GUI::draw_2d()
{
    if (draw_dim == 2)
    {
        if (num_node > 0)
        {
            float* vertices = new float[(size_t)num_node*3];
            get_node_coords(vertices);
            float* colors = new float[(size_t)num_node*3];

            double max_res = 0.0;
            double min_res = 0.0;
            double peak_res = 1.0;

            if (draw_type == 2 || draw_type == 22)
            {
                node_response = new double[(size_t)num_node*3];
                get_node_modal_response(node_response, mode_order);
                max_res = *std::max_element(node_response, node_response+num_node*3);
                min_res = *std::min_element(node_response, node_response+num_node*3);
                peak_res = fmax(max_res, -min_res);
            }

            for (size_t i = 0; i < (size_t)num_node*3; i++)
            {
                if (draw_type == 2)
                {
                    vertices[i] += node_response[i]*scale_factor_dsp;
                    colors[i] = node_response[i]/peak_res;
                }
                else if (draw_type == 22)
                {
                    vertices[i] += node_response[i]*scale_factor_dsp*sinf(2.0*3.142/5.0*glfwGetTime());
                    colors[i] = node_response[i]/peak_res*sinf(2.0*3.142/5.0*glfwGetTime());
                }
                else
                {
                    colors[i] = vertices[i];
                }
            }

            // glUseProgram(shaderProgram);
            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO_COLOR);
            glBufferData(GL_ARRAY_BUFFER, (size_t)num_node*3*sizeof(float), colors, GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, (size_t)num_node*3*sizeof(float), vertices, GL_DYNAMIC_DRAW);

            glDrawArrays(GL_POINTS, 0, num_node);

            if (num_ele > 0) {
                updateNodeIdMapIndex();
                int* indices = new int[(size_t)num_ele*2];
                get_rod2d_node_id(indices);

                for (int i = 0; i < num_ele * 2; i++)
                {
                    indices[i] = nodeIdMapIndex.at(indices[i]);
                }

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, (size_t)num_ele*2*sizeof(int), indices, GL_DYNAMIC_DRAW);

                glDrawElements(GL_LINES, 2*num_ele, GL_UNSIGNED_INT, (void*)0);

                delete[] indices;
            }
            delete[] vertices;
        }
    }
}

void RODS_GUI::updateDOFList()
{
    num_dof = get_num_dof();
    if (num_dof > 0)
    {
        dofList = new int [num_dof];
        get_dof_id(dofList);
        dofStrList = new const char* [num_dof];
        for (int i = 0; i < num_dof; i++)
        {
            auto dof_str = new char[C_STR_LEN_S];
            snprintf(dof_str, C_STR_LEN_S, "%d", dofList[i]);
            dofStrList[i] = dof_str;
        }
    }
}

void RODS_GUI::genDofList()
{
    num_dof = get_num_dof();
    if (num_dof > 0)
    {
        dofs = new int[num_dof];
        get_ids_dof(dofs);
        dofs_str.clear();
        for (int i = 0; i < num_dof; i++)
        {
            dofs_str.append(std::to_string(dofs[i]));
            dofs_str.push_back('\0');
        }

        num_dof_x = get_num_dof_x();
        if (num_dof_x > 0)
        {
            dofs_x = new int [num_dof_x];
            get_ids_dof_x(dofs_x);
            dofs_x_str.clear();
            for (int i = 0; i < num_dof_x; i++)
            {
                dofs_x_str.append(std::to_string(dofs_x[i]));
                dofs_x_str.push_back('\0');
            }
        }

        num_dof_y = get_num_dof_y();
        if (num_dof_y > 0)
        {
            dofs_y = new int [num_dof_y];
            get_ids_dof_y(dofs_y);
            dofs_y_str.clear();
            for (int i = 0; i < num_dof_y; i++)
            {
                dofs_y_str.append(std::to_string(dofs_y[i]));
                dofs_y_str.push_back('\0');
            }
        }

        num_dof_z = get_num_dof_z();
        if (num_dof_z > 0)
        {
            dofs_z = new int [num_dof_z];
            get_ids_dof_z(dofs_z);
            dofs_z_str.clear();
            for (int i = 0; i < num_dof_z; i++)
            {
                dofs_z_str.append(std::to_string(dofs_z[i]));
                dofs_z_str.push_back('\0');
            }
        }

        num_dof_rx = get_num_dof_rx();
        if (num_dof_rx > 0)
        {
            dofs_rx = new int [num_dof_rx];
            get_ids_dof_rx(dofs_rx);
            dofs_rx_str.clear();
            for (int i = 0; i < num_dof_rx; i++)
            {
                dofs_rx_str.append(std::to_string(dofs_rx[i]));
                dofs_rx_str.push_back('\0');
            }
        }

        num_dof_ry = get_num_dof_ry();
        if (num_dof_ry > 0)
        {
            dofs_ry = new int [num_dof_ry];
            get_ids_dof_ry(dofs_ry);
            dofs_ry_str.clear();
            for (int i = 0; i < num_dof_ry; i++)
            {
                dofs_ry_str.append(std::to_string(dofs_ry[i]));
                dofs_ry_str.push_back('\0');
            }
        }

        num_dof_rz = get_num_dof_rz();
        if (num_dof_rz > 0)
        {
            dofs_rz = new int [num_dof_rz];
            get_ids_dof_rz(dofs_rz);
            dofs_rz_str.clear();
            for (int i = 0; i < num_dof_rz; i++)
            {
                dofs_rz_str.append(std::to_string(dofs_rz[i]));
                dofs_rz_str.push_back('\0');
            }
        }
    }
}

void RODS_GUI::genPointList()
{
    num_point = get_num_point();
    if (num_point > 0)
    {
        points = new int[num_point];
        get_ids_point(points);
        points_str.clear();
        for (int i = 0; i < num_point; i++)
        {
            points_str.append(std::to_string(points[i]));
            points_str.push_back('\0');
        }
    }
}

void RODS_GUI::genNodeList()
{
    num_node = get_num_node();
    if (num_node > 0)
    {
        nodes = new int[num_node];
        get_ids_node(nodes);
        nodes_str.clear();
        for (int i = 0; i < num_node; i++)
        {
            nodes_str.append(std::to_string(nodes[i]));
            nodes_str.push_back('\0');
        }
    }
}

void RODS_GUI::updateDOFRecorderList()
{
    dofRecorderList = new int [num_dof_recorder];
    get_dof_recorder_id(dofRecorderList);
    dofRecorderStrList = new const char* [num_dof_recorder];
    for (int i = 0; i < num_dof_recorder; i++)
    {
        auto dof_recorder_str = new char[C_STR_LEN_S];
        snprintf(dof_recorder_str, C_STR_LEN_S, "%d", dofRecorderList[i]);
        dofRecorderStrList[i] = dof_recorder_str;
    }
}

void RODS_GUI::genDofRecorderList()
{
    num_dof_recorder = get_num_dof_recorder();
    if (num_dof_recorder > 0)
    {
        dof_recorders = new int[num_dof_recorder];
        get_ids_dof_recorder(dof_recorders);
        dof_recorders_str.clear();
        for (int i = 0; i < num_dof_recorder; i++)
        {
            dof_recorders_str.append(std::to_string(dof_recorders[i]));
            dof_recorders_str.push_back('\0');
        }
    }
}

void RODS_GUI::updateEleRecorderList()
{
    eleRecorderList = new int [num_ele_recorder];
    get_ele_recorder_id(eleRecorderList);
    eleRecorderStrList = new const char* [num_ele_recorder];
    for (int i = 0; i < num_ele_recorder; i++)
    {
        auto ele_recorder_str = new char[C_STR_LEN_S];
        snprintf(ele_recorder_str, C_STR_LEN_S, "%d", eleRecorderList[i]);
        eleRecorderStrList[i] = ele_recorder_str;
    }
}

void RODS_GUI::genEleRecorderList()
{
    num_ele_recorder = get_num_ele_recorder();
    if (num_ele_recorder > 0)
    {
        ele_recorders = new int[num_ele_recorder];
        get_ids_ele_recorder(ele_recorders);
        ele_recorders_str.clear();
        for (int i = 0; i < num_ele_recorder; i++)
        {
            ele_recorders_str.append(std::to_string(ele_recorders[i]));
            ele_recorders_str.push_back('\0');
        }
    }
}

void RODS_GUI::updateWaveList()
{
    waveList = new int [num_wave];
    get_wave_id(waveList);
    waveStrList = new const char* [num_wave];
    for (int i = 0; i < num_wave; i++)
    {
        auto wave_str = new char[C_STR_LEN_S];
        snprintf(wave_str, C_STR_LEN_S, "%d", waveList[i]);
        waveStrList[i] = wave_str;
    }
}

void RODS_GUI::updateEleList()
{
    eleList = new int [num_ele];
    get_ele_id(eleList);
    eleStrList = new const char* [num_ele];
    for (int i = 0; i < num_ele; i++)
    {
        auto item_str = new char[C_STR_LEN_S];
        snprintf(item_str, C_STR_LEN_S, "%d", eleList[i]);
        eleStrList[i] = item_str;
    }
}

void RODS_GUI::updatedofIdMapIndex()
{
    dofList = new int [num_dof];
    get_dof_id(dofList);

    dofIdMapIndex.clear();
    for (auto index = 0; index < num_dof; index++)
    {
        dofIdMapIndex[dofList[index]] = index;
    }
}

void RODS_GUI::updateNodeIdMapIndex()
{
    genNodeList();

    nodeIdMapIndex.clear();
    for (auto index = 0; index < num_node; index++)
    {
        nodeIdMapIndex[nodes[index]] = index;
    }
}

void RODS_GUI::glDeleteAll()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
}

void RODS_GUI::sdof_model()
{
    set_name("SDOF");
    set_work_dir("D:/Develop/RODS/RODS/x64/Release");

    set_damping_ratio(0.02);
    set_rayleigh_damping(10.0, 20.0);

    add_dof_x(1, 1.0);
    num_dof = add_dof_x(2, 1.0);
    fix_dof(1);

    add_spring(1,1,2,100.0);
    add_dashpot(2,1,2,0.4);

    num_ele = get_num_ele();

    assemble_matrix();

    record_all_dof_response(1);
    record_all_ele_response(1);

    char rfn[C_STR_LEN] = "D:/Develop/RODS/RODS/x64/Release/disp.txt";
    num_dof_recorder = add_dof_recorder(2,0,rfn);
    add_dof_to_recorder(2,2);

    char wfn[C_STR_LEN] = "D:/Develop/RODS/RODS/x64/Release/EQ-S-1.txt";
    num_wave = add_wave(1, 0.005, wfn);

    set_dynamic_solver(0);
    active_ground_motion(0,1,1.0);
    solve_seismic_response();
}

void RODS_GUI::initVars()
{
    num_dof = 0;
    num_node = 0;
    num_point = 0;
    num_line = 0;
    num_ele = 0;
    num_spring = 0;
    num_dashpot = 0;
    num_inerter = 0;
    num_eqn = 0;
    num_wave = 0;
    num_dof_recorder = 0;
    num_ele_recorder = 0;

    point_index = 0;
    dof_index = 0;
}

void RODS_GUI::updateVars()
{
    num_dof = get_num_dof();
    num_node = get_num_node();
    num_point = get_num_point();
    num_line = get_num_line();
    num_ele = get_num_ele();
    num_spring = get_num_spring();
    num_dashpot = get_num_dashpot();
    num_inerter = get_num_inerter();
    num_eqn = get_num_eqn();
    num_wave = get_num_wave();
    num_dof_recorder = get_num_dof_recorder();
    num_ele_recorder = get_num_ele_recorder();
}
