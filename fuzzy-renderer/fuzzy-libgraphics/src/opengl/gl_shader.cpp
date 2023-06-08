#include <opengl/gl_shader.h>

#include <logger.h>

#include <fstream>
#include <sstream>
#include <source_location>
#include <span>

#include <glad/glad.h>

namespace libgraphics
{
    GLShader::GLShader(const std::string_view vertex, const std::string_view fragment)
    {
        auto ReadShader = [&](const std::string& path) -> std::string {
            std::ifstream shader_file(path);
            if (!shader_file.is_open()) 
            {
                const auto location = std::source_location::current();
                const auto error_message = std::format("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: {} [{}:{}]", path, location.file_name(), location.line());
                CX_CORE_CRITICAL(error_message);
                throw std::runtime_error(error_message);
            }

            std::stringstream shader_stream;
            shader_stream << shader_file.rdbuf();
            shader_file.close();
            return shader_stream.str();
        };

        // Read shaders
        const auto vertex_shader_code = ReadShader(vertex.data());
        const auto fragment_shader_code = ReadShader(fragment.data());

        // Compile shader
        auto CompileShader = [&](const std::span<const char> shader_source, const GLenum shader_type) -> GLuint {
	        const auto shader = glCreateShader(shader_type);
            const char* shader_source_ptr = shader_source.data();
            glShaderSource(shader, 1, &shader_source_ptr, nullptr);
            glCompileShader(shader);

            GLint success;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                GLchar info_log[512];
                glGetShaderInfoLog(shader, sizeof info_log, nullptr, info_log);

                const auto location = std::source_location::current();
                const auto error_message = std::format("ERROR::SHADER::COMPILATION_FAILED: [{}:{}]\n{}", location.file_name(), location.line(), info_log);
                CX_CORE_CRITICAL(error_message);
                throw std::runtime_error(error_message);
            }

            return shader;
        };

        const auto vertex_id = CompileShader(std::span(vertex_shader_code.data(), vertex_shader_code.size()), GL_VERTEX_SHADER);
        const auto fragment_id = CompileShader(std::span(fragment_shader_code.data(), fragment_shader_code.size()), GL_FRAGMENT_SHADER);

        // Shader program
        m_program_id = glCreateProgram();
        glAttachShader(m_program_id, vertex_id);
        glAttachShader(m_program_id, fragment_id);
        glLinkProgram(m_program_id);

        GLint success;
        glGetProgramiv(m_program_id, GL_LINK_STATUS, &success);
        if (!success) {
            GLchar info_log[512];
            glGetProgramInfoLog(m_program_id, sizeof info_log, nullptr, info_log);

            const auto location = std::source_location::current();
            const auto error_message = std::format("ERROR::SHADER::PROGRAM::LINKING_FAILED: [{}:{}]\n{}", location.file_name(), location.line(), info_log);
            CX_CORE_CRITICAL(error_message);
            throw std::runtime_error(error_message);
        }

        glDeleteShader(vertex_id);
        glDeleteShader(fragment_id);

        CX_CORE_INFO("Shaders successfully compiled!");
    }
}