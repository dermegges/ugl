#include "DFBTest.hpp"

#include <ugl/sdl/App.hpp>


int main(int /*argc*/, char* /*argv*/[])
{
	ugl::sdl::App app;
    return app.run(new DFBTest(), 1024, 768, "Dynamic Fragment Buffer Test (SDL)", true, false, 4, 3);
}
