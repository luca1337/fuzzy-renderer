#include <render_profiler.h>

namespace libgraphics::utils::profiling
{
	auto RenderProfiler::GetInstance() -> RenderProfiler&
	{
		static auto instance = RenderProfiler{};
		return instance;
	}

	auto RenderProfiler::Update() -> void
	{
		
	}

}
