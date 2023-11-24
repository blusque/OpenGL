#pragma once

#include <memory>
#include <vector>

#include <GL/glew.h>

#include "Debugger.h"

struct VertexBufferElements
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	inline static unsigned int GetSizeofType(unsigned int t)
	{
		if (t == GL_FLOAT)
		{
			return 4;
		}
		else if (t == GL_UNSIGNED_INT)
		{
			return 4;
		}
		else if (t == GL_UNSIGNED_BYTE)
		{
			return 1;
		}
		else
		{
			ASSERT(false);
			return 0;
		}
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElements> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout()
		: m_Stride(0) { }
	~VertexBufferLayout() = default;

	template <typename T>
	void Push(unsigned int count)
	{
		ASSERT(false);
	}

	template <>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferElements::GetSizeofType(GL_FLOAT);
	}

	template <>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * VertexBufferElements::GetSizeofType(GL_UNSIGNED_INT);
	}

	template <>
	void Push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += count * VertexBufferElements::GetSizeofType(GL_UNSIGNED_BYTE);
	}

	inline auto GetElements() const
	{
		return m_Elements;
	}

	inline auto GetStride() const
	{
		return m_Stride;
	}
};

using VertexBufferLayoutPtr = std::shared_ptr<VertexBufferLayout>;
using VertexBufferLayoutUniquePtr = std::shared_ptr<VertexBufferLayout>;

