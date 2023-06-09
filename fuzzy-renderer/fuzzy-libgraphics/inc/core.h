#pragma once

#include <interfaces/igraphics_window.h>
#include <opengl/camera.h>

namespace libgraphics
{
	class IGraphicsWindow;
	enum class GraphicsAPI;

	using RenderFunction = std::function<void(double)>;

	class CoreImpl
	{
	public:
		GraphicsAPI m_graphics_api = {};
		Camera m_main_camera = {};
		std::shared_ptr<IGraphicsWindow> m_graphics_window = {};
	};

	class Core final
	{
	public:
		~Core() { delete m_p_impl; }
		Core(const Core&) = delete;
		Core(Core&&) = delete;
		Core& operator=(const Core&) = delete;
		Core& operator=(Core&&) = delete;

		LIBGRAPHICS_API auto Init(const GraphicsAPI api_type, int context_width, int context_height, const std::string_view context_title) -> void;
		LIBGRAPHICS_API auto Update(const RenderFunction&) const -> void;
		LIBGRAPHICS_API static auto GetInstance() -> Core&;

		LIBGRAPHICS_API [[nodiscard]] auto GetMainCamera() const -> Camera& { return m_p_impl->m_main_camera; }
		LIBGRAPHICS_API [[nodiscard]] auto GetGraphicsWindow() const -> std::shared_ptr<IGraphicsWindow>& { return m_p_impl->m_graphics_window; }

	private:
		Core() = default;

		std::shared_ptr<class IMesh> m_test_cube = {};

		CoreImpl* m_p_impl = nullptr;
	};
}