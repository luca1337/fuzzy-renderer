#pragma once

namespace libgraphics::lighting
{
    struct Material
    {
        int m_diffuse_map = {};
        int m_specular_map = {};
        int m_normal_map = {};
        int m_height_map = {};
        int m_ambient_map = {};
        int m_emissive_map = {};
        int m_opacity_map = {};
        int m_displacement_map = {};
        int m_reflection_map = {};

        float m_shininess = {};
        float m_roughness = {};

        bool m_use_textures = {};
    };
}