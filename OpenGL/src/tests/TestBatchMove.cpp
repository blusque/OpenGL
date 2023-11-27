#include "TestBatchMove.h"

#include <iostream>

#include "VertexBufferLayout.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/constants.hpp"
#include "vendor/imgui/imgui.h"

Renderer test::TestBatchMove::renderer = Renderer();

test::TestBatchMove::TestBatchMove()
    : transform(glm::vec3(0.f, 0.f, 0.f))
{
    m_Shader = std::make_unique<Shader>("F:/vs/TheCherno/OpenGL/res/shaders/BatchMove.glsl");
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0,

        4, 5, 6,
        6, 7, 4
    };
    m_IBO = std::make_unique<IndexBuffer>(indices, 4 * 3);
    
    float box[] = {
        -1.5f, -0.5f, 0.f,
        -0.5f, -0.5f, 0.f,
        -0.5f,  0.5f, 0.f,
        -1.5f,  0.5f, 0.f,

         0.5f, -0.5f, 0.f,
         1.5f, -0.5f, 0.f,
         1.5f,  0.5f, 0.f,
         0.5f,  0.5f, 0.f,
    };
    m_VBO = std::make_unique<VertexBuffer>(box, 8 * 4 * sizeof(float), DrawMode::Dynamic);
    
    GlCall(glEnable(GL_BLEND));
    GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    
    auto layout = VertexBufferLayout();
    layout.Push<float>(3);

    m_VAO = std::make_unique<VertexArray>();
    m_VAO->AddBuffer(*m_VBO, layout);

    m_Shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.2f, 1.0f);
}

void test::TestBatchMove::OnUpdate(float deltaTime)
{
    auto trans = transform;
    trans[0] = trans[0] / 480.f;
    trans[1] = trans[1] / 320.f;
    auto model = scale(glm::mat4(1.f), glm::vec3(300.f, 300.f, 300.f));
    // auto model = glm::mat4(1.f);
    auto view = glm::mat4(1.f);
    // auto projection = glm::perspective(glm::radians(45.0f), 1.5f, -1.f, 100.0f);
    auto projection = glm::ortho(-480.f, 480.f, -320.f, 320.f, -1.f, 1.f);
    model = translate(model, trans);
    auto mvp = projection * view * model;
    m_Shader->SetUniformMat4f("u_MVP", mvp);
}

void test::TestBatchMove::OnRender()
{
    GlCall(glClearColor(0.2f, 0.2f, 0.2f, 1.f));
    GlCall(glClear(GL_COLOR_BUFFER_BIT));
    renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
}

void test::TestBatchMove::OnImGuiRender()
{
    ImGui::Button("Open Texture...");
    ImGui::SliderFloat2("Transform", &transform[0], -480, 480);
}
