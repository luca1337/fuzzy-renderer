#include <entities/model.h>

#include <logger.h>
#include <ranges>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#define STB_IMAGE_IMPLEMENTATION
#include <filesystem>
#include <stb_image.h>

#include "components/mesh_renderer.h"

namespace libgraphics
{
	struct TexturePair
	{
		aiTextureType m_type = {};
		std::string m_path = {};
	};

#pragma region FREE FUNCTIONS

	auto AssimpTextureTypeToNative(const aiTextureType type) -> TextureType
	{
		switch (type)
		{
		case aiTextureType_DIFFUSE:return TextureType::albedo;
		case aiTextureType_SPECULAR:return TextureType::specular;
		case aiTextureType_NORMALS:return TextureType::normals;
		case aiTextureType_HEIGHT:return TextureType::height;
		case aiTextureType_AMBIENT:return TextureType::ambient;
		case aiTextureType_EMISSIVE:return TextureType::emissive;
		case aiTextureType_OPACITY:return TextureType::opacity;
		case aiTextureType_DISPLACEMENT:return TextureType::displacement;
		case aiTextureType_REFLECTION:return TextureType::reflection;
		default:return TextureType::albedo; // Valore predefinito in caso di tipo di texture non riconosciuto
		}
	}


	auto LoadMaterialTextures(const aiScene& scene, const aiMaterial& material, const aiTextureType type, const std::string_view type_name, const std::string_view model_path) -> std::vector<Texture>
	{
		auto textures_loaded = std::unordered_map<TextureType, Texture>{};
		auto textures = std::vector<Texture>{};

		const auto textures_count = std::views::iota(0ul) | std::views::take(material.GetTextureCount(type));

		std::ranges::for_each(textures_count, [&](const auto texture_idx) {
			auto texture_assimp_path = aiString{};
			material.GetTexture(type, texture_idx, &texture_assimp_path);

			const auto texture_type = AssimpTextureTypeToNative(type);

			auto skip = false;
			if (const auto it = textures_loaded.find(texture_type); it != textures_loaded.end())
			{
				// Texture with the same type has already been loaded, continue to next one.
				textures.push_back(it->second);
				skip = true;
			}

			if (!skip)
			{
				if (const auto ai_texture = scene.GetEmbeddedTexture(texture_assimp_path.C_Str()))
				{
					const auto& texture = Texture(reinterpret_cast<stbi_uc*>(ai_texture->pcData), ai_texture->mWidth, ai_texture->mHeight, texture_type);
					textures.push_back(texture);
					textures_loaded[texture_type] = texture;
				}
				else
				{
					auto file_path = std::filesystem::path{ model_path };
					const auto& model_folder_path = file_path.remove_filename().string();
					const auto& texture_from_file_path = model_folder_path + texture_assimp_path.C_Str();
					const auto& texture = Texture(texture_from_file_path, texture_type);
					textures.push_back(texture);
					textures_loaded[texture_type] = texture;
				}
			}
		});

		return textures;
	}

	auto ExtractTextures(const aiMesh& mesh, const aiScene& scene, const std::string_view model_path, auto& out_textures) -> void
	{
		const auto texture_pairs = std::vector<TexturePair>
		{
			{aiTextureType_DIFFUSE, "texture_diffuse"}, // albedo
			{aiTextureType_SPECULAR, "texture_specular"},
			{aiTextureType_NORMALS, "texture_normal"},
			{aiTextureType_HEIGHT, "texture_height"},
			{aiTextureType_AMBIENT, "texture_ambient"},
			{aiTextureType_EMISSIVE, "texture_emissive"},
			{aiTextureType_OPACITY, "texture_opacity"},
			{aiTextureType_DISPLACEMENT, "texture_displacement"},
			{aiTextureType_REFLECTION, "texture_reflection"}
		};

		const auto material = scene.mMaterials[mesh.mMaterialIndex];

		std::ranges::for_each(std::views::iota(0ul) | std::views::take(texture_pairs.size()), [&](const auto idx) {
			const auto& [m_type, m_path] = texture_pairs[idx];
			const auto& current_loaded_texture = LoadMaterialTextures(scene, *material, m_type, m_path, model_path);
			std::ranges::copy(current_loaded_texture, std::back_inserter(out_textures));
			});
	};

