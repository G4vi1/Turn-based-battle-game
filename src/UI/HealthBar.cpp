#include "UI/HealthBar.hpp"

// Constructor.
//
// Stores the position, dimensions,
// and maximum health of the bar.

HealthBar::HealthBar(float x,
                     float y,
                     float width,
                     float height,
                     int maxHealth)
    : x(x),
      y(y),
      width(width),
      height(height),
      health(maxHealth),
      maxHealth(maxHealth)
{
}

// Draws the health bar.
//
// The background represents missing health.
// The filled portion represents current health.

void HealthBar::draw(Renderer& renderer)
{
    // Draw the health bar background.

    renderer.setDrawColor(
        60,
        60,
        60,
        255
    );

    renderer.drawFilledRect(
        x,
        y,
        width,
        height
    );

    // Calculate the percentage of remaining health.

    float healthPercentage =
        static_cast<float>(health) /
        static_cast<float>(maxHealth);

    // Calculate the width of the health portion.

    float healthWidth =
        width * healthPercentage;

    // Draw the current health.

    renderer.setDrawColor(
        40,
        180,
        70,
        255
    );

    renderer.drawFilledRect(
        x,
        y,
        healthWidth,
        height
    );

    // Draw the border.

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

// Changes the current health.

void HealthBar::setHealth(int health)
{
    // Prevent health from going below zero.

    if (health < 0)
    {
        health = 0;
    }

    // Prevent health from exceeding maximum health.

    if (health > maxHealth)
    {
        health = maxHealth;
    }

    this->health = health;
}

// Returns the current health.

double HealthBar::getHealth() const
{
    return health;
}

double HealthBar::getMaxHealth() const
{
    return maxHealth;
}