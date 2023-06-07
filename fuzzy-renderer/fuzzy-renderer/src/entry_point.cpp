#include <core.h>
#include <enums.h>
#include <iostream>

int main(int argc, char** argv)
{
	auto& core = libgraphics::Core::GetInstance();
	core.Init(libgraphics::GraphicsAPI::OpenGL, 800, 600, "GLContext");

	float ctx = { };

	core.Update([&ctx](float deltaTime) {
		ctx += deltaTime;
		std::cout << ctx << std::endl;
	});

	return 0;
}