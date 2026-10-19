#include "BulletHell/BulletPattern.hpp"
#include <algorithm>
#include <random>

BulletPattern::BulletPattern(const std::string& name,
                             float duration,
                             float spawnInterval,
                             float bulletSpeed,
                             float bulletSize,
                             int bulletDamage)
    : name(name),
      duration(duration),
      spawnInterval(spawnInterval),
      bulletSpeed(bulletSpeed),
      bulletSize(bulletSize),
      bulletDamage(bulletDamage),
      elapsedTime(0.0f),
      timeSinceLastSpawn(0.0f),
      active(false)
{
}

void BulletPattern::start()
{
    elapsedTime = 0.0f;
    timeSinceLastSpawn = 0.0f;
    active = duration > 0.0f && spawnInterval > 0.0f;
}

void BulletPattern::update(float deltaTime,
                           std::vector<Bullet>& bullets,
                           float arenaWidth)
{
    if (!active || deltaTime <= 0.0f || arenaWidth < bulletSize)
    {
        return;
    }

    // Never advance the pattern beyond its configured duration.
    const float remainingTime = duration - elapsedTime;
    const float timeToProcess = std::min(deltaTime, remainingTime);
    elapsedTime += timeToProcess;
    timeSinceLastSpawn += timeToProcess;

    static std::random_device randomDevice;
    static std::mt19937 generator(randomDevice());
    std::uniform_real_distribution<float> spawnPosition(0.0f,
                                                         arenaWidth - bulletSize);

    // A longer frame can cross more than one spawn interval.
    while (timeSinceLastSpawn >= spawnInterval)
    {
        bullets.emplace_back(spawnPosition(generator),
                             -bulletSize,
                             0.0f,
                             bulletSpeed,
                             bulletSize,
                             bulletDamage);
        timeSinceLastSpawn -= spawnInterval;
    }

    if (elapsedTime >= duration)
    {
        active = false;
    }
}

bool BulletPattern::isFinished() const
{
    return !active;
}

const std::string& BulletPattern::getName() const
{
    return name;
}

float BulletPattern::getDuration() const
{
    return duration;
}
