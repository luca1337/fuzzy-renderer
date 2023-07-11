#pragma once

#include <enums.h>
#include <string_view>
#include <glad/gl.h>

namespace libgraphics
{
	class Texture
	{
	public:
		Texture() = default;
		explicit Texture(const std::string_view file_path, const TextureType type);
		explicit Texture(unsigned char* data, const unsigned int width, const unsigned int height, const TextureType type);

		[[nodiscard]] auto GetFilePath() const -> std::string_view { return m_file_path; }
		[[nodiscard]] auto GetTextureID() const -> GLuint { return m_texture_id; }
		[[nodiscard]] auto GetType() const -> TextureType { return m_type; }
		[[nodiscard]] auto GetIndex() const -> int { return m_index; }

		auto SetIndex(const int index) -> void { m_index = index; }

	private:
		std::string_view m_file_path = {};
		GLuint m_texture_id = {};
		TextureType m_type = {};
		int m_index = {};
	};
}