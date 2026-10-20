#include "BulletHell/Arena.hpp"
#include <algorithm>

Arena::Arena(float width, float height)
    : width(width),
      height(height),
      defaultWidth(width),
      defaultHeight(height),
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
    width = pattern.getArenaWidth();
    height = pattern.getArenaHeight();
    soul.keepInsideRoom(width, height);
    activePattern = &pattern;
    activePattern->start();

    if (activePattern->isFinished())
    {
        reset();
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
        bullet.update(deltaTime, soul);

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

    // Pattern duration is the final limit for every attack. This is important
    // for homing bullets, which could otherwise remain inside the arena.
    if (activePattern != nullptr && activePattern->isFinished())
    {
        bullets.clear();
        activePattern = nullptr;
    }
}

void Arena::reset()
{
    activePattern = nullptr;
    bullets.clear();
    damageTaken = 0;
    width = defaultWidth;
    height = defaultHeight;
    soul.keepInsideRoom(width, height);
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
