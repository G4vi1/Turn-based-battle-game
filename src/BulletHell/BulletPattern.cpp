#include "BulletHell/BulletPattern.hpp"
#include <algorithm>
#include <cmath>
#include <random>

BulletPattern::BulletPattern(const std::string& name,
                             BulletPatternType type,
                             float duration, // The total time the pattern will be active.
                             float spawnInterval, // The time between each bullet spawn.
                             float bulletSpeed, // The speed of each bullet.
                             float bulletSize, // The size of each bullet.
                             int bulletDamage, // The damage of each bullet.
                             float arenaWidth,
                             float arenaHeight)
    : name(name), // The name of the bullet pattern.
      type(type), // The movement behavior used by the pattern.
      duration(duration), // The total time the pattern will be active.
      spawnInterval(spawnInterval), // The time between each bullet spawn.
      bulletSpeed(bulletSpeed), // The speed of each bullet.
      bulletSize(bulletSize), // The size of each bullet.
      bulletDamage(bulletDamage), // The damage of each bullet.
      arenaWidth(arenaWidth), // The width used while this pattern is active.
      arenaHeight(arenaHeight), // The height used while this pattern is active.
      elapsedTime(0.0f), // The time that has elapsed since the pattern started.
      timeSinceLastSpawn(0.0f),// The time that has elapsed since the last bullet was spawned.
      active(false), // Whether the pattern is currently active.
      spawnFromLeft(true), // The side used by the next lateral bullet.
      spiralAngle(0.0f), // The direction used by the next spiral bullet.
      mazeOffset(0) // The horizontal shift applied to the next maze opening.
{
}

// Resets the internal timers before the pattern starts.

void BulletPattern::start()
{
    // Reset the elapsed time and time since last spawn to zero.
    elapsedTime = 0.0f;
    // Reset the time since last spawn to zero.
    timeSinceLastSpawn = 0.0f;
    // The pattern is active if the duration and spawn interval are both positive.
    active = duration > 0.0f && spawnInterval > 0.0f &&
             arenaWidth >= bulletSize && arenaHeight >= bulletSize;
    spawnFromLeft = true;
    spiralAngle = 0.0f;
    mazeOffset = 0;
}

// Advances the pattern and adds new bullets to the supplied collection.

