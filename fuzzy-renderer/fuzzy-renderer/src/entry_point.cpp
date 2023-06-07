#include <core.h>
#include <enums.h>

int main(int argc, char** argv)
{
	auto& core = libgraphics::Core::GetInstance();
	core.Init(libgraphics::GraphicsAPI::OpenGL, 800, 600, "GLContext");
	core.Update();

	return 0;
}