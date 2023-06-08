#include <opengl/gl_mesh.h>

namespace libgraphics
{
	GLMesh::GLMesh(const std::string_view file_name)
	{
	}

	GLMesh::GLMesh(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& uvs)
	{
		GenerateVaoAndVbo(1, &m_vao, 3, m_vbos);

		SendGPUData(vertices.data(), sizeof(vertices[0]) * vertices.size(), 0, 0, 3, sizeof(float) * 3, 0);
		SendGPUData(normals.data(), sizeof(normals[0]) * normals.size(), 1, 1, 3, sizeof(float) * 3, 1);
		SendGPUData(uvs.data(), sizeof(uvs[0]) * uvs.size(), 2, 2, 2, sizeof(float) * 2, 2);

		SetVertices(vertices);
		SetNormals(normals);
		SetUvs(uvs);
	}

	auto GLMesh::SendGPUData(const void* data, const int data_size, const size_t vbo_index, const unsigned slot,
	                         const int slot_size, const size_t stride, const unsigned attrib_array_index) -> void
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbos[vbo_index]);
		glBufferData(GL_ARRAY_BUFFER, data_size, data, GL_STATIC_DRAW);
		glVertexAttribPointer(slot, slot_size, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(stride), nullptr);
		glEnableVertexAttribArray(attrib_array_index);
	}

	auto GLMesh::GenerateVaoAndVbo(const int vao_size, unsigned* vao_array, const int vbo_size,
	                               unsigned* vbo_array) -> void
	{
		glGenVertexArrays(vao_size, vao_array);
		glBindVertexArray(*vao_array);
		glGenBuffers(vbo_size, vbo_array);
	}
}
