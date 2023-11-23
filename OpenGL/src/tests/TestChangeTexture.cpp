#include "TestChangeTexture.h"

#include <iostream>

#include "VertexBufferLayout.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/constants.hpp"
#include "vendor/imgui/imgui.h"

test::TestChangeTexture::TestChangeTexture()
    : index{0, 1, 2,
            2, 3, 0}
    , box {
        -0.5f, -0.5f, 0.f, 0.f,
         0.5f, -0.5f, 1.f, 0.f,
         0.5f,  0.5f, 1.f, 1.f,
        -0.5f,  0.5f, 0.f, 1.f
    }
    , vao(VertexArray())
    , vbo(VertexBuffer(box, 4 * 4 * sizeof(float)))
    , ibo(IndexBuffer(index, 6))
    , shader(Shader("F:/vs/TheCherno/OpenGL/res/shaders/Basic.glsl"))
    , renderer(Renderer())
    , tex(Texture("F:/vs/TheCherno/OpenGL/res/textures/ChernoLogo.png"))
    , transform(glm::vec3(0.f, 0.f, 0.f))
{
    GlCall(glEnable(GL_BLEND));
    GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    
    auto layout = VertexBufferLayout();
    layout.Push<float>(2);
    layout.Push<float>(2);

    vao.AddBuffer(vbo, layout);

    tex.Bind();
    shader.SetUniform1i("u_Texture", 0);
}

void test::TestChangeTexture::OnUpdate(float deltaTime)
{
    auto trans = transform;
    trans[0] = trans[0] / 480.f;
    trans[1] = trans[1] / 320.f;
    auto model = scale(glm::mat4(1.f), glm::vec3(300.f, 300.f, 300.f));
    auto view = glm::mat4(1.f);
    auto projection = glm::ortho(-480.f, 480.f, -320.f, 320.f, -1.f, 1.f);
    model = translate(model, trans);
    auto mvp = projection * view * model;
    shader.SetUniformMat4f("u_MVP", mvp);
}

void test::TestChangeTexture::OnRender()
{
    GlCall(glClearColor(0.2f, 0.3f, 0.8f, 1.f));
    GlCall(glClear(GL_COLOR_BUFFER_BIT));
    renderer.Draw(vao, ibo, shader);
}

void test::TestChangeTexture::OnImGuiRender()
{
    ImGui::Button("Open Texture...");
    ImGui::SliderFloat2("Transform", &transform[0], -480, 480);
}
