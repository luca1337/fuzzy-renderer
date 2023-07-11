#pragma once

#include <glad/gl.h>
#include <interfaces/ishader.h>

namespace libgraphics
{
	class GLShader final : public IShader
	{
	public:
		GLShader() = default;
		GLShader(const std::string_view vertex, const std::string_view fragment);

		auto Bind() const -> void override { glUseProgram(m_program_id); }
		auto Unbind() const -> void override { glUseProgram(0); }
		auto AllocateLightsBuffer(const std::string& uniform_block_name) -> void;

		auto SetMatrix4x4(const std::string_view name, const glm::mat4& m) -> void override { glUniformMatrix4fv(glGetUniformLocation(m_program_id, name.data()), 1, false, &m[0][0]); }
		auto SetFloat(const std::string_view name, const float value) -> void override { glUniform1f(glGetUniformLocation(m_program_id, name.data()), value); }
		auto SetVec3(const std::string_view name, const glm::vec3& value) -> void override { glUniform3fv(glGetUniformLocation(m_program_id, name.data()), 1, &value[0]); }
		auto SetInt(const std::string_view name, const int value) -> void override { glUniform1i(glGetUniformLocation(m_program_id, name.data()), value); }
		auto SetUint(const std::string_view name, const uint32_t value) -> void override { glUniform1ui(glGetUniformLocation(m_program_id, name.data()), value); }
		auto SetBool(const std::string_view name, const bool value) -> void override { glUniform1i(glGetUniformLocation(m_program_id, name.data()), value); }

		auto GetID() const -> GLuint override { return m_program_id; }
		auto GetLightsBufferID() const -> GLuint override { return m_lights_buffer; }

	private:
		GLuint m_program_id = {};
		GLuint m_lights_buffer = {};
	};
}