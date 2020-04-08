#pragma once
#include "efpch.h"

#include <string>
#include <unordered_map>

namespace efgl
{
	namespace ogl
	{
		class GLshader
		{
		public:
			GLshader(const std::string& filepath);
			~GLshader();

			void bind()	  const;
			void unbind() const;

			// Set uniforms
			void setUniform(const std::string& name, bool value)
			{
				GLCall(glUniform1i(getUniformLocation(name), static_cast<int>(value)));
			}

			void setUniform(const std::string& name, int value)
			{
				GLCall(glUniform1i(getUniformLocation(name), value));
			}

			void setUniform(const std::string& name, float value)
			{
				GLCall(glUniform1f(getUniformLocation(name), value));
			}

		private:
			struct ShaderProgramSource
			{
				std::string vertexSrc;
				std::string fragSrc;
			};

			unsigned int m_RendererID;
			std::string m_FilePath;

			// caching for uniforms
			std::unordered_map<std::string, int> m_UniformLocationCache;

			int getUniformLocation(const std::string& name);

			ShaderProgramSource parseShader(const std::string& filepath);
			unsigned int compileShader(unsigned int type, const std::string& source);
			unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);

		};
	}
}