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
		if (unsigned char* data = stbi_load(file_name.c_str(), &width, &height, &nr_components, 0))
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
			//std::cout << "Texture failed to load at path: " << path << std::endl;
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
		const auto scene = import.ReadFile(path.data(), aiProcess_Triangulate | aiProcess_FlipUVs);

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
			if (false) // does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
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
			/*else
			{
				vertex.m_tex_coords = glm::vec2(0.0f, 0.0f);
			}*/

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
		std::vector<Texture>& diffuse_maps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
		// 2. specular maps
		std::vector<Texture>& specular_maps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
		// 3. normal maps
		std::vector<Texture> normal_maps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normal_maps.begin(), normal_maps.end());
		// 4. height maps
		std::vector<Texture> height_maps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), height_maps.begin(), height_maps.end());

		// return a mesh object created from the extracted mesh data
		return { vertices, indices, textures };
	}

	auto Model::LoadMaterialTextures(const aiMaterial* material, const aiTextureType type, const std::string_view type_name) -> std::vector<Texture>&
	{
		auto textures = std::vector<Texture>{};
		for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
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
			{   // if texture hasn't been loaded already, load it
				Texture texture;
				texture.m_id = TextureFromFile(str.C_Str(), m_directory.data(), true);
				texture.m_type = type_name;
				texture.m_path = str.C_Str();
				textures.push_back(texture);
				textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
			}
		}
		return textures;
	}
}
