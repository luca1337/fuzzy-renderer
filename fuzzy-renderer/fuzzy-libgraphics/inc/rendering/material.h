#pragma once

#include "texture.h"

namespace libgraphics::lighting
{
    struct Material
    {
        int m_albedo_map{};
        int m_normal_map{};
        int m_metallic_map{};
        int m_roughness_map{};
        int m_occlusion_map{};
        int m_emission_map{};
        int m_height_map{};
        int m_opacity_map{};
        int m_specular_map{};

        float m_metallic{};
        float m_roughness{};
        float m_occlusion_strength{};
        float m_emission_strength{};

        glm::vec3 m_albedo_color{ 1.0f };
        glm::vec3 m_emission_color{ 1.0f };

        bool m_use_textures{};
    };
}
