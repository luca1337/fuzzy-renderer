#pragma once

#include <memory>
#include <components/component.h>
#include <opengl/gl_mesh.h>

#include <rendering/material.h>

namespace libgraphics
{
	class MeshRenderer : public Component
    {
    public:
        MeshRenderer(GLMesh mesh) : m_mesh(std::move(mesh)) {}
        auto Initialize() -> void override;
        auto Render() -> void override;

        [[nodiscard]] auto& GetMesh() const { return m_mesh; }
        auto SetMesh(const GLMesh& mesh) -> void { m_mesh = mesh; }

        [[nodiscard]] auto& GetShader() const { return m_shader; }
        auto SetShader(const std::shared_ptr<IShader>& shader) -> void { m_shader = shader; }

        [[nodiscard]] auto& GetMaterial() const { return m_default_material; }
        auto SetMaterial(const std::shared_ptr<lighting::Material>& material) -> void { m_default_material = material; }

    private:

        auto UpdateMatrix() const -> void;

        GLMesh m_mesh;
        std::shared_ptr<IShader> m_shader = {};
        std::shared_ptr<lighting::Material> m_default_material = {};

        GLuint lights_buffer = {};
    };
}
