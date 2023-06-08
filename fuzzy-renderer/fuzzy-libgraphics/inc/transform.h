#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

namespace libgraphics
{
	struct Transform
	{
		glm::dvec3 m_translation = {};
		glm::dvec3 m_scale = {};
		glm::dquat m_rotation = {};
	};
}
