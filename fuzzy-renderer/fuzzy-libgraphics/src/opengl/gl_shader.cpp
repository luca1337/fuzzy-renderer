#include <engine_constants.h>
#include <fstream>
#include <logger.h>
#include <source_location>
#include <span>
#include <sstream>
#include <glad/gl.h>
#include <opengl/gl_shader.h>
#include <rendering/light.h>

namespace libgraphics
{
	auto read_shader(const std::string& path) -> std::string
	{
		auto shader_file = std::ifstream{ path };
		if (!shader_file.is_open())
		{
			const auto location = std::source_location::current();
			const auto error_message = std::format("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: {} [{}:{}]", path, location.file_name(), location.line());
			CX_CORE_CRITICAL(error_message);
			throw std::runtime_error(error_message);
		}

		std::stringstream shader_stream = {};
		shader_stream << shader_file.rdbuf();
		shader_file.close();
		return shader_stream.str();
	}

	auto compile_shader(const std::span<const char> shader_source, const GLenum shader_type) -> GLuint
	{
		const auto shader = glCreateShader(shader_type);
		const auto shader_source_ptr = shader_source.data();
		glShaderSource(shader, 1, &shader_source_ptr, nullptr);
		glCompileShader(shader);

		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) 
		{
			GLchar info_log[512];
			glGetShaderInfoLog(shader, sizeof info_log, nullptr, info_log);

			const auto location = std::source_location::current();
			const auto error_message = std::format("ERROR::SHADER::COMPILATION_FAILED: [{}:{}]\n{}", location.file_name(), location.line(), info_log);
			CX_CORE_CRITICAL(error_message);
			throw std::runtime_error(error_message);
		}

		return shader;
	}

	GLShader::GLShader(const std::string_view vertex, const std::string_view fragment)
	{
		const auto vertex_shader_code = read_shader(vertex.data());
		const auto fragment_shader_code = read_shader(fragment.data());

		const auto vertex_id = compile_shader(std::span(vertex_shader_code.data(), vertex_shader_code.size()), GL_VERTEX_SHADER);
		const auto fragment_id = compile_shader(std::span(fragment_shader_code.data(), fragment_shader_code.size()), GL_FRAGMENT_SHADER);

		// Shader program
		m_program_id = glCreateProgram();
		glAttachShader(m_program_id, vertex_id);
		glAttachShader(m_program_id, fragment_id);
		glLinkProgram(m_program_id);

		auto success = GLint{};
		glGetProgramiv(m_program_id, GL_LINK_STATUS, &success);
		if (!success) 
		{
			GLchar info_log[512] = {};
			glGetProgramInfoLog(m_program_id, sizeof info_log, nullptr, info_log);

			const auto location = std::source_location::current();
			const auto error_message = std::format("ERROR::SHADER::PROGRAM::LINKING_FAILED: [{}:{}]\n{}", location.file_name(), location.line(), info_log);
			CX_CORE_CRITICAL(error_message);
			throw std::runtime_error(error_message);
		}

		glDeleteShader(vertex_id);
		glDeleteShader(fragment_id);

		CX_CORE_INFO("GLSL Shaders successfully compiled!");
	}

	auto GLShader::AllocateLightsBuffer(const std::string& uniform_block_name) -> void
	{
		GLint binding_point = {};
		glGetActiveUniformBlockiv(m_program_id, glGetUniformBlockIndex(m_program_id, uniform_block_name.c_str()), GL_UNIFORM_BLOCK_BINDING, &binding_point);

		glGenBuffers(1, &m_lights_buffer);
		glBindBuffer(GL_UNIFORM_BUFFER, m_lights_buffer);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(Light) * constants::MaxNumberOfLights, nullptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glBindBufferRange(GL_UNIFORM_BUFFER, binding_point, m_lights_buffer, 0, constants::MaxNumberOfLights * sizeof(Light));
	}
}
