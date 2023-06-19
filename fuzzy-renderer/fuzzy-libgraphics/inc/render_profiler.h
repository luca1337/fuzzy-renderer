#pragma once

class IMesh;

namespace libgraphics::utils::profiling
{
	class RenderProfiler
	{
	public:
        RenderProfiler(const RenderProfiler&) = delete;
        RenderProfiler& operator=(const RenderProfiler&) = delete;

        static auto GetInstance() -> RenderProfiler&;
        static auto Update() -> void;

    private:
        RenderProfiler() = default;
	};
}