#ifndef BULLET_HELL_ARENA_HPP
#define BULLET_HELL_ARENA_HPP

#include "BulletHell/BulletPattern.hpp"
#include "BulletHell/Soul.hpp"
#include <vector>

class Arena
{
    public:
        Arena(float width, float height);
        void placeSoul(float startX, float startY, float speed);
        void moveSoulUp(float deltaTime);
        void moveSoulDown(float deltaTime);
        void moveSoulLeft(float deltaTime);
        void moveSoulRight(float deltaTime);

        // Begins an attack pattern. The pattern must remain alive while active.
        void startPattern(BulletPattern& pattern);

        // Updates the current pattern and every active bullet.
        void update(float deltaTime);

        // Clears the current phase and its accumulated result.
        void reset();

        // Reports whether a pattern or its remaining bullets are still active.
        bool isPatternActive() const;

        // Returns the total damage accumulated during the current pattern.
        int getDamageTaken() const;

        float getWidth() const;
        float getHeight() const;
        const Soul& getSoul() const;
        const std::vector<Bullet>& getBullets() const;

    private:
        float width; // The width of the arena
        float height; // The height of the arena
        float defaultWidth;
        float defaultHeight;
        Soul soul; // The Soul object representing the player
        BulletPattern* activePattern;
        std::vector<Bullet> bullets;
        int damageTaken;
        float hitInvincibilityDuration;

};

#endif
