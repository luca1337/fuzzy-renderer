#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

namespace libgraphics
{
	class Transform
	{
	public:
		glm::vec3 m_translation = {};
		glm::vec3 m_scale = {};
		glm::quat m_rotation = {};
	};
}
