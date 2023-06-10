#pragma once

#include <interfaces/igraphics_window.h>
#include <color.h>

namespace libgraphics
{
	class IGraphicsContext;

	class GLWindow final : public IGraphicsWindow
	{
	public:
		auto Create(const int width, const int height, const std::string_view title) -> void override;
		auto Destroy() -> void override;
		auto Clear() -> void override;
		auto SwapBuffers() -> void override;
		[[nodiscard]] auto GetNativeHandle() const -> const std::shared_ptr<IGraphicsContext> & override { return m_graphics_context; }
		auto SetClearColor(const Color&) -> void override;

	private:

		Color m_clear_color = {};
		std::shared_ptr<IGraphicsContext> m_graphics_context;
	};
}
