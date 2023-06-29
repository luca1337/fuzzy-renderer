#include <components/component.h>
#include <entities/entity.h>

namespace libgraphics
{
	auto Component::SetEntity(Entity* entity) -> void { m_entity = entity; }

	auto Component::GetEntity() const -> Entity& { return *m_entity; }
}
