#include "UI/Button.hpp"

// Constructor.
//
// Stores the position, dimensions,
// and text of the button.

Button::Button(float x,
               float y,
               float width,
               float height,
               const std::string& text)
    : x(x),
      y(y),
      width(width),
      height(height),
      text(text)
{
}

// Draws the button.
//
// The button changes its appearance
// depending on its current state.

void Button::draw(Renderer& renderer)
{
    // Draw the button background.

    if (pressed)
    {
        // Button is being pressed.

        renderer.setDrawColor(
            100,
            100,
            100,
            255
        );
    }
    else if (hovered)
    {
        // Mouse is over the button.

        renderer.setDrawColor(
            180,
            180,
            180,
            255
        );
    }
    else
    {
        // Normal button state.

        renderer.setDrawColor(
            140,
            140,
            140,
            255
        );
    }

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

    // Draw the button text.

    renderer.drawText(
        text.c_str(),
        x + 20,
        y + 20,
        32
    );
}

// Checks whether the button was clicked.

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

// Checks whether the mouse is over the button.

bool Button::isHovered(float mouseX, float mouseY) const
{
    return mouseX >= x &&
           mouseX <= x + width &&
           mouseY >= y &&
           mouseY <= y + height;
}

// Changes the hover state.

void Button::setHovered(bool hovered)
{
    this->hovered = hovered;
}

// Changes the pressed state.

void Button::setPressed(bool pressed)
{
    this->pressed = pressed;
}

void Button::setText(const std::string& text)
{
    this->text = text;
}