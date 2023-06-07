#pragma once

#include "igraphics_window.h"

namespace libgraphics
{
	class IGraphicsWindow;
	enum class GraphicsAPI;

	using RenderFunction = std::function<void(float)>;

	class LIBGRAPHICS_API Core final
	{
	public:
		Core(const Core&) = delete;
		Core(Core&&) = delete;
		Core& operator=(const Core&) = delete;
		Core& operator=(Core&&) = delete;

		auto Init(const GraphicsAPI api, int contextWidth, int contextHeight, std::string_view contextTitle) -> void;
		auto Update(const RenderFunction&) -> void;
		static auto GetInstance() -> Core&;

	private:
		Core() = default;

		GraphicsAPI _graphicsApi = {};
		std::shared_ptr<IGraphicsWindow> _graphicsWindow = {};
	};
}