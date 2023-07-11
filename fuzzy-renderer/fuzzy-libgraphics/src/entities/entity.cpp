#include <ranges>
#include <entities/entity.h>

namespace libgraphics
{
	auto Entity::Render() -> void
	{
		for (auto&& component : std::views::values(m_components))
		{
			component->Render();
		}

		for (auto&& child : m_childrens)
		{
			child->Render();
		}
	}

	auto Entity::AddChild(const std::shared_ptr<Entity>& child) -> void
	{
		child->m_parent = this;
		m_childrens.push_back(child);
	}

	auto Entity::Update(const float delta_time) -> void
	{
		for (auto&& component_ptr : std::views::values(m_components))
		{
			component_ptr->Update(delta_time);
		}

		UpdateSelfAndChild();

		for (auto&& child : m_childrens)
		{
			child->Update(delta_time);
		}
	}

	auto Entity::UpdateSelfAndChild() const -> void
	{
		const auto& transform = GetTransformComponent();

		if (transform->IsDirty())
		{
			ForceUpdateSelfAndChild();
			return;
		}

		for (auto&& child : m_childrens)
		{
			child->UpdateSelfAndChild();
		}
	}

	auto Entity::ForceUpdateSelfAndChild() const -> void
	{
		const auto& transform = GetTransformComponent();

		if (m_parent)
		{
			transform->ComputeModelMatrix(m_parent->GetTransformComponent()->GetWorldModelMatrix());
		}
		else
		{
			transform->ComputeModelMatrix();
		}

		for (auto&& child : m_childrens)
		{
			child->ForceUpdateSelfAndChild();
		}
	}
}
