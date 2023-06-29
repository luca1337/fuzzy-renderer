#include <opengl/gl_mesh.h>

#include <core.h>
#include <loaders.h>
#include <ranges>
#include <glm/gtx/intersect.hpp>
#include <opengl/camera.h>
#include <opengl/gl_context.h>

#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

#include "rendering/light.h"
#include "rendering/material.h"

#include <entities/entity.h>

namespace libgraphics
{
	GLMesh::GLMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures)
		: m_vertices{ std::move(vertices) }, m_indices{ std::move(indices) }, m_textures{ std::move(textures) }
	{
		GenerateMeshDataAndSendToGPU();
	}

	Light lights[3] = {};
	GLuint lightsBuffer;
	float attenuation_sin_wave = {};
	float dlt = {};
	glm::vec3 new_pos = { };

	auto GLMesh::Draw(const std::shared_ptr<IShader>& shader) -> void
	{
		shader->Bind();

#pragma region TEXTURES

		auto material = libgraphics::lighting::Material{};
		material.m_shininess = 32.0f;
		material.m_roughness = 0.0f;
		material.m_use_textures = !m_textures.empty();

		std::ranges::for_each(std::views::iota(0ul) | std::views::take(m_textures.size()), [&](const auto texture_idx) {
			glActiveTexture(GL_TEXTURE0 + texture_idx);

			const auto& [m_id, m_type, m_path] = m_textures[texture_idx];

			if (m_type == "texture_diffuse") { material.m_diffuse_map = texture_idx; }
			else if (m_type == "texture_specular") { material.m_specular_map = texture_idx; }
			else if (m_type == "texture_normal") { material.m_normal_map = texture_idx; }
			else if (m_type == "texture_height") { material.m_height_map = texture_idx; }
			else if (m_type == "texture_ambient") { material.m_ambient_map = texture_idx; }
			else if (m_type == "texture_emissive") { material.m_emissive_map = texture_idx; }
			else if (m_type == "texture_opacity") { material.m_opacity_map = texture_idx; }
			else if (m_type == "texture_displacement") { material.m_displacement_map = texture_idx; }
			else if (m_type == "texture_reflection") { material.m_reflection_map = texture_idx; }

			glBindTexture(GL_TEXTURE_2D, m_id);
		});

		shader->SetInt("material.diffuse", material.m_diffuse_map);
		shader->SetInt("material.specular", material.m_specular_map);
		shader->SetInt("material.normal", material.m_normal_map);

		shader->SetFloat("material.shininess", material.m_shininess);
		shader->SetFloat("material.roughness", material.m_roughness);
		shader->SetBool("material.useTextures", material.m_use_textures);

#pragma endregion

#pragma region LIGHTS

		const GLuint lights_block_index = glGetUniformBlockIndex(shader->GetID(), "LightsBlock");

		GLint binding_point;
		glGetActiveUniformBlockiv(shader->GetID(), lights_block_index, GL_UNIFORM_BLOCK_BINDING, &binding_point);

		static bool first = false;
		if (!first)
		{
			// point
			lights[0].m_position = glm::vec3(0.0f, 0.0f, 0.0f);
			lights[0].m_attenuation = glm::vec3(1.0f, 0.09f, 0.032f);
			lights[0].m_direction = glm::vec3{ -0.5, 0.4, 0.0 };
			lights[0].m_intensity = 15.1f;
			lights[0].m_color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
			lights[0].m_type = 1;
			lights[0].m_is_active = true;

			// spot
			lights[1].m_position = glm::vec3(28.0f, 94.0f, 36.0f);
			lights[1].m_attenuation = glm::vec3(1.0f, 0.09f, 0.032f);
			lights[1].m_direction = glm::vec3{ -0.8, 0.7f, 0.0 };
			lights[1].m_intensity = 45.1f;
			lights[1].m_color = glm::vec4(1.0f, 0.0f, 1.0f, 0.0f);
			lights[1].m_type = 2;
			lights[1].m_is_active = true;

			// dir
			lights[2].m_direction = glm::vec3{ 0.7f, 0.7f, 0.0 };
			lights[2].m_intensity = 2.0f;
			lights[2].m_color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
			lights[2].m_type = 0;
			lights[2].m_is_active = true;

			glGenBuffers(1, &lightsBuffer);

			// Carica i dati nel buffer
			glBindBuffer(GL_UNIFORM_BUFFER, lightsBuffer);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(Light) * 3, nullptr, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);

			glBindBufferRange(GL_UNIFORM_BUFFER, 0, lightsBuffer, 0, 3 * sizeof(Light));

			first = true;
		}

		glBindBuffer(GL_UNIFORM_BUFFER, lightsBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, 3 * sizeof(Light), &lights[0]);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

#pragma endregion

		glBindVertexArray(m_vao);

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);
	}

	auto GLMesh::SendGPUData(const unsigned slot, const int slot_size, const unsigned attrib_array_index, const void* ptr) -> void
	{
		glVertexAttribPointer(slot, slot_size, GL_FLOAT, GL_FALSE, sizeof Vertex, ptr);
		glEnableVertexAttribArray(attrib_array_index);
	}

	auto GLMesh::GenerateVaoVboEbo() -> void
	{
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ebo);

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(m_vertices.size() * sizeof(Vertex)), m_vertices.data(), GL_STATIC_DRAW);
	}

	auto GLMesh::GenerateIndexBuffer() const -> void
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizei>(m_indices.size() * sizeof(uint32_t)), m_indices.data(), GL_STATIC_DRAW);
	}

	auto GLMesh::GenerateMeshDataAndSendToGPU() -> void
	{
		GenerateVaoVboEbo();

		GenerateIndexBuffer();

		SendGPUData(0, 3, 0, nullptr);
		SendGPUData(1, 3, 1, reinterpret_cast<void*>(offsetof(Vertex, m_normal)));
		SendGPUData(2, 2, 2, reinterpret_cast<void*>(offsetof(Vertex, m_tex_coords)));
		SendGPUData(3, 3, 3, reinterpret_cast<void*>(offsetof(Vertex, m_tangent)));
		SendGPUData(4, 3, 4, reinterpret_cast<void*>(offsetof(Vertex, m_bitangent)));
	}
}
