#include "Viewer.hpp"

#include <ugl/sdl/App.hpp>


int main(int /*argc*/, char* /*argv*/[])
{
	ugl::sdl::App app;
    return app.run(new Viewer(), 1024, 768, "Viewer (SDL)", true, false);
}
