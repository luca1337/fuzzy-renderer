#pragma once

#include <logger.h>

#include <map>
#include <memory>
#include <optional>

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
			auto& resource_vec = GetResourceVec<Resource>();

			if (!find<Resource>(params))
			{
				resource_vec.push_back(params);
			}
			else
			{
				CX_CORE_ERROR("This resource is already registered, please use Get() to retrieve it.");
			}
		}

		template <std::derived_from<libgraphics::IShader> Resource>
		static std::optional<ResourceParams<Resource>> GetFromCache(const ResourceParams<Resource>& params)
		{
			auto& resource_vec = GetResourceVec<Resource>();

			if (const auto& param = find<Resource>(params))
			{
				return param;
			}

			CX_CORE_ERROR("This resource doesn't exist, please use Register() to add it first");
			return std::nullopt;
		}

	private:
		template <std::derived_from<libgraphics::IShader> Resource>
		static auto& GetResourceVec()
		{
			static auto resource_vec = Resources<Resource>{};
			return resource_vec;
		}

		template <std::derived_from<libgraphics::IShader> Resource>
		static auto find(const ResourceParams<Resource>& params) -> std::optional<ResourceParams<Resource>>
		{
			auto& resource_vec = GetResourceVec<Resource>();

			const auto& find_element = [&](const auto& rp) { return rp.m_resource_type == params.m_resource_type && rp.m_name == params.m_name; };

			auto it = std::find_if(resource_vec.begin(), resource_vec.end(), find_element);

			if (it != resource_vec.end()) {
				return *it;
			}

			return std::nullopt;
		}
	};

}
