#pragma once

namespace libgraphics
{
	enum class GraphicsAPI
	{
		opengl,
		directx
		// add more..
	};

	enum class LightType
	{
		directional,
		point,
		spot
	};

	enum class ResourceType
	{
		shaders,
		textures,
		materials,

		max_enum
	};

	inline auto ResourceTypeToString(const ResourceType type) -> std::string
	{
		switch (type)
		{
		case ResourceType::shaders: return "Shaders";
		case ResourceType::textures: return "Textures";
		case ResourceType::materials: return "Materials";
		case ResourceType::max_enum: return "Invalid";
		default: break;
		}
		return {};
	}
}
