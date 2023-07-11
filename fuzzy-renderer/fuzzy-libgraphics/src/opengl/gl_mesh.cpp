#include <opengl/gl_mesh.h>

#include <loaders.h>
#include <ranges>
#include <glm/gtx/intersect.hpp>
#include <opengl/camera.h>

#include "rendering/light.h"

#include <entities/entity.h>

#include <core.h>

#include <utils.h>

namespace libgraphics
{
	GLMesh::GLMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
		: m_vertices{ std::move(vertices) }, m_indices{ std::move(indices) }
	{ }

	GLMesh::GLMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures, std::string name = "")
		: m_vertices{ std::move(vertices) }, m_indices{ std::move(indices) }, m_textures{ std::move(textures) }, m_name{
			  std::move(name)
		  }
	{
		GenerateMeshDataAndSendToGPU();
	}

	auto GLMesh::Draw(const std::shared_ptr<IShader>& shader) -> void
	{
		const auto& core = Core::GetInstance();
		const auto light_buffer_size = core.GetLights().size();
		const auto light_buffer_id = shader->GetLightsBufferID();

		glBindBuffer(GL_UNIFORM_BUFFER, light_buffer_id);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, light_buffer_size * sizeof(Light), &core.GetLights().data()->get()[0]);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
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
