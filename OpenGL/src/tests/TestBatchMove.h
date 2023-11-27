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
    class TestBatchMove : public Test
    {
    public:
        TestBatchMove();
        ~TestBatchMove() override = default;

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

        static Renderer renderer;

    private:
        VertexArrayUniquePtr m_VAO;
        VertexBufferUniquePtr m_VBO;
        IndexBufferUniquePtr m_IBO;
        ShaderUniquePtr m_Shader;
        glm::vec3 transform;
    };
}
