#ifndef BULLET_HELL_BULLET_PATTERN_HPP
#define BULLET_HELL_BULLET_PATTERN_HPP

#include "BulletHell/Bullet.hpp"
#include <string>
#include <vector>

class BulletPattern
{
public:
    // Creates a vertical-rain pattern with its timing and bullet properties.
    BulletPattern(const std::string& name,
                  float duration,
                  float spawnInterval,
                  float bulletSpeed,
                  float bulletSize,
                  int bulletDamage);

    // Resets the internal timers before the pattern starts.
    void start();

    // Advances the pattern and adds new bullets to the supplied collection.
    void update(float deltaTime,
                std::vector<Bullet>& bullets,
                float arenaWidth);

    bool isFinished() const;
    const std::string& getName() const;
    float getDuration() const;

private:
    std::string name;
    float duration;
    float spawnInterval;
    float bulletSpeed;
    float bulletSize;
    int bulletDamage;
    float elapsedTime;
    float timeSinceLastSpawn;
    bool active;
};

#endif
