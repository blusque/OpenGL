#include "Renderer.h"

void Renderer::Clear(const glm::vec4& color) const
{
	GlCall(glClearColor(color.x, color.y, color.z, color.w));
	GlCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) const
{
	shader.Bind();
	vao.Bind();
	ibo.Bind();
	GlCall(glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr));
}
