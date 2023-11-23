#include "Shader.h"

#include <GL/glew.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Debugger.h"
#include "Renderer.h"
#include "Renderer.h"
#include "Renderer.h"
#include "Renderer.h"

Shader::Shader(const std::string& filepath)
	: m_FilePath(filepath)
	, m_RendererID(0)
{
	auto ShaderSources = ParserShader();
	// std::cout << "Vertex:\n" << ShaderSources.VertexShaderSource << std::endl;
	// std::cout << "Fragment:\n" << ShaderSources.FragmentShaderSource << std::endl;
	m_RendererID = CreateShader(ShaderSources.VertexShaderSource, ShaderSources.FragmentShaderSource);
	GlCall(glUseProgram(m_RendererID));
}

Shader::~Shader()
{
	GlCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
	GlCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GlCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	auto location = GetUniformLocation(name);
	GlCall(glUniform1i(location, value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
	auto location = GetUniformLocation(name);
	GlCall(glUniform1f(location, value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	auto location = GetUniformLocation(name);
	GlCall(glUniform4f(location, v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& mat)
{
	auto location = GetUniformLocation(name);
	GlCall(glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]));
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_ULocationMap.find(name) == m_ULocationMap.end())
	{
		GlCall(m_ULocationMap[name] = glGetUniformLocation(m_RendererID, name.c_str()));
		auto location = m_ULocationMap[name];
		if (location == -1)
		{
			std::cout << "Warning: uniform " << name << " doesn't exist!\n";
		}
	}
	return m_ULocationMap[name];
}

ShaderProgramSource Shader::ParserShader() const
{
	auto fs = std::fstream(m_FilePath);
	auto line = std::string();
	std::stringstream ss[2];
	enum class ShaderType
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};
	auto type = ShaderType::NONE;

	while (getline(fs, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else if (type != ShaderType::NONE)
		{
			ss[static_cast<size_t>(type)] << line << '\n';
		}
	}
	return ShaderProgramSource{ ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& shaderString) const
{
	GlCall(auto shaderId = glCreateShader(type));
	auto shaderSource = shaderString.c_str();
	GlCall(glShaderSource(shaderId, 1, &shaderSource, NULL));
	GlCall(glCompileShader(shaderId));

	// TODO: Error Handling!
	auto result = -1;
	GlCall(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result));
	if (GL_FALSE == result)
	{
		auto length = -1;
		GlCall(glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length));
		auto info = new char[length];
		GlCall(glGetShaderInfoLog(shaderId, length, &length, info));
		std::cout << "FATAL ERROR in compile "
			<< (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< " shader: \n";
		std::cout << info << '\n';
		delete[] info;
		GlCall(glDeleteShader(shaderId));
		return static_cast<unsigned int>(0);
	}

	return shaderId;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	GlCall(m_RendererID = glCreateProgram());
	auto vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	auto fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GlCall(glAttachShader(m_RendererID, vs));
	GlCall(glAttachShader(m_RendererID, fs));
	GlCall(glLinkProgram(m_RendererID));
	GlCall(glValidateProgram(m_RendererID));

	GlCall(glDeleteShader(vs));
	GlCall(glDeleteShader(fs));

	return m_RendererID;
}