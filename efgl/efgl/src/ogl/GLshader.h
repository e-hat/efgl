#pragma once
#include "efpch.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

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
			void setUniform(const std::string& name, const bool& value)
			{
				GLCall(glUniform1i(getUniformLocation(name), static_cast<int>(value)));
			}

			void setUniform(const std::string& name, const int& value)
			{
				GLCall(glUniform1i(getUniformLocation(name), value));
			}

			void setUniform(const std::string& name, const float& value)
			{
				GLCall(glUniform1f(getUniformLocation(name), value));
			}

			void setUniform(const std::string& name, const glm::vec2& value)
			{
				GLCall(glUniform2fv(getUniformLocation(name), 1, glm::value_ptr(value)));
			}
		
			void setUniform(const std::string& name, const float& x, const float& y)
			{
				GLCall(glUniform2f(getUniformLocation(name), x, y));
			}

			void setUniform(const std::string& name, const glm::vec3& value)
			{
				GLCall(glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(value)));
			}

			void setUniform(const std::string& name, const float& x, const float& y, 
							const float& z)
			{
				GLCall(glUniform3f(getUniformLocation(name), x, y, z));
			}

			void setUniform(const std::string& name, const glm::vec4& value)
			{
				GLCall(glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(value)));
			}

			void setUniform(const std::string& name, const float& x, const float& y, 
							const float& z, const float& w)
			{
				GLCall(glUniform4f(getUniformLocation(name), x, y, z, w));
			}

			void setUniform(const std::string& name, const glm::mat2& mat)
			{
				GLCall(glUniformMatrix2fv(getUniformLocation(name), 1, GL_FALSE, 
										  glm::value_ptr(mat)));
			}

			void setUniform(const std::string& name, const glm::mat3& mat)
			{
				GLCall(glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, 
										  glm::value_ptr(mat)));
			}

			void setUniform(const std::string& name, const glm::mat4 mat)
			{
				GLCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, 
										  glm::value_ptr(mat)));
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