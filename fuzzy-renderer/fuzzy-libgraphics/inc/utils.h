#pragma once

#include <logger.h>
#include <glm/vec3.hpp>

namespace libgraphics
{
	class GLMesh;
	struct RayHit;
	struct Vertex;
}

namespace utils::gl
{
	/**
	 * \brief Generate 3D skybox effect
	 * \param folder_path folder path containing skyboxes (top.jpg, bottom.jpg etc..)
	 * \return id of the texture which will be drawn
	 */
	auto LoadCubemap(const std::string_view folder_path) -> uint32_t;

	/**
	 * \brief Checks for ray-mesh intersection and returns the closest hit point.
	 * \param ray_origin The origin of the ray.
	 * \param ray_direction The direction of the ray.
	 * \param mesh The mesh to check for intersection against.
	 * \return An optional `RayHit` struct representing the closest hit point, if any.
	 */
	auto CheckRayMeshIntersection(const glm::vec3& ray_origin, const glm::vec3& ray_direction, const libgraphics::GLMesh& mesh) -> std::optional<libgraphics::RayHit>;
}

namespace utils::common
{
	template <typename T, size_t N>
	constexpr size_t ArraySize(T(&)[N]) { return N; }
}