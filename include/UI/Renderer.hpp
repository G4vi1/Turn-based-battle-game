#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

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
        const char* text,
        float x,
        float y,
        float size
    );

private:

    SDL_Window* window = nullptr;

    SDL_Renderer* renderer = nullptr;

    TTF_Font* font = nullptr;
};

#endif