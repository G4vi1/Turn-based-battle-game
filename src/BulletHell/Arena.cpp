#include "BulletHell/Arena.hpp"
#include <algorithm>

Arena::Arena(float width, float height)
    : width(width),
      height(height),
      soul(0.0f, 0.0f, 0.0f),
      activePattern(nullptr),
      damageTaken(0),
      hitInvincibilityDuration(0.5f)
{
}

void Arena::placeSoul(float startX, float startY, float speed)
{
    soul = Soul(startX, startY, speed);
}

void Arena::moveSoulUp(float deltaTime)
{
    soul.moveUp(deltaTime);
    soul.keepInsideRoom(width, height);
}

void Arena::moveSoulDown(float deltaTime)
{
    soul.moveDown(deltaTime);
    soul.keepInsideRoom(width, height);
}

void Arena::moveSoulLeft(float deltaTime)
{
    soul.moveLeft(deltaTime);
    soul.keepInsideRoom(width, height);
}

void Arena::moveSoulRight(float deltaTime)
{
    soul.moveRight(deltaTime);
    soul.keepInsideRoom(width, height);
}

void Arena::startPattern(BulletPattern& pattern)
{
    bullets.clear();
    damageTaken = 0;
    activePattern = &pattern;
    activePattern->start();

    if (activePattern->isFinished())
    {
        activePattern = nullptr;
    }
}

void Arena::update(float deltaTime)
{
    soul.updateInvincibility(deltaTime);

    if (activePattern != nullptr)
    {
        activePattern->update(deltaTime, bullets, width, height);
    }

    for (Bullet& bullet : bullets)
    {
        bullet.update(deltaTime);

        // A bullet can damage the Soul once before being removed.
        if (soul.canTakeDamage() && bullet.collidesWith(soul))
        {
            damageTaken += bullet.getDamage();
            bullet.deactivate();
            soul.startInvincibility(hitInvincibilityDuration);
        }
    }

    bullets.erase(
        std::remove_if(bullets.begin(),
                       bullets.end(),
                       [this](const Bullet& bullet)
                       {
                           return !bullet.isActive() ||
                                  bullet.isOutsideArena(width, height);
                       }),
        bullets.end());

    // The phase ends only after the pattern and all spawned bullets finish.
    if (activePattern != nullptr && activePattern->isFinished() && bullets.empty())
    {
        activePattern = nullptr;
    }
}

void Arena::reset()
{
    activePattern = nullptr;
    bullets.clear();
    damageTaken = 0;
}

bool Arena::isPatternActive() const
{
    return activePattern != nullptr;
}

int Arena::getDamageTaken() const
{
    return damageTaken;
}

float Arena::getWidth() const
{
    return width;
}

float Arena::getHeight() const
{
    return height;
}

const Soul& Arena::getSoul() const
{
    return soul;
}

const std::vector<Bullet>& Arena::getBullets() const
{
    return bullets;
}
