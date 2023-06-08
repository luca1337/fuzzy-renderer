#pragma once

#include <interfaces/ishader.h>

namespace libgraphics
{
	class GLShader : public IShader
	{
	public:
		GLShader(const std::string_view vertex, const std::string_view fragment);

		virtual auto Apply() const -> void override;

		virtual auto SetMatrix4x4(const std::string_view name, const glm::mat4& m) -> void override;
		virtual auto SetFloat(const std::string_view name, const float value) -> void override;
		virtual auto SetVec3(const std::string_view name, const glm::vec3& value) -> void override;

	private:
	};
}