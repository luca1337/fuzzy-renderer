#pragma once

#include <interfaces/imesh.h>
#include <transform.h>
#include <opengl/gl_shader.h>

namespace libgraphics
{
	class GLMesh final : public IMesh
	{
	public:
		GLMesh(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& uvs);
		explicit GLMesh(const std::string_view file_name);

		auto UpdateMatrix(const std::shared_ptr<Transform>& transform) -> void override;
		auto Draw() -> void override;

		auto SetShader(const std::shared_ptr<IShader>& shader) -> void override;
		auto GetShader() const -> const IShader & override { return m_shader; }

		[[nodiscard]] auto GetVertices() const -> std::vector<glm::vec3> override { return m_vertices; }
		[[nodiscard]] auto GetVertexNormals() const -> std::vector<glm::vec3> override { return m_normals; }
		[[nodiscard]] auto GetUvs() const -> std::vector<glm::vec2> override { return m_uvs; }

		auto SetVertices(const std::vector<glm::vec3>& vertices) -> void override { m_vertices = vertices; }
		auto SetNormals(const std::vector<glm::vec3>& normals) -> void override { m_normals = normals; }
		auto SetUvs(const std::vector<glm::vec2>& uvs) -> void override { m_uvs = uvs; }

	private:
		GLShader m_shader = {};

		Transform m_transform = {};

		std::vector<glm::vec3> m_vertices = {};
		std::vector<glm::vec3> m_normals = {};
		std::vector<glm::vec2> m_uvs = {};

		unsigned int m_vao = {};
		unsigned int m_vbos[3] = {};

		auto SendGPUData(const void* data, const int data_size, const size_t vbo_index, const unsigned slot, const int slot_size, const size_t stride, const unsigned attrib_array_index) -> void;
		auto GenerateVaoAndVbo(const int vao_size, unsigned int* vao_array, const int vbo_size, unsigned int* vbo_array) -> void;
	};
}
