#include "efpch.h"

#include "ComputeShader.h"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

namespace efgl {

	ComputeShader::ComputeShader(const std::string& filePath)
		: m_FilePath(filePath)
	{
		std::string src = parseShader(filePath);
		m_RendererID = createShader(filePath);
	}

	std::string ComputeShader::parseShader(const string& filepath)
	{
		std::ifstream stream(filepath);

		if (!stream.good()) {
			std::cout << "Failed to load shader at " << filepath << std::endl;
		}
		
		string line;
		stringstream ss;
		while (getline(stream, line))
		{
			ss << line << '\n';
		}

		return ss.str();
	}

	unsigned int ComputeShader::compileShader(unsigned int type, const string& source)
	{
		GLCall(unsigned int id = glCreateShader(type));
		const char* src = source.c_str();
		GLCall(glShaderSource(id, 1, &src, nullptr));
		GLCall(glCompileShader(id));

		int result;
		GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
		if (result == GL_FALSE)
		{
			int length;
			GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
			char* msg = (char*)_malloca(length * sizeof(char));
			GLCall(glGetShaderInfoLog(id, length, &length, msg));
			cout << "Failed to compile " << ((type == GL_VERTEX_SHADER) ? "vertex" : "fragment")
				<< "shader. Error log:" << endl;
			cout << msg << endl;
			GLCall(glDeleteShader(id));
			return 0;
		}

		return id;
	}

	unsigned int ComputeShader::createShader(const string& source)
	{
		GLCall(unsigned int program = glCreateProgram());
		unsigned int cs = compileShader(GL_COMPUTE_SHADER, source);

		GLCall(glAttachShader(program, cs));
		GLCall(glLinkProgram(program));
		GLCall(glValidateProgram(program));

		GLCall(glDeleteShader(cs));

		return program;
	}

	void ComputeShader::Bind() const
	{
		GLCall(glUseProgram(m_RendererID));
	}

	void ComputeShader::Unbind() const
	{
		GLCall(glUseProgram(0));
	}

	int ComputeShader::getUniformLocation(const string& name)
	{
		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
			return m_UniformLocationCache[name];
		GLCall(unsigned int location = glGetUniformLocation(m_RendererID, name.c_str()));
		if (location == -1)
			cout << "Warning: Uniform " << name << " doesn't exist." << endl;

		m_UniformLocationCache[name] = location;
		return location;
	}
}