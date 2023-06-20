#pragma once

#include <memory>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include <opengl/gl_mesh.h>

namespace libgraphics
{
	struct Texture;
	class IShader;

	class Model
	{
	public:
		explicit Model(const std::string_view path);
		auto Draw(const std::shared_ptr<IShader>& shader) const -> void;

	private:
		std::vector<GLMesh> m_meshes = {};
		auto LoadModel(const std::string_view path) -> void;
		auto ProcessNode(const aiNode* node, const aiScene* scene, const std::string_view model_path) -> void;
		static auto ProcessMesh(const aiMesh* mesh, const aiScene* scene, const std::string_view model_path) -> GLMesh;
	};
}
