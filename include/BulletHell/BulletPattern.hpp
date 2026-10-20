#ifndef BULLET_HELL_BULLET_PATTERN_HPP
#define BULLET_HELL_BULLET_PATTERN_HPP

#include "BulletHell/Bullet.hpp"
#include <string>
#include <vector>

enum class BulletPatternType
{
    Rain,
    SideBarrage,
    Crossfire,
    Spiral,
    WallGap,
    DiagonalCross,
    Homing,
    LanceDrop,
    Maze
};

class BulletPattern
{
public:
    // Creates a pattern with its behavior, timing, and bullet properties.
    BulletPattern(const std::string& name,
                  BulletPatternType type,
                  float duration,
                  float spawnInterval,
                  float bulletSpeed,
                  float bulletSize,
                  int bulletDamage,
                  float arenaWidth,
                  float arenaHeight);

    // Resets the internal timers before the pattern starts.
    void start();

    // Advances the pattern and adds new bullets to the supplied collection.
    void update(float deltaTime,
                std::vector<Bullet>& bullets,
                float arenaWidth,
                float arenaHeight);

    bool isFinished() const;
    const std::string& getName() const;
    float getDuration() const;
    float getArenaWidth() const;
    float getArenaHeight() const;

private:
    std::string name;
    BulletPatternType type;
    float duration;
    float spawnInterval;
    float bulletSpeed;
    float bulletSize;
    int bulletDamage;
    float arenaWidth;
    float arenaHeight;
    float elapsedTime;
    float timeSinceLastSpawn;
    bool active;
    bool spawnFromLeft;
    float spiralAngle;
    int mazeOffset;
};

#endif
