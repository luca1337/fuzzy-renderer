#pragma once

#include <interfaces/igraphics_window.h>
#include <opengl/camera.h>

namespace libgraphics
{
	class EntityManager;
	struct Light;
	class GLSkybox;
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
		LIBGRAPHICS_API auto Update(const RenderFunction&) -> void;
		LIBGRAPHICS_API static auto GetInstance() -> Core&;

		LIBGRAPHICS_API auto AddLight(const std::shared_ptr<Light>&) -> void;
		LIBGRAPHICS_API [[nodiscard]] auto GetLights() const -> std::vector<std::shared_ptr<Light>>;

		LIBGRAPHICS_API auto GetDeltaTime() const -> float { return m_delta_time; }

		LIBGRAPHICS_API [[nodiscard]] auto GetMainCamera() const -> Camera& { return m_p_impl->m_main_camera; }
		LIBGRAPHICS_API [[nodiscard]] auto GetGraphicsWindow() const -> std::shared_ptr<IGraphicsWindow>& { return m_p_impl->m_graphics_window; }

		LIBGRAPHICS_API [[nodiscard]] auto GetEntityManager () const -> std::shared_ptr<EntityManager> { return m_entity_manager; }

	private:
		Core() = default;

		std::shared_ptr<EntityManager> m_entity_manager = {};

		float m_delta_time = {};

		std::shared_ptr<class Model> m_entity_model = {};
		std::shared_ptr<class Model> m_entity_model2 = {};
		std::shared_ptr<GLSkybox> m_sky_box = {};

		std::vector<std::shared_ptr<Light>> m_lights = {};

		CoreImpl* m_p_impl = nullptr;
	};
}