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

int * pointList = nullptr;
int * dofList = nullptr;
const char* *dofStrList = nullptr;

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
            if (ImGui::MenuItem("Exit")) {
                glfwSetWindowShouldClose(window, true);
            }
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

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Info"))
                show_basic_info_window = true;
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
        static float zeta = 0.05;
        ImGui::InputFloat("Damping Ratio", &zeta);
        if (ImGui::Button("Set Damping Ratio"))
            set_damping_ratio(zeta);
        static int rayleigh_damping_modes[2] = { 1, 2 };
        ImGui::InputInt2("Order of Modes", rayleigh_damping_modes);
        if (ImGui::Button("Use Rayleigh Damping"))
            set_rayleigh_damping(rayleigh_damping_modes[0], rayleigh_damping_modes[1]);
        static int num_modes = 1;
        ImGui::InputInt("Number of Modes", &num_modes);
        if (ImGui::Button("Use Mode Orthogonal Damping"))
            set_num_modes_inherent_damping(num_modes);
        
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
                static int dof_item_index = 0;
                ImGui::Combo("DOF", &dof_item_index, dofStrList, num_dof);
                int selected_dof_id = dofList[dof_item_index];
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
        ImGui::Text("Number of DOFs: %d", num_dof);
        ImGui::Text("Number of Points: %d", num_point);
        ImGui::Text("Number of Lines: %d", num_line);
        ImGui::Text("Number of Elements: %d", num_ele);
        ImGui::Text("Use RayleighDamping: %s", get_use_rayleigh_damping() ? "True" : "False");
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
            static int point_item_index_i = 0;
            static int point_item_index_j = 0;
            ImGui::Combo("Point I", &point_item_index_i, pointItems, num_point);
            ImGui::Combo("Point J", &point_item_index_j, pointItems, num_point);
            p_id_i = pointList[point_item_index_i];
            p_id_j = pointList[point_item_index_j];
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
            ImGui::Text("Points are duplicated!");
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
            wave_t_data = new float[num_wave_steps];
            if (dt > 0.0)
            {
                for (int i = 0; i < num_wave_steps; i++)
                {
                    wave_t_data[i] = dt*i;
                }
                if (ImPlot::BeginPlot("Wave")) {
                    ImPlot::SetupAxes("Time", "Value", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
                    ImPlot::PlotLine(waveFileName.c_str(), wave_t_data, wave_a_data, num_wave_steps);
                    ImPlot::EndPlot();
                }
            }
        }
        if (ImGui::Button("Add Wave")) {
            char * waveFilePathName_ = &waveFilePathName[0];
            add_wave(wave_id, dt, waveFilePathName_);
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
        switch (ele_type)
        {
        case 0:
            
            break;
        
        default:
            break;
        }
        ImGui::Button("Add Element");
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
        auto item_str = new char[20];
        snprintf(item_str, 20, "%d", dofList[i]);
        dofStrList[i] = item_str;
    }
}
