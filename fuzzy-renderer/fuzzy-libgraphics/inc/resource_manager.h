#pragma once

#include <string_view>
#include <memory>
#include <unordered_map>

namespace libgraphics
{
	class IShader;
}

namespace libgraphics::resources
{
	enum class ResourceType
	{
		shader
	};

	template <typename Resource>
	concept ResourceConcept = requires(Resource resource)
	{
		std::is_base_of_v<IShader, Resource>;
	};

	class ResourceManager
	{
	public:
		template <ResourceConcept Resource>
		auto LoadResource(const ResourceType type, const std::string_view file_name);

	private:
		template <ResourceConcept Resource>
		static std::unordered_map<std::string, std::shared_ptr<Resource>> m_resource_cache = {};

		[[nodiscard]] auto GetFullPath(const ResourceType) const -> std::string_view;
	};

	template <ResourceConcept Resource>
	auto ResourceManager::LoadResource(const ResourceType type, const std::string_view file_name)
	{
		const auto& full_resource_path = GetFullPath(type);

	}
}
