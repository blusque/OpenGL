#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
private:
	unsigned int m_RendererID;

public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout) const;

	void Bind() const;
	void Unbind() const;
};

using VertexArrayPtr = std::shared_ptr<VertexArray>;
using VertexArrayUniquePtr = std::shared_ptr<VertexArray>;
