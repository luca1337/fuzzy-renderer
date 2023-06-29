#include <entities/model.h>

#include <logger.h>
#include <ranges>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <filesystem>

#include "components/mesh_renderer.h"

namespace libgraphics
{
	struct TexturePair
	{
		aiTextureType m_type = {};
		std::string m_path = {};
	};

#pragma region FREE FUNCTIONS

	auto LoadTextureFromEmbeddedData(const aiTexture& ai_texture)
	{
		// Assuming the embedded texture data is in RGB format
		GLenum format = GL_RGB;
		if (ai_texture.mHeight == 0)
		{
			// If the texture has no height, it is a 1D texture
			format = GL_RGB;
		}
		else if (ai_texture.mHeight > 0)
		{
			// If the texture has height, it is a 2D texture
			format = GL_RGBA;
		}

		int width, height, channels;
		stbi_uc* image_data = {};

		uint32_t texture_id = 0;
		if (ai_texture.mHeight == 0)
		{
			// Load and decode the compressed image data
			image_data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(ai_texture.pcData), static_cast<int>(ai_texture.mWidth), &width, &height, &channels, STBI_rgb_alpha);
		}
		else
		{
			// Use the raw image data directly
			width = static_cast<int>(ai_texture.mWidth);
			height = static_cast<int>(ai_texture.mHeight);
			image_data = reinterpret_cast<stbi_uc*>(ai_texture.pcData);
		}

		if (image_data)
		{
			// Create texture using the imageData (RGB or RGBA values)
			glGenTextures(1, &texture_id);
			glBindTexture(GL_TEXTURE_2D, texture_id);
			glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(format), width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
			glGenerateMipmap(GL_TEXTURE_2D);

			// Set texture parameters and options as needed
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// If the image_data was loaded using stbi_load_from_memory, free the decoded image data
			if (ai_texture.mHeight == 0)
			{
				stbi_image_free(image_data);
			}
		}

		return texture_id;
	}

	auto TextureFromFile(const std::string_view path)
	{
		auto texture_id = uint32_t{ };
		glGenTextures(1, &texture_id);

		int width, height, nr_components;
		if (const auto data = stbi_load(path.data(), &width, &height, &nr_components, 0))
		{
			auto format = GLint{};
			if (nr_components == 1)
				format = GL_RED;
			else if (nr_components == 3)
				format = GL_RGB;
			else if (nr_components == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, texture_id);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			const auto error_message = std::format("Texture failed to load at path: {}", path);
			CX_CORE_ERROR(error_message);
			stbi_image_free(data);
		}

		return texture_id;
	}

	auto LoadMaterialTextures(const aiScene& scene, const aiMaterial& material, const aiTextureType type, const std::string_view type_name, const std::string_view model_path) -> std::vector<Texture>
	{
		auto textures_loaded = std::vector<Texture>{};
		auto textures = std::vector<Texture>{};

		const auto textures_count = std::views::iota(0ul) | std::views::take(material.GetTextureCount(type));
		std::ranges::for_each(textures_count, [&](const auto texture_idx) {

			auto texture_assimp_path = aiString{};
			material.GetTexture(type, texture_idx, &texture_assimp_path);

			// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			auto skip = false;
			auto found_texture = std::ranges::find_if(textures_loaded, [&](const auto& j) { return j.m_path.data() == texture_assimp_path.C_Str(); });
			if (found_texture != textures_loaded.end())
			{
				// a texture with the same filepath has already been loaded, continue to next one. (optimization)
				textures.push_back(*found_texture);
				skip = true;
			}

			if (!skip)
			{
				// if texture hasn't been loaded already, load it
				auto texture = Texture{};
				if (const auto ai_texture = scene.GetEmbeddedTexture(texture_assimp_path.C_Str()))
				{
					texture.m_id = LoadTextureFromEmbeddedData(*ai_texture);
				}
				else
				{
					auto file_path = std::filesystem::path{ model_path };
					const auto& model_folder_path = file_path.remove_filename().string();
					const auto& texture_from_file_path = model_folder_path + texture_assimp_path.C_Str();
					texture.m_id = TextureFromFile(texture_from_file_path);
				}

				texture.m_type = type_name;
				texture.m_path = texture_assimp_path.C_Str();
				textures.push_back(texture);
				textures_loaded.push_back(texture);  // store it as texture loaded for the entire model, to ensure we won't unnecessarily load duplicate textures.
			}
			});

		return textures;
	}

	auto ExtractTextures(const aiMesh& mesh, const aiScene& scene, const std::string_view model_path, auto& out_textures) -> void
	{
		const auto texture_pairs = std::vector<TexturePair>
		{
			{aiTextureType_DIFFUSE, "texture_diffuse"},
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
			mesh_entity->AddComponent(mesh_renderer);

			m_entities.push_back(mesh_entity);
		}
	}

	auto Model::Update() const -> void
	{
		for (const auto& entity : m_entities)
		{
			entity->Render();
			entity->Update(0.0f);
		}
	}
}
