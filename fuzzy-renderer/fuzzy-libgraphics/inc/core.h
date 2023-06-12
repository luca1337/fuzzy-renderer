#pragma once

#include <interfaces/igraphics_window.h>
#include <opengl/camera.h>

namespace libgraphics
{
	class IGraphicsWindow;
	class IShader;
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

		LIBGRAPHICS_API auto Init(const GraphicsAPI api_type, const int context_width, const int context_height, const std::string_view context_title) -> void;
		LIBGRAPHICS_API auto Update(const RenderFunction&) const -> void;
		LIBGRAPHICS_API static auto GetInstance() -> Core&;

		LIBGRAPHICS_API [[nodiscard]] auto GetMainCamera() const -> Camera& { return m_p_impl->m_main_camera; }
		LIBGRAPHICS_API [[nodiscard]] auto GetGraphicsWindow() const -> std::shared_ptr<IGraphicsWindow>& { return m_p_impl->m_graphics_window; }

	private:
		Core() = default;

		std::shared_ptr<class Model> m_test_cube = {};
		std::shared_ptr<IShader> default_shader = {};

		CoreImpl* m_p_impl = nullptr;
	};
}