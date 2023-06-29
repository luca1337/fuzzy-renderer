#pragma once

namespace libgraphics
{
    class Entity;

    class Component
    {
    public:
        Component() = default;
        virtual ~Component() = default;

        virtual auto Initialize() -> void {}
        virtual auto Update(float delta_time) -> void {}
        virtual auto Render() -> void {}

        [[nodiscard]] auto GetEntity() const -> Entity&;
        auto SetEntity(Entity* entity) -> void;

        bool m_active = {};

    protected:
        Entity* m_entity = {};
    };
}