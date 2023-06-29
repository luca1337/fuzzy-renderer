#pragma once

#include <entities/entity.h>

#include <components/mesh_renderer.h>

namespace libgraphics
{
	class MeshEntity : public Entity
	{
	public:
		MeshEntity() { AddComponent<MeshRenderer>(); }
	};
}
