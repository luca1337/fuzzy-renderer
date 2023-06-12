#include <opengl/gl_mesh.h>

#include <core.h>
#include <opengl/camera.h>
#include <opengl/gl_context.h>
#include <loaders.h>

#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace libgraphics
{
	/**
	 * \brief Create a raw mesh
	 * \param vertices  
	 * \param indices  
	 * \param textures  
	 */
	GLMesh::GLMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures)
		: m_vertices{std::move(vertices)}, m_indices{std::move(indices)}, m_textures{std::move(textures)}
	{
		GenerateMeshDataAndSendToGPU();
	}

	auto GLMesh::Draw(const std::shared_ptr<IShader>& shader) -> void
	{
		shader->Bind();

		// draw textures here: glBindTexture(GL_TEXTURE_2D, texture);

		glBindVertexArray(m_vao);

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(0);

		auto transform = Transform{};
		transform.m_scale = { 100, 100, 100 };

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

		glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);
	}

	auto GLMesh::GenerateIndexBuffer() const -> void
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(uint32_t), m_indices.data(), GL_STATIC_DRAW);
	}

	auto GLMesh::GenerateMeshDataAndSendToGPU() -> void
	{
		GenerateVaoVboEbo();

		GenerateIndexBuffer();

		SendGPUData(0, 3, 0, nullptr);
		SendGPUData(1, 3, 1, reinterpret_cast<void*>(offsetof(Vertex, m_normal)));  // NOLINT(performance-no-int-to-ptr)
		SendGPUData(2, 2, 2, reinterpret_cast<void*>(offsetof(Vertex, m_tex_coords)));  // NOLINT(performance-no-int-to-ptr)
	}

	auto GLMesh::UpdateMatrix(const std::shared_ptr<IShader>& shader, const Transform& transform) -> void
	{
		const auto& core = Core::GetInstance();
		const auto gl_context = ::std::static_pointer_cast<GLContext>(core.GetGraphicsWindow()->GetNativeHandle());

		const auto translation_mat = glm::translate(transform.m_translation);
		const auto scale_mat = glm::scale(transform.m_scale);
		const auto rotation_mat = glm::mat4_cast(transform.m_rotation);

		const auto model = translation_mat * rotation_mat * scale_mat;
		const auto view = GetViewMatrix(core.GetMainCamera().m_camera_props);
		const auto projection = ComputeCameraProjection(60.0, gl_context->Data().m_width, gl_context->Data().m_height, 0.01, 1000.0);
		const auto eye = core.GetMainCamera().GetWorldPosition();

		shader->SetMatrix4x4("model", model);
		shader->SetMatrix4x4("view", view);
		shader->SetMatrix4x4("projection", projection);
		shader->SetVec3("eye", eye);
	}
}
