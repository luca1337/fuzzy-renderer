#include <opengl/gl_mesh.h>

#include <core.h>
#include <opengl/camera.h>
#include <opengl/gl_context.h>
#include <loaders.h>
#include <map>
#include <ranges>

#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "rendering/material.h"

namespace libgraphics
{
	/**
	 * \brief Create a raw mesh
	 * \param vertices
	 * \param indices
	 * \param textures
	 */
	GLMesh::GLMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures)
		: m_vertices{ std::move(vertices) }, m_indices{ std::move(indices) }, m_textures{ std::move(textures) }
	{
		GenerateMeshDataAndSendToGPU();
	}

	auto GLMesh::Draw(const std::shared_ptr<IShader>& shader) -> void
	{
		shader->Bind();

		auto material = libgraphics::lighting::Material{};
		material.m_shininess = 32.0f;
		material.m_roughness = 0.44f;

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

		glBindVertexArray(m_vao);

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);

		auto transform = Transform{};
		transform.m_scale = { 0.5, 0.5, 0.5 };

		UpdateMatrix(shader, transform);
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

	auto GLMesh::UpdateMatrix(const std::shared_ptr<IShader>& shader, const Transform& transform) -> void
	{
		const auto& core = Core::GetInstance();
		const auto gl_context = ::std::static_pointer_cast<GLContext>(core.GetGraphicsWindow()->GetNativeHandle());

		const auto& translation_mat = glm::translate(transform.m_translation);
		const auto& scale_mat = glm::scale(transform.m_scale);
		const auto& rotation_mat = glm::mat4_cast(transform.m_rotation);

		const auto& model = translation_mat * rotation_mat * scale_mat;
		const auto& view = GetViewMatrix(core.GetMainCamera().m_camera_props);
		const auto& projection = ComputeCameraProjection(60.0, gl_context->Data().m_width, gl_context->Data().m_height, 0.01, 1000.0);
		const auto& eye = core.GetMainCamera().GetWorldPosition();

		shader->SetMatrix4x4("model", model);
		shader->SetMatrix4x4("view", view);
		shader->SetMatrix4x4("projection", projection);
		shader->SetVec3("eye", eye);
	}
}
