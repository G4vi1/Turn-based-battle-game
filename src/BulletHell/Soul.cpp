#include "BulletHell/Soul.hpp"

Soul::Soul(float startX, float startY, float speed)
    : x(startX), y(startY), speed(speed), size(20.0f) // Initialize size to a default value
{
}

void Soul::moveUp(float deltaTime)
{
    y -= speed * deltaTime;
}

void Soul::moveDown(float deltaTime)
{
    y += speed * deltaTime;
}

void Soul::moveLeft(float deltaTime)
{
    x -= speed * deltaTime;
}

void Soul::moveRight(float deltaTime)
{
    x += speed * deltaTime;
}

// Prevents the Soul from moving beyond the arena boundaries.
// Impede o robô de atravessar os limites da sala.
void Soul::keepInsideRoom(float roomWidth, float roomHeight)
{
    /*If the Soul goes past the left boundary,
    place it exactly at the beginning of the arena.*/ 
    if (x < 0)
    {
        x = 0;
    }

    /* If it goes past the upper boundary,
    place it at the top of the arena.*/
    if (y < 0)
    {
        y = 0;
    }

    /*x represents the Soul's left edge.
    Therefore, its size must be added before comparing
    it with the arena width.*/
    if (x + size > roomWidth)
    {
        x = roomWidth - size;
    }

    /*The same reasoning is applied to the lower boundary.*/
    if (y + size > roomHeight)
    {
        y = roomHeight - size;
    }
}

float Soul::getX() const
{
    return x;
}

float Soul::getY() const
{
    return y;
}
