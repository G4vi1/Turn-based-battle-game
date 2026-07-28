#include "Game.hpp"

#include "Battle.hpp"
#include "Character.hpp"
#include "Skill.hpp"

void Game::run()
{
    initialize();

    // Hero skills

    Skill holyStrike("Holy Strike", SkillType::Physical, 10, 25.0);
    Skill divineSpear("Divine Spear", SkillType::Physical, 15, 35.0);
    Skill healingBlessing("Healing Blessing", SkillType::Special, 20, 15.0);

    // Enemy skills

    Skill shadowClaw("Shadow Claw", SkillType::Physical, 8, 20.0);
    Skill arcaneBlast("Arcane Blast", SkillType::Magical, 25, 45.0);
    Skill devastatingRoar("Devastating Roar", SkillType::Special, 30, 50.0);

    // Characters

    Character hero("Archangel", 100.0, 0.8, 12, 60);
    Character enemy("Leviathan", 100.0, 0.4, 15, 70);

    // Add hero skills

    hero.addSkill(holyStrike);
    hero.addSkill(divineSpear);
    hero.addSkill(healingBlessing);

    // Add enemy skills

    enemy.addSkill(shadowClaw);
    enemy.addSkill(arcaneBlast);
    enemy.addSkill(devastatingRoar);

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