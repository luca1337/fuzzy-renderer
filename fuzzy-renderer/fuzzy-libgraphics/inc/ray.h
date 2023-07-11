#pragma once
#include <glm/vec3.hpp>

namespace libgraphics
{
	struct Ray
	{
		glm::vec3 m_origin = {};
		glm::vec3 m_direction = {};
	};
}
