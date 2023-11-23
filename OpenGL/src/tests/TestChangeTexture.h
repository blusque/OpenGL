#pragma once

#include "Test.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"
#include "vendor/glm/glm.hpp"

namespace test
{
    class TestChangeTexture : public Test
    {
    public:
        TestChangeTexture();
        ~TestChangeTexture() override = default;

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        unsigned int index[6];
        float box[16]; 
        VertexArray vao;
        VertexBuffer vbo;
        IndexBuffer ibo;
        Shader shader;
        Renderer renderer;
        Texture tex;
        glm::vec3 transform;
    };
}
