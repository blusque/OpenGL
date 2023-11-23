#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <utility>

#include "Debugger.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "Shader.h"

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
// Imgui includes
#include "tests/TestChangeTexture.h"
#include "tests/TestClearColor.h"
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(auto_inline(, "legacy_studio_definitions")
#endif

static void glfw_error_callback(int error, const char* description)
{
    std::fprintf(stderr, "GLFW Error %d: %s", error, description);
}

#define K1 1000.f
#define K2 1000.f
#define M 2.f
#define C 100.f
#define D_T 0.017f
#define BASE_D_T 0.001f
constexpr auto RATIO = BASE_D_T / D_T;
#define SHADER_PATH "F:/vs/TheCherno/OpenGL/res/shaders/Basic.glsl"

Renderer renderer;

auto dist(float* points, uint32_t i1, uint32_t i2)
{
    auto result = std::sqrt(std::pow(points[2 * i1] - points[2 * i2], 2.f) +
                            std::pow(points[2 * i1 + 1] - points[2 * i2 + 1], 2.f));
    return result;
}

auto compute_constraint(float* points, unsigned int size, float *length, unsigned int now)
{
    auto constraints = std::vector<float>(2, 0.f);
    for (unsigned int i = 0; i < size; i++)
    {
        if (i != now)
        {
            auto index = now * size + i;
            auto displacement = dist(points, now, i);
            auto delta = length[index] - displacement;
            auto cos_theta = (points[2 * i] - points[2 * now]) / displacement;
            auto sin_theta = (points[2 * i + 1] - points[2 * now + 1]) / displacement;
            if (std::abs(length[index] - 1.f) < 1e-6f)
            {
                constraints[0] += K1 * delta * cos_theta;
                constraints[1] += K1 * delta * sin_theta;
            }
            else
            {
                constraints[0] += K2 * delta * cos_theta;
                constraints[1] += K2 * delta * sin_theta;
            }
        }
    }
    // std::cout << "constraints: (" << constraints[0] << ", " << constraints[1] << ");";
    return constraints;
}

auto compute_obstruct(float* points, unsigned int size, float* velocities, unsigned int now)
{
    auto obstructs = std::vector<float>(2, 0.f);
    for (unsigned int i = 0; i < size; i++)
    {
        if (i != now)
        {
			auto rvelocity_x = velocities[2 * now] - velocities[2 * i];
            auto rvelocity_y = velocities[2 * now + 1] - velocities[2 * i + 1];
            auto displacement = dist(points, now, i);
            auto cos_theta = (points[2 * i] - points[2 * now]) / displacement;
            auto sin_theta = (points[2 * i + 1] - points[2 * now + 1]) / displacement;
            obstructs[0] += RATIO * C * (rvelocity_x * cos_theta + rvelocity_y * sin_theta) * cos_theta;
            obstructs[1] += RATIO * C * (rvelocity_x * cos_theta + rvelocity_y * sin_theta) * sin_theta;
        }
    }
    return obstructs;
}

auto cloth_update(float* positions, float* velocities, unsigned int size, float* length,
                  unsigned int fixed, bool gravity = true)
{
    float accelerate[] = { 0.f, 0.f };
    auto new_velocities = new float[size * 2]; // uninitialized, now an arbitrary value
    auto new_positions = new float[size * 2];  // uninitialized, now an arbitrary value
    for (unsigned int i = 0; i < size; i++)
    {
        if (i != fixed)
        {
			// compute constraint and obstruct
            auto constraints = compute_constraint(positions, size, 
                                                  length, i);
            auto obstructs = compute_obstruct(positions, size, 
                                              velocities, i);

			// update_accelerate
            accelerate[0] = (-constraints[0] - obstructs[0]) / M;
            accelerate[1] = (-constraints[1] - obstructs[1]) / M - 9.8f;
            // std::cout << "accelerate: (" << accelerate[0] << ", " << accelerate[1] << ");\n";

			// update_velocity
            new_velocities[2 * i] = velocities[2 * i] + D_T * accelerate[0];
            new_velocities[2 * i + 1] = velocities[2 * i + 1] + D_T * accelerate[1];

			// update position
            new_positions[2 * i] = positions[2 * i] + D_T * new_velocities[2 * i];
            new_positions[2 * i + 1] = positions[2 * i + 1] + D_T * new_velocities[2 * i + 1];
        }
        else
        {
            new_velocities[2 * i] = velocities[2 * i];
            new_velocities[2 * i + 1] = velocities[2 * i + 1];
            new_positions[2 * i] = positions[2 * i];
            new_positions[2 * i + 1] = positions[2 * i + 1];
        }
    }
    std::memcpy(positions, new_positions, sizeof(float) * size * 2);
    std::memcpy(velocities, new_velocities, sizeof(float) * size * 2);
    delete[] new_positions;
    delete[] new_velocities;
    new_positions = nullptr;
    new_velocities = nullptr;
}

int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    /* Create a windowed mode window and its OpenGL context */
    auto window = glfwCreateWindow(960, 640, "Hello World", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed To Creat Window!" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);
    
    auto err = glewInit();
    if (err != GLEW_OK)
    {
        std::cout << "glew init error!\n";
        return -1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto io = ImGui::GetIO();
    (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    
    std::cout << "GL version: " << glGetString(GL_VERSION) << std::endl;
    {
        // auto Test = test::TestClearColor();
        auto Test = test::TestMenu();

        Test.Register<test::TestClearColor>("Clear Color");
        Test.Register<test::TestChangeTexture>("Change Texture");
        
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
            Test.OnUpdate(0.0f);
            
            /* Render here */
            Test.OnRender();
            
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            
            Test.OnImGuiRender();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
        }
    }
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}