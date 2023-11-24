#pragma once
#include <memory>

class IndexBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_Count;

public:
	IndexBuffer(const unsigned int* index, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	unsigned int GetCount() const;
};

using IndexBufferPtr = std::shared_ptr<IndexBuffer>;
using IndexBufferUniquePtr = std::shared_ptr<IndexBuffer>;
