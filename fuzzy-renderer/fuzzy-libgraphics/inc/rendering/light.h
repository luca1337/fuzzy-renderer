#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace libgraphics
{
    __declspec(align(16)) struct Light final
    {
        glm::vec3 m_position = {};
        float _pad0 = {};
        glm::vec3 m_direction = {};
        float m_intensity = {};
        glm::vec3 m_attenuation = {};
        float _pad1 = {};
        glm::vec4 m_color = {};
        int m_type = {};
        int m_is_active = {};
    };
}