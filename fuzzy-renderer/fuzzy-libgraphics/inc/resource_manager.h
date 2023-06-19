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
			if (!Find<Resource>(type, resource_name))
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
			if (const auto& resource = Find<Resource>(type, resource_name))
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
			static std::map<ResourceType, std::map<std::string, std::shared_ptr<Resource>>> resource_map;
			return resource_map;
		}

		template <resource_concept Resource>
		static auto Find(const ResourceType type, const std::string& resource_name) -> std::optional<std::shared_ptr<Resource>>
		{
			const auto& resource_map = GetResourceMap<Resource>();
			if (const auto type_iter = resource_map.find(type); type_iter != resource_map.end()) 
			{
				if (const auto resource_iter = type_iter->second.find(resource_name); resource_iter != type_iter->second.end()) 
				{
					return { resource_iter->second };
				}
			}
			return {};
		}
	};
}
