#include "IndexBuffer.h"

#include <GL/glew.h>

#include "Debugger.h"

IndexBuffer::IndexBuffer(const unsigned int* indices, unsigned int count)
	:m_Count(count)
{
	GlCall(glGenBuffers(1, &m_RendererID));
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	if (m_RendererID != 0)
	{
		GlCall(glDeleteBuffers(1, &m_RendererID));
		m_RendererID = 0;
	}
}

void IndexBuffer::Bind() const
{
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

unsigned int IndexBuffer::GetCount() const
{
	return m_Count;
}
