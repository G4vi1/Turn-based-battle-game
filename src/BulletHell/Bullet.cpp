#include "BulletHell/Bullet.hpp"

Bullet::Bullet(float startX,
               float startY,
               float speedX,
               float speedY,
               float size,
               int damage)
    : x(startX),
      y(startY),
      speedX(speedX),
      speedY(speedY),
      size(size),
      damage(damage),
      active(true)
{
}

//Moves the bullet according to its velocity and elapsed time.

void Bullet::update(float deltaTime)

{
    if (!active)
    {
        return;
    }

    x += speedX * deltaTime;
    y += speedY * deltaTime;
}

bool Bullet::isOutsideArena(float arenaWidth, float arenaHeight) const
{
    return x + size < 0.0f ||
           y + size < 0.0f ||
           x > arenaWidth ||
           y > arenaHeight;
}

bool Bullet::collidesWith(const Soul& soul) const
{
    if (!active)
    {
        return false;
    }

    // Both objects use their top-left corner as position.
    return x < soul.getX() + soul.getSize() &&
           x + size > soul.getX() &&
           y < soul.getY() + soul.getSize() &&
           y + size > soul.getY();
}

float Bullet::getX() const
{
    return x;
}

float Bullet::getY() const
{
    return y;
}

float Bullet::getSize() const
{
    return size;
}

int Bullet::getDamage() const
{
    return damage;
}

bool Bullet::isActive() const
{
    return active;
}

void Bullet::deactivate()
{
    active = false;
}
