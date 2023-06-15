#pragma once

#include <framework.h>

namespace libgraphics
{
	class IShader;

	class GLSkybox
	{
	public:
		GLSkybox();
		auto Render(const std::shared_ptr<IShader>& shader) const -> void;

	private:

		auto Setup() -> void;

		uint32_t m_cube_vao, m_cube_vbo = {};
		uint32_t m_sky_vao, m_sky_vbo = {};
		uint32_t m_cubemap_tex_id = {};
	};
}