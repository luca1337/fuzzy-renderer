#include <opengl/model.h>

#include <assimp/postprocess.h>

#include <logger.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace libgraphics
{
	auto TextureFromFile(const char* path, const std::string& directory, bool gamma)
	{
		std::string file_name = std::string(path);
		file_name = directory + '/' + file_name;

		uint32_t texture_id = { };
		glGenTextures(1, &texture_id);

		int width, height, nr_components;
		if (const auto data = stbi_load(file_name.c_str(), &width, &height, &nr_components, 0))
		{
			GLenum format = {};
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

	Model::Model(const std::string_view path)
	{
		LoadModel(path);
	}

	auto Model::Draw(const std::shared_ptr<IShader>& shader) const -> void
	{
		for (auto mesh : m_meshes)
		{
			mesh.Draw(shader);
		}
	}

	auto Model::LoadModel(const std::string_view path) -> void
	{
		auto import = Assimp::Importer{};
		const auto scene = import.ReadFile(path.data(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			//todo: handle error
			return;
		}

		m_directory = path.substr(0, path.find_first_of('/'));

		ProcessNode(scene->mRootNode, scene);
	}

	auto Model::ProcessNode(const aiNode* node, const aiScene* scene) -> void
	{
		// process all the node's meshes (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_meshes.push_back(ProcessMesh(mesh, scene));
		}

		// then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}

	auto Model::ProcessMesh(const aiMesh* mesh, const aiScene* scene) -> GLMesh
	{
		auto vertices = std::vector<Vertex>{};
		auto indices = std::vector<uint32_t>{};
		auto textures = std::vector<Texture>{};

		// walk through each of the mesh's vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex = {};
			glm::vec3 vector = {}; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.

			// positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.m_position = { vector };

			// normals
			if (mesh->HasNormals())
			{
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				vertex.m_normal = vector;
			}

			// texture coordinates
			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				glm::vec2 vec = {};

				// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.m_tex_coords = vec;

				// tangent
				vector.x = mesh->mTangents[i].x;
				vector.y = mesh->mTangents[i].y;
				vector.z = mesh->mTangents[i].z;
				vertex.m_tangent = vector;

				// bi-tangent
				vector.x = mesh->mBitangents[i].x;
				vector.y = mesh->mBitangents[i].y;
				vector.z = mesh->mBitangents[i].z;
				vertex.m_bitangent = vector;
			}
			else
			{
				vertex.m_tex_coords = glm::vec2{};
			}

			vertices.push_back(vertex);
		}

		// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];

			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		// process materials
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
		// Same applies to other texture as the following list summarizes:
		// diffuse: texture_diffuseN
		// specular: texture_specularN
		// normal: texture_normalN

		// 1. diffuse maps
		auto diffuse_maps = LoadMaterialTextures(scene, material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

		// 2. specular maps
		auto specular_maps = LoadMaterialTextures(scene, material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());

		// 3. normal maps
		auto normal_maps = LoadMaterialTextures(scene, material, aiTextureType_NORMALS, "texture_normal");
		textures.insert(textures.end(), normal_maps.begin(), normal_maps.end());

		// 4. height maps
		auto height_maps = LoadMaterialTextures(scene, material, aiTextureType_HEIGHT, "texture_height");
		textures.insert(textures.end(), height_maps.begin(), height_maps.end());

		// return a mesh object created from the extracted mesh data
		return { vertices, indices, textures };
	}

	auto LoadTextureFromEmbeddedData(const aiTexture* ai_texture)
	{
		// Assuming the embedded texture data is in RGB format
		GLenum format = GL_RGB;
		if (ai_texture->mHeight == 0)
		{
			// If the texture has no height, it is a 1D texture
			format = GL_RGB;
		}
		else if (ai_texture->mHeight > 0)
		{
			// If the texture has height, it is a 2D texture
			format = GL_RGBA;
		}

		int width, height, channels;
		stbi_uc* image_data = {};

		uint32_t texture_id = 0;
		if (ai_texture->mHeight == 0)
		{
			// Load and decode the compressed image data
			image_data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(ai_texture->pcData), static_cast<int>(ai_texture->mWidth), &width, &height, &channels, STBI_rgb_alpha);
		}
		else
		{
			// Use the raw image data directly
			width = static_cast<int>(ai_texture->mWidth);
			height = static_cast<int>(ai_texture->mHeight);
			image_data = reinterpret_cast<stbi_uc*>(ai_texture->pcData);
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
			if (ai_texture->mHeight == 0)
			{
				stbi_image_free(image_data);
			}
		}

		return texture_id;
	}

	auto Model::LoadMaterialTextures(const aiScene* scene, const aiMaterial* material, const aiTextureType type, const std::string_view type_name) -> std::vector<Texture>
	{
		auto textures = std::vector<Texture>{};

		const auto texture_count = material->GetTextureCount(type);
		for (unsigned int i = 0; i < texture_count; i++)
		{
			aiString str;
			material->GetTexture(type, i, &str);

			// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			bool skip = false;
			for (auto& j : textures_loaded)
			{
				if (std::strcmp(j.m_path.data(), str.C_Str()) == 0)
				{
					textures.push_back(j);
					skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
					break;
				}
			}

			if (!skip)
			{
				// if texture hasn't been loaded already, load it
				Texture texture = {};
				// Assuming you have an Assimp `aiScene` object named `scene` that represents the loaded `.glb` file
				if (const aiTexture* ai_texture = scene->GetEmbeddedTexture(str.C_Str()))
				{
					// Here, you can access the embedded texture data from `aiTexture` and load it into your OpenGL texture
					// For example, you can generate a texture ID, set texture parameters, and load the texture data using `glTexImage2D`
					texture.m_id = LoadTextureFromEmbeddedData(ai_texture);
				}
				else
				{
					// The texture is not an embedded texture, you can load it from a file using your existing code
					texture.m_id = TextureFromFile(str.C_Str(), m_directory.data(), true);
				}

				texture.m_type = type_name;
				texture.m_path = str.C_Str();
				textures.push_back(texture);
				textures_loaded.push_back(texture);  // store it as texture loaded for the entire model, to ensure we won't unnecessarily load duplicate textures.
			}
		}
		return textures;
	}

}
