#pragma once

#include <exception>
#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <components/component.h>
#include <components/transform.h>

namespace libgraphics
{
	class Entity
	{
	public:
		virtual ~Entity() = default;
		Entity() { AddComponent<Transform>(); }

		auto AddChild(const std::shared_ptr<Entity>& child) -> void;
		[[nodiscard]] auto& GetChildrens() const { return m_childrens; }

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
		[[nodiscard]] auto GetComponent() const
		{
			const auto& component_type = typeid(ComponentType);
			if (const auto& it = m_components.find(component_type); it != m_components.end())
			{
				return std::static_pointer_cast<ComponentType>(it->second);
			}
			return std::shared_ptr<ComponentType>();
		}

		template <std::derived_from<Component> Component>
		auto RemoveComponent()
		{
			const auto& component_type = typeid(Component);
			m_components.erase(component_type);
		}

		virtual auto Update(float delta_time) -> void;
		virtual auto Render() -> void;

		auto SetName(const std::string& name) { m_name = name; }
		[[nodiscard]] auto& GetName() const { return m_name; }

		[[nodiscard]] auto GetTransformComponent() const { return GetComponent<Transform>(); }

	private:
		Entity* m_parent = {};
		std::vector<std::shared_ptr<Entity>> m_childrens = {};
		std::unordered_map<std::type_index, std::shared_ptr<Component>> m_components = {};
		std::string m_name = {};

		auto UpdateSelfAndChild() const -> void;
		auto ForceUpdateSelfAndChild() const -> void;
	};
}
