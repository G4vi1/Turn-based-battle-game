#include "UI/Renderer.hpp"

/*
    Renderer lifecycle:

    1. SDL_Init starts SDL's video subsystem.
    2. TTF_Init starts SDL_ttf so the game can load fonts and render text.
    3. SDL_CreateWindow creates the game window.
    4. SDL_CreateRenderer creates the renderer associated with that window.
    5. TTF_OpenFont loads the font used by drawText.
    6. setDrawColor selects the color used by the drawing operations.
    7. drawFilledRect and drawRect use SDL_FRect values to draw rectangles.
    8. drawText renders text to a CPU-side surface, converts it to a texture,
       and copies that texture to the renderer.
    9. present displays the completed frame.
    10. shutdown releases the font, renderer, window, SDL_ttf, and SDL in the
        reverse order from initialization.

    If initialization fails, resources already created are released before
    the function returns false.
*/

bool Renderer::initialize()
// Initializes SDL, the window and the renderer.
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        // SDL initialization failed.
        return false;
    }

    // Initializes SDL_ttf for loading fonts and rendering text.
    if (!TTF_Init())
    {
        SDL_Quit();
        return false;
    }

    // Creates the game window.
    window = SDL_CreateWindow(
        "Turn Based Battle Game", // Window title
        1280, // Window width
        720, // Window height
        0
    );

    // Check if the window was created successfully.

    if (window == nullptr)
    {
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    // Creates a renderer associated with the window.
    renderer = SDL_CreateRenderer(
        window, // The window to render to
        nullptr // Use the first available rendering driver
    );

    // Check if the renderer was created successfully.

    if (renderer == nullptr){   
        SDL_DestroyWindow(window);
        window = nullptr;

        TTF_Quit();
        SDL_Quit();
        return false;
    }

// Load the font.

    font = TTF_OpenFont(
    "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
    32
    );

    // Check if the font was loaded successfully.

    if (font == nullptr) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;

        SDL_DestroyWindow(window);
        window = nullptr;

        TTF_Quit();
        SDL_Quit();
        return false;
    }

    return true;
}

// Shuts down SDL and releases resources.
void Renderer::shutdown()
{
    if (font != nullptr)
    {
        TTF_CloseFont(font);
        font = nullptr;
    }

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

    TTF_Quit();
    SDL_Quit();
}


// Clears the current frame.

void Renderer::clear()
{
    SDL_RenderClear(renderer);
}

// Displays the current frame.

void Renderer::present()
{
    SDL_RenderPresent(renderer);
}

// Sets the drawing color.

void Renderer::setDrawColor(
    Uint8 r,
    Uint8 g,
    Uint8 b,
    Uint8 a
)
// Sets the drawing color for the renderer.
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

void Renderer::drawFilledRect(
    float x,
    float y,
    float width,
    float height
)
{
    /*
        Create an SDL_FRect structure to represent the rectangle.

        Obs: SDL_FRect is a structure that represents a
        rectangle with floating-point coordinates.
    */
    SDL_FRect rect{
        x,
        y,
        width,
        height
    };

    /*
        Use SDL_RenderFillRect to draw the filled rectangle on the renderer.
        Observation: note that we call the renderer here + the already made
        SDL_FRect structure with the "rect" alias.
    */

    SDL_RenderFillRect(
        renderer,
        &rect
    );
}


// Draws the outline of a rectangle.

void Renderer::drawRect(
    float x,
    float y,
    float width,
    float height
)

// Creates a rectangle using its position and dimensions.
{
    SDL_FRect rect{
        x,
        y,
        width,
        height
    };

     // Draws only the rectangle's border using the current draw color.
    SDL_RenderRect(
        renderer,
        &rect
    );
}


// Draws text on the screen.
void Renderer::drawText(
    const char* text, // Null-terminated text string.
    float x,
    float y,
    float size
)

{
    // Stops if the font was not loaded.

    if (font == nullptr)
    {
        return;
    }
    // Changes the current font size.
    if (!TTF_SetFontSize(font, size))
    {
        return;
    }
    // Defines the text color as white.
    SDL_Color color{
        255,
        255,
        255,
        255
    };

    // Stores the rendered text pixels in CPU memory.

    SDL_Surface* surface =
        TTF_RenderText_Blended(
            font,
            text,
            0,
            color
        );

    // Stops if the text surface could not be created.

    if (surface == nullptr)
    {
        return;
    }

     // Converts the surface into a GPU texture.

    SDL_Texture* texture =

    /*
        Converts the text pixels stored in the CPU-side surface
        into a texture that the renderer can draw, usually using the GPU.
    */

        SDL_CreateTextureFromSurface(
            renderer,
            surface
        );


    // Releases the surface if texture creation fails.
    if (texture == nullptr)
    {
        SDL_DestroySurface(surface);
        return;
    }

    // Defines the text position and size on the screen.

    SDL_FRect destination{
        x,
        y,
        static_cast<float>(surface->w), // Text width.
        static_cast<float>(surface->h)  // Text height.
    };

    /*
        Segue um exemplo...

        (x, y)
  ┌─────────────────────┐
  │        text         │ height: surface->h
  └─────────────────────┘
       width: surface->w
    */

    /*
         // Copies the text texture to the renderer.
    */
    SDL_RenderTexture(
        renderer, // Renderer that will draw the texture.
        texture,  // Texture containing the rendered text.
        nullptr,  // Uses the entire source texture.
        &destination // Defines the texture's position and size on the screen.
    );

    // Releases the texture from the renderer/GPU after drawing it.
    SDL_DestroyTexture(texture);

    // Releases the temporary pixel data stored in RAM.
    SDL_DestroySurface(surface);
}
