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
           float height,
           const char* text);

    // Draws the button.

    void draw(Renderer& renderer);

    bool isClicked(const SDL_Event& event);

    bool isHovered(float mouseX, float mouseY) const;

    void setHovered(bool hovered);

    void setPressed(bool pressed);

private:

    // Button position.

    float x;
    float y;

    // Button size.

    float width;
    float height;

    // Button text.

    const char* text;

    // Checks if the mouse is over the button.

    bool hovered = false;

    // Checks if the button is pressed.

    bool pressed = false;

};

#endif