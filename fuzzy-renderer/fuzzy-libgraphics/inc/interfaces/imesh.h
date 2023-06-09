#pragma once

#include <framework.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace libgraphics
{
    class IShader;

    struct Vertex
    {
        glm::vec3 m_position = {};
        glm::vec3 m_normal = {};
        glm::vec2 m_tex_coords = {};
        glm::vec3 m_tangent = {};
        glm::vec3 m_bitangent = {};
    };

    class LIBGRAPHICS_API IMesh
    {
    public:
        virtual ~IMesh() = default;

        virtual auto Draw(const std::shared_ptr<IShader>& shader) -> void = 0;

        [[nodiscard]] virtual auto GetVertexBuffer() const->std::vector<Vertex> = 0;
        [[nodiscard]] virtual auto GetIndexBuffer() const -> std::vector<uint32_t> = 0;

        virtual auto SetVertexBuffer(const std::vector<Vertex>& vertex_buffer) -> void = 0;
        virtual auto SetIndexBuffer(const std::vector<uint32_t>& index_buffer) -> void = 0;
    };
}