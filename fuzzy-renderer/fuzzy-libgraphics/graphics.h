#pragma once

#ifdef FUZZYLIBGRAPHICS_EXPORTS
#define LIBGRAPHICS_API __declspec(dllexport)
#else
#define LIBGRAPHICS_API __declspec(dllimport)
#endif

namespace libgraphics
{
	class LIBGRAPHICS_API graphics
	{
	public:
		void DoStuff();
	};
}
