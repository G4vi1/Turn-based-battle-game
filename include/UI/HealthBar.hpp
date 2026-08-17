#ifndef HEALTH_BAR_HPP
#define HEALTH_BAR_HPP

#include "Renderer.hpp"

class HealthBar
{
public:

    // Constructor.

    HealthBar(float x,
              float y,
              float width,
              float height,
              int maxHealth);

    // Draws the health bar.

    void draw(Renderer& renderer);

    // Changes the current health.

    void setHealth(int health);

    double getHealth() const;
    double getMaxHealth() const;

private:

    // Health bar position.

    float x;
    float y;

    // Health bar size.

    float width;
    float height;

    // Current health.

    int health;

    // Maximum health.

    int maxHealth;
};

#endif