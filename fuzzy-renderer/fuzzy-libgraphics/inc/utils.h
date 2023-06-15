#pragma once

#include <vector>
#include <string>
#include <logger.h>

#include <stb_image.h>

#include <glad/gl.h>

namespace utils::gl
{
	/**
	 * \brief Generate 3D skybox effect
	 * \param faces List of faces which represent file names (top.jpg, bottom.jpg etc..)
	 * \return id of the texture which will be drawn
	 */
	auto LoadCubemap(const std::string_view folder_path) -> uint32_t;
}