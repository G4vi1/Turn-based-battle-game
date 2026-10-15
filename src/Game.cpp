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

    // Starting consumables for the hero. Item definitions can be reused by
    // shops, loot, enemies, or the future SDL inventory menu.
    Item potion("Potion", 30.0, 0);
    Item manaPotion("Mana Potion", 0.0, 25);
    Item elixir("Elixir", 20.0, 15);

    hero.addItem(potion, 2);
    hero.addItem(manaPotion, 2);
    hero.addItem(elixir, 1);

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
