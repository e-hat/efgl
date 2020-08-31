#include "efpch.h"

#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

namespace efgl
{
	Shader::Shader(const string& filepath)
		: m_FilePath(filepath)
	{
		ShaderProgramSource src = parseShader(filepath);
		m_RendererID = createShader(src.vertexSrc, src.fragSrc);
	}

	Shader::~Shader()
	{
		GLCall(glDeleteProgram(m_RendererID));
	}

	Shader::ShaderProgramSource Shader::parseShader(const string& filepath)
	{
		std::ifstream stream(filepath);

		if (!stream.good()) {
			std::cout << "Failed to load shader at " << filepath << std::endl;
		}

		enum class ShaderType
		{
			NONE = -1, VERTEX = 0, FRAGMENT = 1
		};

		string line;
		stringstream ss[2];
		ShaderType type = ShaderType::NONE;
		while (getline(stream, line))
		{
			if (line.find("#shader") != string::npos)
			{
				if (line.find("vertex") != string::npos)
					type = ShaderType::VERTEX;
				else if (line.find("fragment") != string::npos)
					type = ShaderType::FRAGMENT;
			}
			else
			{
				EF_ASSERT(type != ShaderType::NONE && "Missing #shader or shader type definition.")
				ss[static_cast<int>(type)] << line << '\n';
			}
		}

		return { ss[0].str(), ss[1].str() };
	}

	unsigned int Shader::compileShader(unsigned int type, const string& source)
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

	unsigned int Shader::createShader(const string& vertexShader, const string& fragmentShader)
	{
		GLCall(unsigned int program = glCreateProgram());
		unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
		unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

		GLCall(glAttachShader(program, vs));
		GLCall(glAttachShader(program, fs));
		GLCall(glLinkProgram(program));
		GLCall(glValidateProgram(program));

		GLCall(glDeleteShader(vs));
		GLCall(glDeleteShader(fs));

		return program;
	}

	void Shader::Bind() const
	{
		GLCall(glUseProgram(m_RendererID));
	}

	void Shader::Unbind() const
	{
		GLCall(glUseProgram(0));
	}

	int Shader::getUniformLocation(const string& name)
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