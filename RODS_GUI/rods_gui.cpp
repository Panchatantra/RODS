#include <stdio.h>
#include <glad/glad.h>
#include "imgui.h"
#include "implot.h"
#include "ImGuiFileDialog.h"

#include "rods_gui.h"
#include "rods.h"

#include <map>
std::map<int, int> PointIdMapIndex;

float * wave_t_data = nullptr;
float * wave_a_data = nullptr;

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

const char* direction[6] = {"X", "Y", "Z", "RX", "RY", "RZ"};
const char* dofResponse[3] = {"Displacement", "Velocity", "Acceleration"};
const char* eleResponse[3] = {"Force", "Deformation", "Force and Deformation"};
const char* dynamicSolver[4] = {"Newmark", "Newmark_NL",
                                "StateSpace", "StateSpace_NL"};

void RODS_GUI::createShader(unsigned int &shaderProgram)
{
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
        "}\0";

    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(0.5f, 0.0f, 0.0f, 1.0f);\n"
        "}\n\0";

    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
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

void RODS_GUI::buildVertex(unsigned int &VBO, unsigned int &VAO, unsigned int &EBO)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void RODS_GUI::mainMenu(GLFWwindow* window)
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New")) {}
            if (ImGui::MenuItem("Open")) {}
            if (ImGui::MenuItem("Save")) {}
            if (ImGui::MenuItem("Exit"))
                glfwSetWindowShouldClose(window, true);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Set"))
        {
            if (ImGui::MenuItem("Inherent Damping"))
                show_damping_window = true;
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("DOF"))
                show_dof_window = true;

            if (ImGui::MenuItem("Point"))
                show_point_window = true;

            if (ImGui::MenuItem("Line"))
                show_line_window = true;

            if (ImGui::MenuItem("Element1D"))
                show_element1d_window = true;

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
            num_dof = add_dof(dof_id++, dir, mass);

        if (num_dof > 0) {
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
                    num_dof = remove_dof(selected_dof_id);
                ImGui::EndPopup();
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
    if (show_point_window) {
        ImGui::Begin("Point");
        static int pt_id = 1;
        ImGui::InputInt("Point ID", &pt_id);
        static float coord[3] = {0.0, 0.0, 0.0};
        ImGui::InputFloat3("Coords (X,Y,Z)", coord);
        if (ImGui::Button("Add Point")) {
            if (PointIdMapIndex.count(pt_id) == 0)
            {
                PointIdMapIndex[pt_id] = point_index;
                point_index++;
            }
            num_point = add_point(pt_id++, coord[0], coord[1], coord[2]);
        }
        ImGui::End();
    }
}

void RODS_GUI::basicInfoWindow()
{
    if (show_basic_info_window)
    {
        ImGui::Begin("Basic Information");
        ImGui::Text("RODS");
        ImGui::Text("Inherent Damping Ratio: %.3f", get_damping_ratio());
        ImGui::Text("Use RayleighDamping: %s", get_use_rayleigh_damping() ? "True" : "False");
        ImGui::Separator();
        ImGui::Text("Number of Points: %d", num_point);
        ImGui::Text("Number of Lines: %d", num_line);
        ImGui::Separator();
        ImGui::Text("Number of DOFs: %d", num_dof);
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
                auto item_str = new char[20];
                snprintf(item_str, 20, "%d", pointList[i]);
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

        if (ImGui::Button("Select File"))
            ImGuiFileDialog::Instance()->OpenDialog("SelectFileDlgKey", "Select File", ".txt,.dat,.*", ".");

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
                if (ImPlot::BeginPlot("Wave")) {
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
        const char * Element1DTypes[3] = {"Spring1D", "Dashpot1D", "Inerter1D"};
        ImGui::Combo("Element Type", &ele_type, Element1DTypes, 3);

        static int dof_index_i = 0;
        static int dof_index_j = 0;
        static double param[3] = {0.0, 0.0, 0.0};

        static int dof_id_i = 0;
        static int dof_id_j = 0;

        if (num_dof > 1) {
            if (ImGui::Button("Select DOFs"))
                ImGui::OpenPopup("Select DOFs for Element1D");
            if (ImGui::BeginPopup("Select DOFs for Element1D"))
            {
                updateDOFList();
                ImGui::Combo("dof I", &dof_index_i, dofStrList, num_dof);
                ImGui::Combo("dof J", &dof_index_j, dofStrList, num_dof);
                dof_id_i = dofList[dof_index_i];
                dof_id_j = dofList[dof_index_j];
                ImGui::EndPopup();
            }
            ImGui::Text("Selected DOFs: I: %d, J: %d", dof_id_i, dof_id_j);

            switch (ele_type)
            {
            case 0:
            {
                ImGui::InputDouble("Stiffness", param);
                if (ImGui::Button("Add Element1D")) {
                    if (dof_id_i == dof_id_j)
                        ImGui::OpenPopup("Error");
                    else
                        num_spring = add_spring(ele_id++, dofList[dof_index_i], dofList[dof_index_j], param[0]);
                        num_ele = get_num_ele();
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
                        num_dashpot = add_dashpot(ele_id++, dofList[dof_index_i], dofList[dof_index_j], param[0]);
                        num_ele = get_num_ele();
                }
            }
                break;
            case 2:
            {
                updateDOFList();
                ImGui::InputDouble("Inertance", param);
                if (ImGui::Button("Add Element1D")) {
                    if (dof_id_i == dof_id_j)
                        ImGui::OpenPopup("Error");
                    else
                        num_inerter = add_inerter(ele_id++, dofList[dof_index_i], dofList[dof_index_j], param[0]);
                        num_ele = get_num_ele();
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
        }

        ImGui::SameLine();
        if (ImGui::Button("Close"))
            show_element1d_window = false;

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
            if (num_dof>0 && num_ele>0)
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
                    ImGui::Text("%.3f", period[row]);
                }
                ImGui::EndTable();
            }
        }

        if (ImGui::Button("Close"))
            show_solve_eigen_window = false;

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
            static int dof_recorder_id = 1;
            ImGui::InputInt("Recorder ID", &dof_recorder_id);
            static int response_type = 0;
            ImGui::Combo("Response Type", &response_type, dofResponse, 3);
            if (ImGui::Button("Set File for Recorder"))
                ImGuiFileDialog::Instance()->OpenDialog("RecorderFileDlgKey", "Select File Path",
                                ".*", ".", "", 1, nullptr, ImGuiFileDialogFlags_ConfirmOverwrite);
            static std::string recorderFilePathName = "";
            static char _recorderFilePathName[100] = "r.txt";
            ImGui::InputText("Recorder File", _recorderFilePathName, 100);
            if (ImGuiFileDialog::Instance()->Display("RecorderFileDlgKey"))
            {
                if (ImGuiFileDialog::Instance()->IsOk())
                {
                    recorderFilePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                    strcpy_s(_recorderFilePathName, recorderFilePathName.c_str());
                }
                ImGuiFileDialog::Instance()->Close();
            }
            if (ImGui::Button("Add DOF Recorder"))
                num_dof_recorder = add_dof_recorder(dof_recorder_id++, response_type, _recorderFilePathName);

            if (num_dof_recorder > 0 && num_dof > 0)
            {
                ImGui::Separator();

                if (ImGui::Button("Select DOF"))
                    ImGui::OpenPopup("Select DOF");

                if (ImGui::BeginPopup("Select DOF"))
                {
                    updateDOFList();
                    updateDOFRecorderList();
                    static int dof_index = 0;
                    static int dof_recorder_index = 0;
                    ImGui::Combo("DOF", &dof_index, dofStrList, num_dof);
                    ImGui::Combo("DOF Recorder", &dof_recorder_index, dofRecorderStrList, num_dof_recorder);
                    if (ImGui::Button("Add"))
                    {
                        add_dof_to_recorder(dofList[dof_index], dofRecorderList[dof_recorder_index]);
                        ImGui::SameLine();
                        ImGui::Text("DOF %d is added to Recorder %d.",
                                    dofList[dof_index], dofRecorderList[dof_recorder_index]);
                    }
                    ImGui::EndPopup();
                }
            }
        }
        else
        {
            static int ele_recorder_id = 1;
            ImGui::InputInt("Recorder ID", &ele_recorder_id);
            static int response_type = 0;
            ImGui::Combo("Response Type", &response_type, eleResponse, 3);
            if (ImGui::Button("Set File for Recorder"))
                ImGuiFileDialog::Instance()->OpenDialog("RecorderFileDlgKey", "Select File Path",
                                ".*", ".", "", 1, nullptr, ImGuiFileDialogFlags_ConfirmOverwrite);
            static std::string recorderFilePathName = "";
            static char _recorderFilePathName[100] = "r.txt";
            ImGui::InputText("Recorder File", _recorderFilePathName, 100);
            if (ImGuiFileDialog::Instance()->Display("RecorderFileDlgKey"))
            {
                if (ImGuiFileDialog::Instance()->IsOk())
                {
                    recorderFilePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                    strcpy_s(_recorderFilePathName, recorderFilePathName.c_str());
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

        if (ImGui::Button("Select Wave") && num_wave>0)
            ImGui::OpenPopup("Select Wave");

        if (ImGui::BeginPopup("Select Wave"))
        {
            updateWaveList();
            static int wave_index = 0;
            ImGui::Combo("Wave", &wave_index, waveStrList, num_wave);

            ImGui::EndPopup();
        }

        static int num_sub_steps = 1;
        ImGui::InputInt("Number of Substeps", &num_sub_steps);

        if (ImGui::Button("Solve Seismic Respones"))
            solve_seismic_response(num_sub_steps);

        ImGui::SameLine();
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

void RODS_GUI::draw(unsigned int VBO, unsigned int VAO, unsigned int EBO)
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
                indices[i] = PointIdMapIndex.at(indices[i]);
            }

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, (size_t)num_line*2*sizeof(int), indices, GL_DYNAMIC_DRAW);

            glDrawElements(GL_LINES, 2*num_line, GL_UNSIGNED_INT, (void*)0);

            delete[] indices;
        }
        delete[] vertices;
    }
}

void RODS_GUI::updateDOFList()
{
    dofList = new int [num_dof];
    get_dof_id(dofList);
    dofStrList = new const char* [num_dof];
    for (int i = 0; i < num_dof; i++)
    {
        auto dof_str = new char[20];
        snprintf(dof_str, 20, "%d", dofList[i]);
        dofStrList[i] = dof_str;
    }
}

void RODS_GUI::updateDOFRecorderList()
{
    dofRecorderList = new int [num_dof_recorder];
    get_dof_recorder_id(dofRecorderList);
    dofRecorderStrList = new const char* [num_dof_recorder];
    for (int i = 0; i < num_dof_recorder; i++)
    {
        auto dof_recorder_str = new char[20];
        snprintf(dof_recorder_str, 20, "%d", dofRecorderList[i]);
        dofRecorderStrList[i] = dof_recorder_str;
    }
}

void RODS_GUI::updateEleRecorderList()
{
    eleRecorderList = new int [num_ele_recorder];
    get_ele_recorder_id(eleRecorderList);
    eleRecorderStrList = new const char* [num_ele_recorder];
    for (int i = 0; i < num_ele_recorder; i++)
    {
        auto ele_recorder_str = new char[20];
        snprintf(ele_recorder_str, 20, "%d", eleRecorderList[i]);
        eleRecorderStrList[i] = ele_recorder_str;
    }
}

void RODS_GUI::updateWaveList()
{
    waveList = new int [num_wave];
    get_wave_id(waveList);
    waveStrList = new const char* [num_wave];
    for (int i = 0; i < num_wave; i++)
    {
        auto wave_str = new char[20];
        snprintf(wave_str, 20, "%d", waveList[i]);
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
        auto item_str = new char[20];
        snprintf(item_str, 20, "%d", eleList[i]);
        eleStrList[i] = item_str;
    }
}