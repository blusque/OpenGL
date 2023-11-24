#pragma once
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#include "vendor/glm/glm.hpp"

class Renderer
{
public:
	void Clear(const glm::vec4& color = glm::vec4(0.0, 0.0, 0.0, 1.0)) const;
	void Draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) const;
};
