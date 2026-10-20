#include "BulletHell/Bullet.hpp"
#include <cmath>

Bullet::Bullet(float startX,
               float startY,
               float speedX,
               float speedY,
               float size,
               int damage,
               float height,
               bool homing)
    : x(startX),
      y(startY),
      speedX(speedX),
      speedY(speedY),
      size(size),
      height(height > 0.0f ? height : size),
      damage(damage),
      active(true),
      homing(homing),
      homingSpeed(std::sqrt(speedX * speedX + speedY * speedY))
{
}

//Moves the bullet according to its velocity and elapsed time.

void Bullet::update(float deltaTime, const Soul& soul)

{
    if (!active)
    {
        return;
    }

    if (homing)
    {
        const float targetX = soul.getX() + soul.getSize() / 2.0f;
        const float targetY = soul.getY() + soul.getSize() / 2.0f;
        const float bulletCenterX = x + size / 2.0f;
        const float bulletCenterY = y + height / 2.0f;
        const float distanceX = targetX - bulletCenterX;
        const float distanceY = targetY - bulletCenterY;
        const float distance = std::sqrt(distanceX * distanceX +
                                         distanceY * distanceY);

        if (distance > 0.0f)
        {
            speedX = distanceX / distance * homingSpeed;
            speedY = distanceY / distance * homingSpeed;
        }
    }

    x += speedX * deltaTime;
    y += speedY * deltaTime;
}

bool Bullet::isOutsideArena(float arenaWidth, float arenaHeight) const
{
    return x + size < 0.0f ||
           y + height < 0.0f ||
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
           y + height > soul.getY();
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

float Bullet::getHeight() const
{
    return height;
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
