#pragma once

// il render profile molto genericamente deve tenere traccia di alcuni dati come:
// le mesh in scena, quindi che comprendono il numero di triangoli, facce, vertici, etc..
// il frame time
// i frames al secondo
// i dati di input/output (tastiera e mouse) sarebbe carino profilarli in modo tale da capire la coda di messaggi quanti ne contiene..

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