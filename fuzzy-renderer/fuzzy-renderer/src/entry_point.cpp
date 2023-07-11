#include <core.h>
#include <enums.h>
#include <iostream>

int main(int argc, char** argv)
{
	auto& core = libgraphics::Core::GetInstance();
	core.Init(libgraphics::GraphicsAPI::opengl, 1920, 1080, "GLContext");

	core.Update([&](const double delta_time) {
	});

	return 0;
}