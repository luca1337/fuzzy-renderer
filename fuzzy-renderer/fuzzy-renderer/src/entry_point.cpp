#include <core.h>
#include <enums.h>
#include <iostream>

int main(int argc, char** argv)
{
	auto& core = libgraphics::Core::GetInstance();
	core.Init(libgraphics::GraphicsAPI::OpenGL, 1024, 768, "GLContext");

	core.Update([&](const double delta_time) {
	});

	return 0;
}