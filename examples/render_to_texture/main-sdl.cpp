#include "RenderToTexture.hpp"

#include <ugl/sdl/App.hpp>


int main(int /*argc*/, char* /*argv*/[])
{
	ugl::sdl::App app;
    return app.run(new RenderToTexture(), 1024, 768, "Render To Texture (SDL)", true, false);
}
