#pragma once
#include <GL/glew.h>

enum class DrawMode
{
	Static = GL_STATIC_DRAW,
	Dynamic = GL_DYNAMIC_DRAW,
	Stream = GL_STREAM_DRAW
};

class VertexBuffer
{
private:
	unsigned int m_RendererID;

public:
	VertexBuffer(const void* data, unsigned int size, DrawMode mode = DrawMode::Static);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	void SetBufferData(const void* data, unsigned int size, DrawMode mode = DrawMode::Static) const;
};

