#pragma once

#include <framework.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace libgraphics
{
    class Transform;
    class IShader;

    class IMesh
    {
    public:
        virtual ~IMesh();

        virtual auto UpdateMatrix(const std::shared_ptr<Transform>& transform) -> void = 0;
        virtual auto Draw() -> void = 0;
        virtual auto SetShader(const std::shared_ptr<IShader>& shader) -> void = 0;
        virtual auto GetShader() const -> const IShader& = 0;

        virtual auto GetVertices() const -> std::vector<glm::vec3> = 0;
        virtual auto GetVertexNormals() const -> std::vector<glm::vec3> = 0;
        virtual auto GetUvs() const -> std::vector<glm::vec2> = 0;

        virtual auto SetVertices(const std::vector<glm::vec3>& vertices) -> void = 0;
        virtual auto SetNormals(const std::vector<glm::vec3>& normals) -> void = 0;
        virtual auto SetUvs(const std::vector<glm::vec2>& uvs) -> void = 0;
    };
}