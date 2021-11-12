#include "efpch.h"

#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <../tracy/Tracy.hpp>

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

		if (!stream.good()) 
		{
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
      auto msg = std::make_unique<char[]>(length);
			GLCall(glGetShaderInfoLog(id, length, &length, msg.get()));
			cout << "Compilation error on shader at " << m_FilePath << endl;
			cout << "Failed to compile " << ((type == GL_VERTEX_SHADER) ? "vertex" : "fragment")
				<< "shader. Error log:" << endl;
			cout << msg.get() << endl;
			// TODO: figure out a way to get the right line number from this info...
			// current behavior: since frag + vert shaders share files, frag line
			// # is fragNumber + total number of lines in vert
			
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

	void Shader::BindBlockIndex(const std::string& blockName, unsigned int slot) 
	{
		int location = GL_INVALID_INDEX;
		if (m_UniformBlockLocationCache.find(blockName) != m_UniformBlockLocationCache.end())
			location = m_UniformBlockLocationCache[blockName];
		else 
		{
			GLCall(location = glGetUniformBlockIndex(m_RendererID, blockName.c_str()));
			if (location == GL_INVALID_INDEX)
				cout << "Warning: Uniform block " << blockName << " doesn't exist in shader at " << m_FilePath << endl;
			else m_UniformBlockLocationCache[blockName] = location;
		}

		GLCall(glUniformBlockBinding(m_RendererID, location, slot));
	}

	void Shader::Bind() const
	{
		ZoneScoped;
		GLCall(glUseProgram(m_RendererID));
	}

	void Shader::Unbind() const
	{
		GLCall(glUseProgram(0));
	}

	int Shader::getUniformLocation(const string& name)
	{
		ZoneScopedC(tracy::Color::Aqua);
		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
			return m_UniformLocationCache[name];
		GLCall(unsigned int location = glGetUniformLocation(m_RendererID, name.c_str()));
		if (location == -1)
			cout << "Warning: Uniform " << name << " doesn't exist." << endl;

		m_UniformLocationCache[name] = location;
		return location;
	}
}
