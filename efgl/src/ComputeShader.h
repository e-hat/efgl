#pragma once
#include "efpch.h"

#include <unordered_map>

#include <glm/gtc/type_ptr.hpp>

namespace efgl 
{
	class ComputeShader {
	public:
		~ComputeShader();
		ComputeShader(const std::string& filepath);

		void Bind()	  const;
		void Unbind() const;

		void Dispatch(int x, int y, int z);
		void DispatchIndirect(unsigned int byteOffset);

		// For setting uniform blocks
		void BindBlockIndex(const std::string& blockName, unsigned int slot);

		// Set uniforms
		void SetUniform(const std::string& name, const bool& value)
		{
			GLCall(glUniform1i(getUniformLocation(name), static_cast<int>(value)));
		}

		void SetUniform(const std::string& name, const int& value)
		{
			GLCall(glUniform1i(getUniformLocation(name), value));
		}

		void SetUniform(const std::string& name, const float& value)
		{
			GLCall(glUniform1f(getUniformLocation(name), value));
		}

		void SetUniform(const std::string& name, const glm::vec2& value)
		{
			GLCall(glUniform2fv(getUniformLocation(name), 1, glm::value_ptr(value)));
		}

		void SetUniform(const std::string& name, const float& x, const float& y)
		{
			GLCall(glUniform2f(getUniformLocation(name), x, y));
		}

		void SetUniform(const std::string& name, const glm::vec3& value)
		{
			GLCall(glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(value)));
		}

		void SetUniform(const std::string& name, const float& x, const float& y,
			const float& z)
		{
			GLCall(glUniform3f(getUniformLocation(name), x, y, z));
		}

		void SetUniform(const std::string& name, const glm::vec4& value)
		{
			GLCall(glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(value)));
		}

		void SetUniform(const std::string& name, const float& x, const float& y,
			const float& z, const float& w)
		{
			GLCall(glUniform4f(getUniformLocation(name), x, y, z, w));
		}

		void SetUniform(const std::string& name, const glm::mat2& mat)
		{
			GLCall(glUniformMatrix2fv(getUniformLocation(name), 1, GL_FALSE,
				glm::value_ptr(mat)));
		}

		void SetUniform(const std::string& name, const glm::mat3& mat)
		{
			GLCall(glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE,
				glm::value_ptr(mat)));
		}

		void SetUniform(const std::string& name, const glm::mat4 mat)
		{
			GLCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE,
				glm::value_ptr(mat)));
		}

	private:
		unsigned int m_RendererID;
		std::string m_FilePath;

		// for caching uniform locations
		std::unordered_map<std::string, int> m_UniformLocationCache;
		std::unordered_map<std::string, int> m_UniformBlockLocationCache;

		int getUniformLocation(const std::string& name);

		std::string parseShader(const std::string& filepath);
		unsigned int compileShader(unsigned int type, const std::string& source);
		unsigned int createShader(const std::string& source);
	};
}