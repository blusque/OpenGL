#pragma once

#include <string>

class Texture
{
public:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width;
	int m_Height;
	int m_BPP;

public:
	Texture(const std::string& filepath);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	unsigned int GetTextureWidth() const
	{
		return m_Width;
	}

	unsigned int GetTextureHeight() const
	{
		return m_Height;
	}
};

