#pragma once

#include <interfaces/imesh.h>
#include <opengl/gl_shader.h>
#include <rendering/texture.h>

namespace libgraphics
{
	class Entity;

	class GLMesh final : public IMesh
	{
	public:

		/**
		 * \brief Default constructor of Mesh (use SetVertexBuffer and SetIndexBuffer)
		 */
		LIBGRAPHICS_API GLMesh() = default;

		/**
		 * \brief Creates raw mesh
		 * \param vertices vertices of mesh
		 * \param indices indices of mesh
		 */
		LIBGRAPHICS_API GLMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices);

		/**
		 * \brief Creates raw mesh
		 * \param vertices vertices of mesh
		 * \param indices indices of mesh
		 * \param textures textures of mesh (albedo, metallic, roughness etc..)
		 */
		LIBGRAPHICS_API GLMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices,
		                       std::vector<Texture> textures);

		/**
		 * \brief Standard Draw for a mesh 
		 * \param shader shader you want to bind
		 */
		LIBGRAPHICS_API auto Draw(const std::shared_ptr<IShader>& shader) -> void override;

		/**
		 * \brief Get this mesh vertex buffer
		 * \return Value reference vector containing all mesh vertices
		 */
		LIBGRAPHICS_API  [[nodiscard]] auto GetVertexBuffer() const -> std::vector<Vertex> override { return m_vertices; }

		/**
		 * \brief Get this mesh index buffer
		 * \return Value reference vector containing all mesh normals
		 */
		LIBGRAPHICS_API  [[nodiscard]] auto GetIndexBuffer() const -> std::vector<uint32_t> override { return m_indices; }

		/**
		 * \brief Set this mesh vertex buffer
		 */
		LIBGRAPHICS_API auto SetVertexBuffer(const std::vector<Vertex>& vertices) -> void override { m_vertices = vertices; }

		/**
		 * \brief Set this mesh index buffer
		 */
		LIBGRAPHICS_API auto SetIndexBuffer(const std::vector<uint32_t>& indices) -> void override { m_indices = indices; }

		/**
		 * \brief Get this mesh texture buffer
		 * \return Value reference vector containing all mesh textures (if any)
		 */
		LIBGRAPHICS_API [[nodiscard]] auto GetTextures() -> std::vector<Texture> { return m_textures; }

	private:
		std::vector<Vertex> m_vertices = {};
		std::vector<uint32_t> m_indices = {};
		std::vector<Texture> m_textures = {};

		unsigned int m_vao = {};
		unsigned int m_vbo = {};
		unsigned int m_ebo = {};

		static auto SendGPUData(const unsigned slot, const int slot_size, const unsigned attrib_array_index, const void* ptr) -> void;
		auto GenerateVaoVboEbo() -> void;
		auto GenerateMeshDataAndSendToGPU() -> void;
		auto GenerateIndexBuffer() const -> void;
	};
}
