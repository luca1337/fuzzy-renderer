#include <opengl/gl_shader.h>

namespace libgraphics
{
	GLShader::GLShader(const std::string_view vertex, const std::string_view fragment)
	{
	}

	auto GLShader::Apply() const -> void
	{
	}

	auto GLShader::SetMatrix4x4(const std::string_view name, const glm::mat4& m) -> void
	{
	}

	auto GLShader::SetFloat(const std::string_view name, const float value) -> void
	{
	}

	auto GLShader::SetVec3(const std::string_view name, const glm::vec3& value) -> void
	{
	}
}