	auto ExtractVertices(const aiMesh& mesh, std::vector<Vertex>& out_vertices)
	{
		// walk through each of the mesh's vertices
		for (unsigned int i = 0; i < mesh.mNumVertices; i++)
		{
			auto vertex = Vertex{};

			// positions
			vertex.m_position = { mesh.mVertices[i].x, mesh.mVertices[i].y, mesh.mVertices[i].z };

			// normals
			if (mesh.HasNormals())
			{
				vertex.m_normal = { mesh.mNormals[i].x, mesh.mNormals[i].y, mesh.mNormals[i].z };
			}

			// texture coordinates
			if (mesh.mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				glm::vec2 vec = {};

				// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vertex.m_tex_coords = { mesh.mTextureCoords[0][i].x, mesh.mTextureCoords[0][i].y };

				// tangent
				vertex.m_tangent = { mesh.mTangents[i].x, mesh.mTangents[i].y, mesh.mTangents[i].z };

				// bi-tangent
				vertex.m_bitangent = { mesh.mBitangents[i].x, mesh.mBitangents[i].y, mesh.mBitangents[i].z };
			}

			out_vertices.push_back(vertex);
		}
	}

	auto ExtractIndices(const aiMesh& mesh, std::vector<uint32_t>& out_indices)
	{
		std::ranges::for_each(std::views::iota(0ul, mesh.mNumFaces), [&](auto face_idx) {
			const auto& face = mesh.mFaces[face_idx];
			std::ranges::for_each(std::views::iota(0ul, face.mNumIndices), [&](auto indices_idx) { out_indices.push_back(face.mIndices[indices_idx]); });
			});
	}

	auto ProcessMesh(const aiMesh& mesh, const aiScene& scene, const std::string_view model_path) -> GLMesh
	{
		auto vertices = std::vector<Vertex>{};
		ExtractVertices(mesh, vertices);

		auto indices = std::vector<uint32_t>{};
		ExtractIndices(mesh, indices);

		auto textures = std::vector<Texture>{};
		ExtractTextures(mesh, scene, model_path, textures);

		return { vertices, indices, textures };
	}

	auto ProcessNode(const aiNode& node, const aiScene& scene, const std::string_view model_path, std::vector<GLMesh>& out_meshes) -> void
	{
		// process all the node's meshes (if any)
		for (auto i = 0ul; i != node.mNumMeshes; ++i)
		{
			const auto mesh = scene.mMeshes[node.mMeshes[i]];
			out_meshes.push_back(ProcessMesh(*mesh, scene, model_path));
		}

		// then do the same for each of its children
		for (auto i = 0ul; i != node.mNumChildren; ++i)
		{
			ProcessNode(*node.mChildren[i], scene, model_path, out_meshes);
		}
	}

	auto LoadModel(const std::string_view path, std::vector<GLMesh>& out_meshes) -> void
	{
		auto import = Assimp::Importer{};
		const auto scene = import.ReadFile(path.data(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			CX_CORE_ERROR("couldn't load assimp model");
			return;
		}

		ProcessNode(*scene->mRootNode, *scene, path, out_meshes);
	}

#pragma endregion

	Model::Model(const std::string_view path)
	{
		auto meshes = std::vector<GLMesh>{};

		LoadModel(path, meshes);

		for (const auto& mesh : meshes)
		{
			auto mesh_renderer = std::make_shared<MeshRenderer>(mesh);

			auto mesh_entity = std::make_shared<Entity>();
			mesh_entity->GetTransformComponent().SetScale(glm::vec3{ 0.3f });
			mesh_entity->AddComponent(mesh_renderer);

			m_entities.push_back(mesh_entity);
		}
	}

	auto Model::Render() -> void
	{
		Entity::Render();

		for (const auto& entity : m_entities)
		{
			entity->Render();
		}
	}

	auto Model::Update(const float delta_time) -> void
	{
		Entity::Update(delta_time);

		for (const auto& entity : m_entities)
		{
			entity->Update(delta_time);
		}
	}
}
