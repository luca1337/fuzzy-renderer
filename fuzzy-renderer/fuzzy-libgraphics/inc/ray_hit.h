#pragma once
#include <glm/vec3.hpp>

namespace libgraphics
{
	struct RayHit
	{
		float distance = {};
		glm::vec3 hit_point = {};
	};
}
