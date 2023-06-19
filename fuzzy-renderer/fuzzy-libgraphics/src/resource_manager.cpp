#include <resource_manager.h>

#include <filesystem>

namespace libgraphics::resources
{
	auto ResourceManager::GetFullPath(const ResourceType type) const -> std::string_view
	{
		const auto current_path = std::filesystem::current_path().parent_path() / "fuzzy-libgraphics" / "shaders";

		return std::string_view{current_path.string()};
	}

}