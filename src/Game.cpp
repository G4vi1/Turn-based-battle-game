#include "Game.hpp"

#include "Battle.hpp"
#include "Character.hpp"
#include "Skill.hpp"
#include "GameData/Characters.hpp"

void Game::run()
{
    initialize();

    Character hero = createHero();
    Character enemy = createEnemy();

    // Start battle

    Battle battle(hero, enemy);
    battle.start();

    shutdown();
}

void Game::initialize()
{
    // Initialization logic
}

void Game::shutdown()
{
    // Cleanup logic
}
