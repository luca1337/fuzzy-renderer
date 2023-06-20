#pragma once

#include <logger.h>

#include <memory>
#include <optional>
#include <ranges>

namespace libgraphics::resources
{
	class IShader;

	enum class ResourceType
	{
		shader,
		texture
	};

	template <std::derived_from<libgraphics::IShader> Resource>
	struct ResourceParams
	{
		ResourceType m_resource_type = {};
		std::string m_name = {};
		std::shared_ptr<Resource> m_resource = {};
	};

	template <std::derived_from<libgraphics::IShader> Resource>
	using Resources = std::vector<ResourceParams<Resource>>;

	class ResourceManager
	{
	public:
		template <std::derived_from<libgraphics::IShader> Resource>
		static void RegisterResource(const ResourceParams<Resource>& params)
		{
			if (!find(params))
			{
				ResourceVec<Resource>().push_back(params);
			}
			else
			{
				CX_CORE_ERROR("This resource will not be loaded as it's already registered, please use GetFromCache() to retrieve it.");
			}
		}

		template <std::derived_from<libgraphics::IShader> Resource>
		static auto GetFromCache(const ResourceParams<Resource>& params) -> std::optional<decltype(std::declval<ResourceParams<Resource>>().m_resource)> { return find(params).value().m_resource; }

	private:
		template <std::derived_from<libgraphics::IShader> Resource>
		static auto& ResourceVec()
		{
			static auto resource_vec = Resources<Resource>{};
			return resource_vec;
		}

		template <std::derived_from<libgraphics::IShader> Resource, typename ReturnType = typename std::optional<ResourceParams<Resource>>::value_type>
		static auto find(const ResourceParams<Resource>& params) -> std::optional<ReturnType>
		{
			auto it = std::ranges::find(ResourceVec<Resource>(), params.m_name, &ResourceParams<Resource>::m_name);
			return it != ResourceVec<Resource>().end() ? *it : std::optional<ReturnType>{};
		}
	};
}
