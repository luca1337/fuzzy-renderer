#pragma once

#include <components/component.h>
#include <transform.h>
#include <vector>
#include <memory>

namespace libgraphics
{
	class Entity
	{
	public:
		Entity() = default;

        template <std::derived_from<Component> Component>
        auto& AddComponent()
		{
            m_components.emplace_back(std::make_unique<Comp>());
            Component& new_component = *m_components.back();
            new_component.SetEntity(this);
            new_component.Initialize();

            return new_component;
        }

        template <std::derived_from<Component> Component>
        auto RemoveComponent()
        {
            m_components.erase(std::remove_if(m_components.begin(), m_components.end(), [](const auto& component) {
                return std::dynamic_pointer_cast<Component*>(component.get()) != nullptr;
            }), m_components.end());
        }

		auto TransformData() const -> const Transform& { return m_transform; }
        auto TransformData() -> Transform& { return m_transform; }

	private:
        Transform m_transform;
		std::vector<std::unique_ptr<Component>> m_components = {};
	};
}
