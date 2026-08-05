#include "UI/Button.hpp"

// Constructor.
//
// Stores the position and dimensions
// of the button.

Button::Button(float x,
               float y,
               float width,
               float height)
    : x(x),
      y(y),
      width(width),
      height(height)
{
}

// Draws the button.
//
// For now, a button is simply a
// light gray rectangle.

void Button::draw(Renderer& renderer)
{
    // Selects the button color.

    renderer.setDrawColor(
        140,
        140,
        140,
        255
    );

    // Draws the button body.

    renderer.drawFilledRect(
        x,
        y,
        width,
        height
    );
}