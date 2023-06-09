#include <opengl/gl_mesh.h>

#include <core.h>
#include <opengl/camera.h>
#include <opengl/gl_context.h>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace libgraphics
{
	GLMesh::GLMesh(const std::string_view file_name)
	{
	}

	GLMesh::GLMesh(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& uvs)
	{
		GenerateVaoAndVbo(1, &m_vao, 3, m_vbos);

		SendGPUData(vertices.data(), static_cast<int>(sizeof(vertices[0]) * vertices.size()), 0, 0, 3, sizeof(float) * 3, 0);
		SendGPUData(normals.data(), static_cast<int>(sizeof(normals[0]) * normals.size()), 1, 1, 3, sizeof(float) * 3, 1);
		SendGPUData(uvs.data(), static_cast<int>(sizeof(uvs[0]) * uvs.size()), 2, 2, 2, sizeof(float) * 2, 2);

		SetVertices(vertices);
		SetNormals(normals);
		SetUvs(uvs);
	}

	auto GLMesh::Draw(const std::shared_ptr<IShader>& shader) -> void
	{
		shader->Bind();

		// draw textures here: glBindTexture(GL_TEXTURE_2D, texture);

		glBindVertexArray(m_vao);

		glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_vertices.size()));

		UpdateMatrix(shader, {});
	}

	auto GLMesh::SendGPUData(const void* data, const int data_size, const size_t vbo_index, const unsigned slot,
	                         const int slot_size, const size_t stride, const unsigned attrib_array_index) const -> void
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbos[vbo_index]);
		glBufferData(GL_ARRAY_BUFFER, data_size, data, GL_STATIC_DRAW);
		glVertexAttribPointer(slot, slot_size, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(stride), nullptr);
		glEnableVertexAttribArray(attrib_array_index);
	}

	auto GLMesh::GenerateVaoAndVbo(const int vao_size, unsigned int* vao_array, const int vbo_size, unsigned int* vbo_array) -> void
	{
		glGenVertexArrays(vao_size, vao_array);
		glBindVertexArray(*vao_array);
		glGenBuffers(vbo_size, vbo_array);
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
