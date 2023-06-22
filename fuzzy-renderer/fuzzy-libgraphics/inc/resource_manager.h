#pragma once

#include <any>
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
		std::shared_ptr<Resource> m_resource = {}; // this can even be removed and i can use an Object_Ref -> Resource& m_resource = {};
	};

	template <std::derived_from<libgraphics::IShader> Resource>
	using Resources = std::vector<ResourceParams<Resource>>;

	class ResourceManager
	{
	public:
		template <std::derived_from<libgraphics::IShader> Resource>
		static void RegisterResource(const ResourceParams<Resource>& params)
		{
			if (!find_private(params)) m_resources<Resource>.push_back(params);
			else CX_CORE_ERROR("This resource will not be loaded as it's already registered, please use GetFromCache() to retrieve it.");
		}

		template <std::derived_from<libgraphics::IShader> Resource>
		[[nodiscard]] static auto GetFromCache(const ResourceParams<Resource>& params) -> std::optional<decltype(std::declval<ResourceParams<Resource>>().m_resource)> { return find_private(params).value().m_resource; }

		template <typename Resource>
		std::vector<ResourceParams<Resource>> GetAllResources()
		{
			return ResourceManager::template m_resources<Resource>;
		}


	private:
		template <std::derived_from<libgraphics::IShader> Resource>
		inline static Resources<Resource> m_resources = {};

		template <std::derived_from<libgraphics::IShader> Resource, typename ReturnType = typename std::optional<ResourceParams<Resource>>::value_type>
		static auto find_private(const ResourceParams<Resource>& params) -> std::optional<ReturnType>
		{
			auto it = std::ranges::find(m_resources<Resource>, params.m_name, &ResourceParams<Resource>::m_name);
			return it != m_resources<Resource>.end() ? *it : std::optional<ReturnType>{};
		}
	};
}
