#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "vendor/glm/glm.hpp"

struct ShaderProgramSource
{
	std::string VertexShaderSource;
	std::string FragmentShaderSource;
};

class Shader
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	std::unordered_map<std::string, int> m_ULocationMap;
	
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name, const glm::mat4& mat);

	int GetUniformLocation(const std::string& name);

private:
	ShaderProgramSource ParserShader() const;
	unsigned int CompileShader(unsigned int type, const std::string& shaderString) const;
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
};

using ShaderPtr = std::shared_ptr<Shader>;
using ShaderUniquePtr = std::shared_ptr<Shader>;
