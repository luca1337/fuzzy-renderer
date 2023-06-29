#pragma once

#include <components/component.h>
#include <components/transform.h>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <string>
#include <exception>
#include <ranges>

namespace libgraphics
{
	class Entity
	{
	public:
		Entity() { AddComponent<Transform>(); }

		template <std::derived_from<Component> Component>
		auto& AddComponent()
		{
			static_assert(std::is_base_of_v<Component, Component>, "T must derive from Component");

			const auto& component_type = typeid(Component);

			if (m_components.contains(component_type))
			{
				throw std::exception(("Component of type " + std::string(component_type.name()) + " already exists.").c_str());
			}

			auto new_component = std::make_shared<Component>();
			auto& new_component_ref = *new_component;
			new_component_ref.SetEntity(this);
			new_component_ref.Initialize();

			m_components[component_type] = std::move(new_component);

			return new_component_ref;
		}

		template <std::derived_from<Component> Component>
		auto& AddComponent(const std::shared_ptr<Component>& component)
		{
			static_assert(std::is_base_of_v<Component, Component>, "T must derive from Component");

			const auto& component_type = typeid(Component);

			if (m_components.contains(component_type))
			{
				throw std::exception(("Component of type " + std::string(component_type.name()) + " already exists.").c_str());
			}

			auto& new_component_ref = *component;
			new_component_ref.SetEntity(this);
			new_component_ref.Initialize();

			m_components[component_type] = component;

			return new_component_ref;
		}

		template <typename ComponentType>
		[[nodiscard]] auto& GetComponent() const
		{
			const auto& component_type = typeid(ComponentType);
			if (const auto& it = m_components.find(component_type); it != m_components.end())
			{
				return *std::static_pointer_cast<ComponentType>(it->second);
			}
			throw std::exception(("Component of type " + std::string(component_type.name()) + " does not exist.").c_str());
		}

		template <std::derived_from<Component> Component>
		auto RemoveComponent()
		{
			const auto& component_type = typeid(Component);
			m_components.erase(component_type);
		}

		auto Update(float delta_time) const -> void;

		auto Render() const -> void;

		[[nodiscard]] auto& GetTransformComponent() const { return GetComponent<Transform>(); }

	private:
		std::unordered_map<std::type_index, std::shared_ptr<Component>> m_components;
	};
}