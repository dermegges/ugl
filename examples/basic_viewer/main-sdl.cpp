#include "BasicViewer.hpp"

#include <ugl/sdl/App.hpp>


int main(int /*argc*/, char* /*argv*/[])
{
	ugl::sdl::App app;
    return app.run(new BasicViewer(), 1024, 768, "Basic Viewer (SDL)", true, false);
}
