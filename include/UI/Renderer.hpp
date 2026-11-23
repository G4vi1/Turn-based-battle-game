#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

/*
    Renderer draws the game's visual elements on the screen. It encapsulates
    the SDL operations used to create a window, draw graphics and text, and
    manage graphical resources.
*/
class Renderer
{
public:

    // Initializes SDL, the window and the renderer.

    bool initialize();

    // Shuts down SDL and releases resources.

    void shutdown();

    // Clears the current frame.

    void clear();

    // Displays the current frame.

    void present();

    // Sets the drawing color.

    void setDrawColor(
        Uint8 r,
        Uint8 g,
        Uint8 b,
        Uint8 a
    );

    // Draws a filled rectangle.

    void drawFilledRect(
        float x,
        float y,
        float width,
        float height
    );

    // Draws the border of a rectangle.

    void drawRect(
        float x,
        float y,
        float width,
        float height
    );

    // Draws text on the screen.

    void drawText(
        const char* text, // Null-terminated text string.
        float x,
        float y,
        float size
    );

private:

    // These pointers are private so only Renderer can manage these resources.

    // The SDL window pointer used for rendering.

    SDL_Window* window = nullptr;

    // The SDL renderer used for drawing.

    SDL_Renderer* renderer = nullptr;

    TTF_Font* font = nullptr;
};

#endif
