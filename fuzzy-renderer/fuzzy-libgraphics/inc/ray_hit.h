#pragma once
#include <glm/vec3.hpp>

namespace libgraphics
{
	struct RayHit
	{
		float m_distance = {};
		glm::vec3 m_hit_point = {};
	};
}
