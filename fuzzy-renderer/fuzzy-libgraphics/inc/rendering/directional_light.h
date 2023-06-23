#pragma once

#include <glm/vec3.hpp>

#include <interfaces/ilight.h>

namespace libgraphics
{
	struct DirectionalLight final : public ILight
	{
		DirectionalLight() { m_type = LightType::directional; }

		glm::vec3 m_direction = {};
		glm::vec3 m_ambient = {};
		glm::vec3 m_diffuse = {};
		glm::vec3 m_specular = {};
	};
}
