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
