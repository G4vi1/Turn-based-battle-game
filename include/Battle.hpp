#ifndef BATTLE_HPP
#define BATTLE_HPP

#include "Character.hpp"
#include "Actions/Action.hpp"
#include "BulletHell/Arena.hpp"
#include <string>
#include <vector>

using namespace std;

class Battle {

public:

    // Constructor

    Battle(Character& hero, Character& enemy);

    // Destructor

    ~Battle();

    // Public methods

    void start();

    bool heroUseSkill(std::size_t skillIndex, std::string& message);
    bool heroUseItem(std::size_t itemIndex, std::string& message);
    void heroDefend(std::string& message);
    void enemyTurn(std::string& message);

    // Bullet Hell phase API. The SDL layer will call update and movement
    // methods once per frame while this phase is active.
    bool startEnemyBulletHell(std::string& message);
    void updateBulletHell(float deltaTime);
    void moveBulletHellSoulUp(float deltaTime);
    void moveBulletHellSoulDown(float deltaTime);
    void moveBulletHellSoulLeft(float deltaTime);
    void moveBulletHellSoulRight(float deltaTime);
    bool isBulletHellActive() const;
    bool isBulletHellReadyToResolve() const;
    bool resolveBulletHell(std::string& message);
    const Arena& getBulletHellArena() const;
    bool hasEnded() const;
    void reset();

    Character& getHero();
    Character& getEnemy();

private:

    // Private attributes

    Character& hero;
    Character& enemy;
    Arena bulletHellArena;
    bool bulletHellInProgress;
    std::string activeBulletPatternName;

    // Stores the available actions for the battle.

    std::vector<Action*> actions;

    // Private methods

    string drawBar(double current, double maximum, char symbol);
    void displayStatus();
    bool turn(Character& attacker, Character& defender);
    bool enemyTurn();
    int menu();
};

#endif
