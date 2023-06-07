#include "gl_window.h"
#include "gl_context.h"

namespace libgraphics
{
	auto GLWindow::Create() -> void
	{
		_graphicsContext = std::make_shared<GLContext>();
	}

	auto GLWindow::Destroy() -> void
	{
		_graphicsContext->Shutdown();
	}
}
