#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include "ImGuiFileDialog.h"

#include <stdio.h>
#include <fstream>
#include <map>

#define GL_SILENCE_DEPRECATION
#if defined(__arm__)
#define IMGUI_IMPL_OPENGL_ES2
#endif
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "RODS.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif
#include <math.h>

std::map<int, int> PointIdMapOrder;
static int point_order = 0;

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

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
    "   FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
    "}\n\0";

// Main code
int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.3 + GLSL 330
    const char* glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "RODS", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    io.Fonts->AddFontFromFileTTF("./resource/FiraSans-Regular.ttf", 36.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD!\n");
        return -1;
    }

    glPointSize(10);

    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // delete shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    unsigned int VBO, VAO, EBO;
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

    float* wave_t_data = nullptr;
    float* wave_a_data = nullptr;

    static int num_dof = 0;
    static int num_point = 0;
    static int num_line = 0;

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // ImGui::ShowDemoWindow();

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
        ImGui::End();

        ImGui::Begin("Point");
        static int pt_id = 1;
        ImGui::InputInt("Point ID", &pt_id);
        static float coord[3] = {0.0, 0.0, 0.0};
        ImGui::InputFloat3("Coords (X,Y,Z)", coord);
        if (ImGui::Button("Add Point")) {
            if (PointIdMapOrder.count(pt_id) == 0)
            {
                PointIdMapOrder[pt_id] = point_order;
                point_order++;
            }
            num_point = add_point(pt_id++, coord[0], coord[1], coord[2]);
        }
        ImGui::End();

        int *pointList;
        ImGui::Begin("Line");
        static int l_id = 1;
        static int p_id_i = 0;
        static int p_id_j = 0;
        ImGui::InputInt("Line ID", &l_id);
        if (ImGui::Button("Choose Points"))
            ImGui::OpenPopup("Choose Points for Line");
        if (ImGui::BeginPopup("Choose Points for Line"))
        {
            pointList = new int [num_point];
            get_point_id(pointList);
            const char** pointItems = new const char* [num_point];
            for (size_t i = 0; i < num_point; i++)
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
        ImGui::Text("Choosed Point I: %d, Point J: %d", p_id_i, p_id_j);

        if (ImGui::Button("Add Line")) {
            if (p_id_i == p_id_j)
                ImGui::OpenPopup("Error");
            else num_line = add_line(l_id++, p_id_i, p_id_j);
        }

        if (ImGui::BeginPopupModal("Error"))
        {
            ImGui::Text("Points are duplicated!");
            if (ImGui::Button("Confirm"))
                ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }

        ImGui::End();

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
        ImGui::End();

        ImGui::Begin("Element1D");
        static int ele_id = 1;
        ImGui::InputInt("Element ID", &ele_id);
        static int ele_type = 0;
        const char * Element1DTypes[3] = {"Spring1D", "Dashpot1D", "Inerter1D"};
        ImGui::Combo("Element Type", &ele_type, Element1DTypes, 3);
        ImGui::Button("Add Element");
        ImGui::End();

        ImGui::Begin("Basic Information");
        ImGui::Text("RODS");
        ImGui::Text("Inherent Damping Ratio: %.3f", get_damping_ratio());
        ImGui::Text("Number of DOFs: %d", num_dof);
        ImGui::Text("Number of Points: %d", num_point);
        ImGui::Text("Number of Lines: %d", num_line);
        ImGui::Text("Number of Elements: %d", get_num_ele());
        ImGui::Text("Use RayleighDamping: %s", get_use_rayleigh_damping() ? "True" : "False");
        ImGui::End();

        ImGui::Begin("Wave");
        static int wave_id = 1;
        ImGui::InputInt("Wave ID", &wave_id);
        static double dt = 0.02;
        ImGui::InputDouble("Time Interval", &dt);
        static int num_wave_steps;
        static std::string waveFilePathName;
        static std::string waveFileName;

        // open Dialog Simple
        if (ImGui::Button("Choose File"))
            ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".txt,.dat,.*", ".");

        // display
        if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
        {
            // action if OK
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

                for (size_t i = 0; i < num_wave_steps; i++)
                {
                    wf >> wave_a_data[i];
                }
                wf.close();
            }
            // close
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
                for (size_t i = 0; i < num_wave_steps; i++)
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

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        if (num_point > 0) {

            float* vertices = new float[(size_t)num_point*3];
            get_point_coord(vertices, true);

            glUseProgram(shaderProgram);
            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, (size_t)num_point*3*sizeof(float), vertices, GL_DYNAMIC_DRAW);

            glDrawArrays(GL_POINTS, 0, num_point);

            if (num_line > 0) {

                int* indices = new int[(size_t)num_line*2];
                get_line_point_id(indices);

                for (size_t i = 0; i < (size_t)num_line * 2; i++)
                {
                    //--indices[i];
                    indices[i] = PointIdMapOrder[indices[i]];
                }

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, (size_t)num_line*2*sizeof(int), indices, GL_DYNAMIC_DRAW);

                glDrawElements(GL_LINES, 2*num_line, GL_UNSIGNED_INT, (void*)0);
                
                delete[] indices;
            }
            delete[] vertices;
        }
        
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    delete_model();

    return 0;
}