void BulletPattern::update(float deltaTime,
                           std::vector<Bullet>& bullets, // The collection of bullets to add new bullets to.
                           float arenaWidth,
                           float arenaHeight)
{

    // Stop when the pattern cannot create a valid bullet inside the arena.
    if (!active || deltaTime <= 0.0f ||
        arenaWidth < bulletSize || arenaHeight < bulletSize)
    {
        return;
    }

    // Never advance the pattern beyond its configured duration.
    const float remainingTime = duration - elapsedTime;
    // Process only the time that fits within the remaining duration.
    const float timeToProcess = std::min(deltaTime, remainingTime);
    elapsedTime += timeToProcess;
    timeSinceLastSpawn += timeToProcess;

    // Use a random number generator to determine the horizontal spawn position of each bullet.

    static std::random_device randomDevice;
    static std::mt19937 generator(randomDevice());
    std::uniform_real_distribution<float> horizontalPosition(
        0.0f, arenaWidth - bulletSize);
    std::uniform_real_distribution<float> verticalPosition(
        0.0f, arenaHeight - bulletSize);

    // A longer frame can cross more than one spawn interval.
    while (timeSinceLastSpawn >= spawnInterval)
    {
        if (type == BulletPatternType::Rain)
        {
            bullets.emplace_back(horizontalPosition(generator),
                                 -bulletSize,
                                 0.0f,
                                 bulletSpeed,
                                 bulletSize,
                                 bulletDamage);
        }
        else if (type == BulletPatternType::SideBarrage)
        {
            const float startX = spawnFromLeft ? -bulletSize : arenaWidth;
            const float speedX = spawnFromLeft ? bulletSpeed : -bulletSpeed;

            bullets.emplace_back(startX,
                                 verticalPosition(generator),
                                 speedX,
                                 0.0f,
                                 bulletSize,
                                 bulletDamage);
            spawnFromLeft = !spawnFromLeft;
        }
        else if (type == BulletPatternType::Crossfire)
        {
            const float spawnY = verticalPosition(generator);

            bullets.emplace_back(-bulletSize,
                                 spawnY,
                                 bulletSpeed,
                                 0.0f,
                                 bulletSize,
                                 bulletDamage);
            bullets.emplace_back(arenaWidth,
                                 spawnY,
                                 -bulletSpeed,
                                 0.0f,
                                 bulletSize,
                                 bulletDamage);
        }
        else if (type == BulletPatternType::Spiral)
        {
            const float centerX = (arenaWidth - bulletSize) / 2.0f;
            const float centerY = (arenaHeight - bulletSize) / 2.0f;

            bullets.emplace_back(centerX,
                                 centerY,
                                 std::cos(spiralAngle) * bulletSpeed,
                                 std::sin(spiralAngle) * bulletSpeed,
                                 bulletSize,
                                 bulletDamage);
            spiralAngle += 0.55f;
        }
        else if (type == BulletPatternType::WallGap)
        {
            const int columnCount = static_cast<int>(arenaWidth / bulletSize);
            const int gapWidth = 3;
            std::uniform_int_distribution<int> gapStart(
                0, std::max(0, columnCount - gapWidth));
            const int firstGapColumn = gapStart(generator);

            for (int column = 0; column < columnCount; ++column)
            {
                const bool isGap = column >= firstGapColumn &&
                                   column < firstGapColumn + gapWidth;

                if (!isGap)
                {
                    bullets.emplace_back(column * bulletSize,
                                         -bulletSize,
                                         0.0f,
                                         bulletSpeed,
                                         bulletSize,
                                         bulletDamage);
                }
            }
        }
        else if (type == BulletPatternType::DiagonalCross)
        {
            const float diagonalSpeed = bulletSpeed * 0.7f;

            bullets.emplace_back(-bulletSize,
                                 -bulletSize,
                                 diagonalSpeed,
                                 diagonalSpeed,
                                 bulletSize,
                                 bulletDamage);
            bullets.emplace_back(arenaWidth,
                                 -bulletSize,
                                 -diagonalSpeed,
                                 diagonalSpeed,
                                 bulletSize,
                                 bulletDamage);
        }
        else if (type == BulletPatternType::Homing)
        {
            bullets.emplace_back(horizontalPosition(generator),
                                 -bulletSize,
                                 0.0f,
                                 bulletSpeed,
                                 bulletSize,
                                 bulletDamage,
                                 -1.0f,
                                 true);
        }
        else if (type == BulletPatternType::LanceDrop)
        {
            const float lanceHeight = bulletSize * 5.0f;

            bullets.emplace_back(horizontalPosition(generator),
                                 -lanceHeight,
                                 0.0f,
                                 bulletSpeed,
                                 bulletSize,
                                 bulletDamage,
                                 lanceHeight);
        }
        else if (type == BulletPatternType::Maze)
        {
            const int columnCount = static_cast<int>(arenaWidth / bulletSize);
            const int gapWidth = 3;
            const int maximumGapStart = std::max(0, columnCount - gapWidth);
            const int gapStart = mazeOffset % (maximumGapStart + 1);

            for (int column = 0; column < columnCount; ++column)
            {
                const bool isGap = column >= gapStart &&
                                   column < gapStart + gapWidth;

                if (!isGap)
                {
                    bullets.emplace_back(column * bulletSize,
                                         -bulletSize,
                                         0.0f,
                                         bulletSpeed,
                                         bulletSize,
                                         bulletDamage);
                }
            }

            // Each new wall shifts the opening, forming a scrolling maze.
            mazeOffset += columnCount / 3 + 1;
        }
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

float BulletPattern::getArenaWidth() const
{
    return arenaWidth;
}

float BulletPattern::getArenaHeight() const
{
    return arenaHeight;
}
