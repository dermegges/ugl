/** @file App.cpp



Copyright 2016 Computational Topology Group, University of Kaiserslautern

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

    Author(s): C.Garth, T.Biedert
*/

#include "ugl/sdl/App.hpp"

#include <SDL.h>
#include <SDL_opengl.h>

#include <iostream>
#include <sstream>
#include <iomanip>

namespace ugl
{
namespace sdl
{

/**
 * @brief Creates the SDL2 window and executes the main loop.
 * @param application
 * @param width
 * @param height
 * @param title
 * @param showFPS
 * @param vsync
 * @return
 */
int App::run(ugl::CoreApplication* application, int width, int height, std::string title, bool showFPS, bool vsync, int major, int minor)
{
    /*
     * Initialize SDL and create window.
     */
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window* window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if(window == NULL)
    {
        std::cerr << "Window could not be created: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if(context == NULL)
    {
        std::cerr << "OpenGL context could not be created: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (vsync)
    {
        if(SDL_GL_SetSwapInterval(1) < 0)
            std::cerr << "Warning: Unable to enable VSync: " << SDL_GetError() << std::endl;
    }


    /*
     * Initialize ugl.
     */
    application->uglInit();
    application->uglResize(glm::ivec2(width, height), glm::ivec2(width, height));


    /*
     * Main loop.
     */
    SDL_Event event;
    bool quit = false;

    int frames = 0;
    int lastTicks = SDL_GetTicks();

    while (!quit && !application->isExitRequested())
    {
        // Handle events on queue
        while(SDL_PollEvent(&event) != 0)
        {
            // Quit request
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }

            // Window event
            else if (event.type == SDL_WINDOWEVENT)
            {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                    application->uglResize(glm::ivec2(event.window.data1, event.window.data2), glm::ivec2(event.window.data1, event.window.data2));
            }

            // Mouse move event
            else if (event.type == SDL_MOUSEMOTION)
            {
                application->uglOnMouseMove(event.motion.x, event.motion.y);
            }

            // Mouse down/up event
            else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
            {
                int x = event.button.x;
                int y = event.button.y;

                bool left = event.button.button == SDL_BUTTON_LEFT;
                bool middle = event.button.button == SDL_BUTTON_MIDDLE;
                bool right = event.button.button == SDL_BUTTON_RIGHT;

                SDL_Keymod mod = SDL_GetModState();

                bool shift = (mod & KMOD_SHIFT) > 0;
                bool control = (mod & KMOD_CTRL) > 0;
                bool alt = (mod & KMOD_ALT) > 0;

                if (event.type == SDL_MOUSEBUTTONDOWN)
                    application->uglOnMousePress(x, y, left, middle, right, shift, control, alt);
                else
                    application->uglOnMouseRelease(left, middle, right);
            }

            // Keyboard event
            else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
            {
                int key = event.key.keysym.sym;

                SDL_Keymod mod = SDL_GetModState();

                bool shift = (mod & KMOD_SHIFT) > 0;
                bool control = (mod & KMOD_CTRL) > 0;
                bool alt = (mod & KMOD_ALT) > 0;

                if (event.type == SDL_KEYDOWN)
                    application->uglOnKeyPress(key, shift, control, alt);
                else
                    application->uglOnKeyRelease(key);
            }

        }

        // Render
        application->uglDraw();

        SDL_GL_SwapWindow(window);

        // Update FPS
        if (showFPS)
        {
            ++frames;

            int ticks = SDL_GetTicks();

            // Update title once per second
            float seconds = (ticks - lastTicks) / 1000.0f;
            if (seconds > 1.0f)
            {
                float fps = (float) frames / seconds;

                lastTicks = ticks;
                frames = 0;

                std::ostringstream newTitle;
                newTitle << title << " | " << std::fixed << std::setprecision(2) << fps << " FPS";

                SDL_SetWindowTitle(window, newTitle.str().c_str());

            }
        }
    }


    /*
     * Shutdown.
     */
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

}
}
