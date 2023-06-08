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

	class LIBGRAPHICS_API Core final
	{
	public:
		~Core() { delete m_p_impl; }
		Core(const Core&) = delete;
		Core(Core&&) = delete;
		Core& operator=(const Core&) = delete;
		Core& operator=(Core&&) = delete;

		auto Init(const GraphicsAPI api, int contextWidth, int contextHeight, std::string_view contextTitle) -> void;
		auto Update(const RenderFunction&) const -> void;
		static auto GetInstance() -> Core&;

		[[nodiscard]] auto GetMainCamera() const -> Camera& { return m_p_impl->m_main_camera; }
		[[nodiscard]] auto GetGraphicsWindow() const -> std::shared_ptr<IGraphicsWindow>& { return m_p_impl->m_graphics_window; }

	private:
		Core() = default;

		std::shared_ptr<class IMesh> m_test_cube = {};

		CoreImpl* m_p_impl = nullptr;
	};
}