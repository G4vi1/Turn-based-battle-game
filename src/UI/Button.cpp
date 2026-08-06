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

// Draws only the border of a rectangle.

void Button::draw(Renderer& renderer)
{
    // Draw the button background.

    renderer.setDrawColor(
        140,
        140,
        140,
        255
    );

    renderer.drawFilledRect(
        x,
        y,
        width,
        height
    );

    // Draw the button border.

    renderer.setDrawColor(
        230,
        230,
        230,
        255
    );

    renderer.drawRect(
        x,
        y,
        width,
        height
    );
}

bool Button::isClicked(const SDL_Event& event)
{
    // Only react to mouse button presses.

    if (event.type != SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        return false;
    }

    // Mouse position.

    float mouseX = event.button.x;
    float mouseY = event.button.y;

    // Check if the click happened inside
    // the button rectangle.

    return mouseX >= x &&
           mouseX <= x + width &&
           mouseY >= y &&
           mouseY <= y + height;
}