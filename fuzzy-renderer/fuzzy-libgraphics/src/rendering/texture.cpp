#include <rendering/texture.h>

#include <logger.h>
#include <stb_image.h>

namespace libgraphics
{
	[[nodiscard]] auto LoadTexture(const unsigned char* data, const int width, const int height, const GLenum format) -> GLuint
	{
		auto texture_id = GLuint{};

		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(format), width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);

		return texture_id;
	}

	Texture::Texture(const std::string_view file_path, const TextureType type) : m_file_path{ file_path }, m_type{ type }
	{
		int width, height, num_channels;
		if (const auto image_data = stbi_load(file_path.data(), &width, &height, &num_channels, STBI_rgb_alpha))
		{
			const auto format = (num_channels == 3) ? GL_RGB : GL_RGBA;
			m_texture_id = LoadTexture(image_data, width, height, format);
			stbi_image_free(image_data);
		}
		else
		{
			CX_CORE_ERROR("Unable to load texture from file: {0}", file_path);
		}
	}

	Texture::Texture(unsigned char* data, const unsigned int width, const unsigned int height, const TextureType type) : m_type{ type }
	{
		GLenum texture_format = GL_RGB;
		if (height == 0)
		{
			// If the texture has no height, it is a 1D texture
			texture_format = GL_RGB;
		}
		else if (height > 0)
		{
			// If the texture has height, it is a 2D texture
			texture_format = GL_RGBA;
		}

		int out_width, out_height, out_channels;
		stbi_uc* image_data = {};

		if (height == 0)
		{
			// Load and decode the compressed image data
			image_data = stbi_load_from_memory(data, width, &out_width, &out_height, &out_channels, STBI_rgb_alpha);
		}
		else
		{
			// Use the raw image data directly
			out_width = static_cast<int>(width);
			out_height = static_cast<int>(height);
			image_data = data;
		}

		if (image_data)
		{
			m_texture_id = LoadTexture(image_data, out_width, out_height, texture_format);

			// If the image_data was loaded using stbi_load_from_memory, free the decoded image data
			if (height == 0)
			{
				stbi_image_free(image_data);
			}
		}
	}
}