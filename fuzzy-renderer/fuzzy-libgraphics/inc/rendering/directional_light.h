#pragma once

#include <glm/vec3.hpp>

namespace libgraphics::lighting
{
	struct DirectionalLight
	{
		glm::vec3 m_direction = {};
		glm::vec3 m_ambient = {};
		glm::vec3 m_diffuse = {};
		glm::vec3 m_specular = {};
	};
}