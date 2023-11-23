#include "VertexBuffer.h"

#include <GL/glew.h>

#include "Debugger.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size, DrawMode mode)
	: m_RendererID(0)
{
	ASSERT(sizeof(GLfloat) == sizeof(float));

	GlCall(glGenBuffers(1, &m_RendererID));
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	GlCall(glBufferData(GL_ARRAY_BUFFER, size, data, static_cast<unsigned int>(mode)));
}

VertexBuffer::~VertexBuffer()
{
	if (m_RendererID != 0)
	{
		GlCall(glDeleteBuffers(1, &m_RendererID));
	}
}

void VertexBuffer::Bind() const
{
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::SetBufferData(const void* data, unsigned int size, DrawMode mode) const
{
	Bind();
	GlCall(glBufferData(GL_ARRAY_BUFFER, size, data, static_cast<unsigned int>(mode)));
}
