#pragma once

#include <interfaces/imesh.h>
#include <transform.h>
#include <opengl/gl_shader.h>

namespace libgraphics
{
	class GLMesh final : public IMesh
	{
	public:

		/**
		 * \brief Creates raw mesh
		 * \param vertices vertices of mesh
		 * \param normals normals of mesh
		 * \param uvs uvs of mesh
		 */
		LIBGRAPHICS_API GLMesh(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& uvs);

		/**
		 * \brief Loads obj mesh from file
		 * \param file_name Path to obj file
		 */
		LIBGRAPHICS_API explicit GLMesh(const std::string_view file_name);

		/**
		 * \brief Standard Draw for a mesh 
		 * \param shader shader you want to bind
		 */
		LIBGRAPHICS_API auto Draw(const std::shared_ptr<IShader>& shader) -> void override;

		/**
		 * \brief Get all vertices of this mesh
		 * \return Value reference vector containing all mesh vertices
		 */
		LIBGRAPHICS_API  [[nodiscard]] auto GetVertices() const -> std::vector<glm::vec3> override { return m_vertices; }

		/**
		 * \brief Get all normals of this mesh
		 * \return Value reference vector containing all mesh normals
		 */
		LIBGRAPHICS_API  [[nodiscard]] auto GetVertexNormals() const -> std::vector<glm::vec3> override { return m_normals; }

		/**
		 * \brief Get all uvs of this mesh
		 * \return Value reference vector containing all mesh uvs
		 */
		LIBGRAPHICS_API  [[nodiscard]] auto GetUvs() const -> std::vector<glm::vec2> override { return m_uvs; }

		/**
		 * \brief Set vertices of this mesh
		 * \return 
		 */
		LIBGRAPHICS_API auto SetVertices(const std::vector<glm::vec3>& vertices) -> void override { m_vertices = vertices; }

		/**
		 * \brief Set normals of this mesh
		 * \return 
		 */
		LIBGRAPHICS_API auto SetNormals(const std::vector<glm::vec3>& normals) -> void override { m_normals = normals; }

		/**
		 * \brief Set uvs of this mesh
		 * \return 
		 */
		LIBGRAPHICS_API auto SetUvs(const std::vector<glm::vec2>& uvs) -> void override { m_uvs = uvs; }

	protected:
		auto UpdateMatrix(const std::shared_ptr<IShader>& shader, const Transform& transform) -> void override;

	private:
		std::vector<glm::vec3> m_vertices = {};
		std::vector<glm::vec3> m_normals = {};
		std::vector<glm::vec2> m_uvs = {};

		unsigned int m_vao = {};
		unsigned int m_vbos[3] = {};

		auto SendGPUData(const void* data, const int data_size, const size_t vbo_index, const unsigned slot, const int slot_size, const size_t stride, const unsigned attrib_array_index) const -> void;
		static auto GenerateVaoAndVbo(const int vao_size, unsigned int* vao_array, const int vbo_size, unsigned int* vbo_array) -> void;
		auto GenerateMeshDataAndSendToGPU(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& uvs) -> void;

	};
}
