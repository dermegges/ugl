#include "AfrontViewer.hpp"

#include <ugl/sdl/App.hpp>


int main(int argc, char* argv[])
{
    if (argc <= 1)
    {
        std::cerr << "Error: No input file" << std::endl;
        return 1;
    }

	ugl::sdl::App app;
    return app.run(new AfrontViewer(std::string(argv[1])), 1024, 768, "Afront Viewer (SDL)", true, false);
}
