#pragma once

#include <logger.h>

namespace utils::gl
{
	/**
	 * \brief Generate 3D skybox effect
	 * \param folder_path folder path containing skyboxes (top.jpg, bottom.jpg etc..)
	 * \return id of the texture which will be drawn
	 */
	auto LoadCubemap(const std::string_view folder_path) -> uint32_t;

	auto LoadTextureFromFile(const std::string_view path) -> bool;
}

namespace utils::common
{
	template <typename T, size_t N>
	constexpr size_t ArraySize(T(&)[N]) { return N; }
}