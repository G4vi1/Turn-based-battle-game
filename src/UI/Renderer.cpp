#include "UI/Renderer.hpp"
#include <iostream>

// Constructor

Renderer::Renderer()
    : window(nullptr), renderer(nullptr) {
}

//Destructor

Renderer::~Renderer() {

    // Cleans up the SDL resources.

    if(renderer != nullptr)
    {
        SDL_DestroyRenderer(renderer);
    }

    if(window != nullptr)
    {
        SDL_DestroyWindow(window);
    }

    SDL_Quit();

}

// Initializes the SDL renderer.

bool Renderer::initialize() {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
    }


    //Creates the SDL window and renderer.

    window = SDL_CreateWindow(
        "Turn-Based Battle Game",
        1280, // Width
        720,  // Height
        0     // Flags
    );

    //Verifies if the window was created successfully.

    if(window == nullptr)
    {
        return false;
    }

    //Creates the SDL renderer.

    renderer = SDL_CreateRenderer(
        window, //This arguments appoints that this renderer in specific will be used to render the window created above.
        nullptr //Somehow this argument is used to choose the best rendering driver available.
    );

//Verifies if the renderer was created successfully.

    if(renderer == nullptr)
    {
        return false;
    }

    return true;
}

// Responsible for receiving and handling SDL events, such as user input or window events.

void Renderer::clear()
{
    SDL_SetRenderDrawColor(
        renderer,
        0,
        0,
        0,
        255
    );

    SDL_RenderClear(renderer);
}

// Presents the completed frame.
//
// Everything drawn since clear() becomes visible.

void Renderer::present()
{
    SDL_RenderPresent(renderer);
}

// Changes the current drawing color.

void Renderer::setDrawColor(Uint8 r,
                            Uint8 g,
                            Uint8 b,
                            Uint8 a)
{
    SDL_SetRenderDrawColor(
        renderer,
        r,
        g,
        b,
        a
    );
}

// Draws a filled rectangle.

void Renderer::drawFilledRect(float x,
                              float y,
                              float width,
                              float height)
{
    // SDL_FRect is SDL's rectangle structure
    // that stores floating-point coordinates.

    SDL_FRect rect;

    // Defines the rectangle position.

    rect.x = x;
    rect.y = y;

    // Defines the rectangle dimensions.

    rect.w = width;
    rect.h = height;

    // Sends the rectangle to SDL so it can be
    // rendered on the current frame.

    SDL_RenderFillRect(
        renderer,
        &rect
    );
}

// Responsible for receiving and handling SDL events, such as user input or window events.

void Renderer::shutdown()
{
    if (renderer != nullptr)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window != nullptr)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    SDL_Quit();
}
