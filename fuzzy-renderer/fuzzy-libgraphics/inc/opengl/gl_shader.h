#pragma once

#include <glad/glad.h>
#include <interfaces/ishader.h>

namespace libgraphics
{
	class LIBGRAPHICS_API GLShader final : public IShader
	{
	public:
		GLShader() = default;
		GLShader(const std::string_view vertex, const std::string_view fragment);

		auto Bind() const -> void override { glUseProgram(m_program_id); }

		auto SetMatrix4x4(const std::string_view name, const glm::mat4& m) -> void override { glUniformMatrix4fv(glGetUniformLocation(m_program_id, name.data()), 1, false, &m[0][0]); }
		auto SetFloat(const std::string_view name, const float value) -> void override { glUniform1f(glGetUniformLocation(m_program_id, name.data()), value); }
		auto SetVec3(const std::string_view name, const glm::vec3& value) -> void override { glUniform3fv(glGetUniformLocation(m_program_id, name.data()), 1, &value[0]); }

	private:
		unsigned int m_program_id = {};
	};
}