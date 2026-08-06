#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "Renderer.hpp"

class Button
{
public:

    // Constructor

    Button(float x,
           float y,
           float width,
           float height);

    // Draws the button.

    void draw(Renderer& renderer);

    bool isClicked(const SDL_Event& event);

private:

    // Button position.

    float x;
    float y;

    // Button size.

    float width;
    float height;
};

#endif