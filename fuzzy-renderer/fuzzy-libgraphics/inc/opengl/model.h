#pragma once

#include <memory>
#include <vector>

#include <opengl/gl_mesh.h>

namespace libgraphics
{
	struct Texture;
	class IShader;

	class Model
	{
	public:
		explicit Model(const std::string_view path);
		auto Render(const std::shared_ptr<IShader>& shader) const -> void;

		auto& GetMeshes() const { return m_meshes; }

	private:
		std::vector<GLMesh> m_meshes = {};
	};
}
