#include <utils.h>

#include <filesystem>
#include <logger.h>
#include <stb_image.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/gtx/intersect.hpp>

#include <core.h>
#include <interfaces/imesh.h>

#include <opengl/gl_mesh.h>

#include <ray_hit.h>

namespace utils::gl
{
	auto CompareFileNames(const std::string& file_name1, const std::string& file_name2)
	{
		auto number_string1 = std::string{};
		auto number_string2 = std::string{};

		for (const auto& c : file_name1) 
		{
			if (std::isdigit(c)) 
			{
				number_string1 += c;
			}
		}

		for (const auto& c : file_name2)
		{
			if (std::isdigit(c)) 
			{
				number_string2 += c;
			}
		}

		const int number1 = std::stoi(number_string1);
		const int number2 = std::stoi(number_string2);

		return number1 < number2;
	}

	// loads a cubemap texture from 6 individual texture faces
	// order:
	// +X (right)
	// -X (left)
	// +Y (top)
	// -Y (bottom)
	// +Z (front) 
	// -Z (back)
	// -------------------------------------------------------
	auto LoadCubemap(const std::string_view folder_path) -> uint32_t
	{
		auto faces = std::vector<std::string>{};

		const auto allowed_extensions = std::vector<std::string>{ ".jpg", ".png", ".jpeg" };

		for (const auto& entry : std::filesystem::directory_iterator(folder_path)) 
		{
			if (entry.is_regular_file()) 
			{
				auto extension = entry.path().extension().string();
				std::ranges::transform(extension, extension.begin(), [](const auto& c) { return std::tolower(c); });

				if (std::ranges::find(allowed_extensions, extension) != allowed_extensions.end()) 
				{
					auto file_path = entry.path().string();
					std::ranges::replace(file_path, '\\', '/');
					faces.push_back(file_path);
				}
			}
		}

		// sort faces with given order (below)
		std::ranges::sort(faces, CompareFileNames);

		auto texture_id = uint32_t{};
		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

		int width = {}, height = {}, channels = {};

		for (auto face_idx = 0ul; face_idx != faces.size(); ++face_idx)
		{
			if (const auto data = stbi_load(faces[face_idx].c_str(), &width, &height, &channels, 0))
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face_idx, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			else
			{
				CX_CORE_ERROR("Cubemap tex failed to load at path: " + faces[face_idx]);
				return -1;
			}
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		return texture_id;
	}

	auto CheckRayMeshIntersection(const glm::vec3& ray_origin, const glm::vec3& ray_direction, const libgraphics::GLMesh& mesh) -> std::optional<libgraphics::RayHit>
	{
		auto closest_distance = std::numeric_limits<float>::max();
		auto ray_hit = libgraphics::RayHit{};

		const auto& index_buffer = mesh.GetIndexBuffer();
		const auto& vertex_buffer = mesh.GetVertexBuffer();

		for (auto vertex_idx = 0ul; vertex_idx < index_buffer.size(); vertex_idx += 3)
		{
			const auto& v0 = vertex_buffer[index_buffer[vertex_idx]];
			const auto& v1 = vertex_buffer[index_buffer[vertex_idx + 1]];
			const auto& v2 = vertex_buffer[index_buffer[vertex_idx + 2]];

			auto barycentric_coords = glm::vec2{};
			if (auto distance = 0.0f; glm::intersectRayTriangle(ray_origin, ray_direction, v0.m_position, v1.m_position, v2.m_position, barycentric_coords, distance))
			{
				const auto& intersection_point = ray_origin + ray_direction * distance;
				if (distance < closest_distance)
				{
					closest_distance = distance;
					ray_hit = { closest_distance, intersection_point };
				}
			}
		}
		return ray_hit;
	}
}
