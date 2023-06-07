#pragma once

#include <string_view>
#include <functional>
#include <memory>

#ifdef FUZZYLIBGRAPHICS_EXPORTS
#define LIBGRAPHICS_API __declspec(dllexport)
#else
#define LIBGRAPHICS_API __declspec(dllimport)
#endif
