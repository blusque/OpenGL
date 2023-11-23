#include "VertexArray.h"

#include <GL/glew.h>

#include "Debugger.h"

VertexArray::VertexArray()
	: m_RendererID(0)
{
	GlCall(glBindVertexArray(m_RendererID));
	GlCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	if (m_RendererID != 0)
	{
		GlCall(glDeleteVertexArrays(1, &m_RendererID));
	}
}

void VertexArray::AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout) const
{
	Bind();
	vbo.Bind();
	auto offset = unsigned int(0);
	auto stride = layout.GetStride();
	const auto& elements = layout.GetElements();
	for (unsigned int i = 0; i < elements.size(); ++i)
	{
		const auto& element = elements[i];
		GlCall(glEnableVertexAttribArray(i));
		GlCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, stride, (const void*)offset));
		offset += element.count * VertexBufferElements::GetSizeofType(element.type);
	}
}

void VertexArray::Bind() const
{
	GlCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GlCall(glBindVertexArray(0));
}
