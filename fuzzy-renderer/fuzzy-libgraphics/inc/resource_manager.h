#pragma once

#include <logger.h>

#include <memory>
#include <map>
#include <optional>

namespace libgraphics
{
	class IShader;
}

namespace libgraphics::resources
{
	enum class ResourceType
	{
		shader,
		texture
	};

	template <typename Resource>
	concept resource_concept = std::is_base_of_v<libgraphics::IShader, Resource>;

	class ResourceManager
	{
	public:
		template <resource_concept Resource>
		static auto RegisterResource(const ResourceType type, const std::string& resource_name, const std::shared_ptr<Resource>& resource) -> void
		{
			auto& resource_map = GetResourceMap<Resource>();

			if (!find<Resource>(type, resource_name))
			{
				GetResourceMap<Resource>()[type][resource_name] = resource;
			}
			else
			{
				CX_CORE_ERROR("This resource is already registered, please use Get() to retrieve it.");
			}
		}

		template <resource_concept Resource>
		static auto GetFromCache(const ResourceType type, const std::string& resource_name) -> std::optional<std::shared_ptr<Resource>>
		{
			if (const auto& resource = find<Resource>(type, resource_name))
			{
				return resource;
			}

			CX_CORE_ERROR("This resource doesn't exist, please use Register() to add it first");
			return std::nullopt;
		}

	private:
		template <resource_concept Resource>
		static auto GetResourceMap() -> std::map<ResourceType, std::map<std::string, std::shared_ptr<Resource>>>&
		{
			static auto resource_map = std::map<ResourceType, std::map<std::string, std::shared_ptr<Resource>>>{};
			return resource_map;
		}

		template <resource_concept Resource>
		static auto find(const ResourceType type, const std::string& resource_name) -> std::optional<std::shared_ptr<Resource>>
		{
			auto& resource_map = GetResourceMap<Resource>();

			if (const auto it = std::ranges::find_if(resource_map, [&](const auto& p) {
				return p.first == type && p.second.find(resource_name) != p.second.end();
				}); it != resource_map.end())
			{
				const auto& second_elem = it->second[resource_name];
				return { second_elem };
			}

			return {};
		}
	};
}
