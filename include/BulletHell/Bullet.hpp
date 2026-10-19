#ifndef BULLET_HELL_BULLET_HPP
#define BULLET_HELL_BULLET_HPP

#include "BulletHell/Soul.hpp"

class Bullet
{
public:
    // Creates a bullet with a starting position, velocity, size, and damage.
    Bullet(float startX,
           float startY,
           float speedX,
           float speedY,
           float size,
           int damage);

    // Moves the bullet according to its velocity and elapsed time.
    void update(float deltaTime);

    // Reports whether the bullet has completely left the arena.
    bool isOutsideArena(float arenaWidth, float arenaHeight) const;

    // Checks whether the bullet overlaps the Soul.
    bool collidesWith(const Soul& soul) const;

    float getX() const;
    float getY() const;
    float getSize() const;
    int getDamage() const;
    bool isActive() const;

    // Makes the bullet unavailable for updates and collisions.
    void deactivate();

private:
    float x;
    float y;
    float speedX;
    float speedY;
    float size;
    int damage;
    bool active;
};

#endif
