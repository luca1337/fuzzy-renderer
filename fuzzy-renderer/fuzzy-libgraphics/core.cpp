#include "core.h"
#include <enums.h>
#include <gl_window.h>

namespace libgraphics
{
	void Core::Init(const GraphicsAPI api, int contextWidth, int contextHeight, std::string_view contextTitle)
	{
		switch (api)
		{
		case GraphicsAPI::OpenGL:
		{
			_graphicsWindow = std::make_shared<GLWindow>();
			_graphicsWindow->Create(contextWidth, contextHeight, contextTitle);
		}
		break;
		case GraphicsAPI::DirectX: /* ... */ break;
		default: break;
		}
	}

	auto Core::GetInstance() -> Core&
	{
		static auto core = Core();
		return core;
	}
}