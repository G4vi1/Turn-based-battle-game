#include "GameData/Characters.hpp"
#include "Item.hpp"
#include "Skill.hpp"
#include "BulletHell/BulletPattern.hpp"

Character createHero()
{
    Character hero(
        "Archangel",
        100.0,
        0.8,
        12,
        60
    );

    Skill holyStrike(
        "Holy Strike",
        SkillType::Physical,
        0,
        25
    );

    Skill divineSpear(
        "Divine Spear",
        SkillType::Physical,
        15,
        35
    );

    Skill healingBlessing(
        "Healing Blessing",
        SkillType::Healing,
        20,
        15
    );

    Skill fourthSkill(
        "Fourth Skill",
        SkillType::Special,
        25,
        45
    );

    hero.addSkill(holyStrike);
    hero.addSkill(divineSpear);
    hero.addSkill(healingBlessing);
    hero.addSkill(fourthSkill);

    hero.addItem(Item("Potion", 30.0, 0), 2);
    hero.addItem(Item("Mana Potion", 0.0, 25), 2);
    hero.addItem(Item("Elixir", 20.0, 15), 1);

    return hero;
}

Character createEnemy()
{
    Character enemy(
        "Leviathan",
        100.0,
        0.4,
        15,
        70
    );

    enemy.addSkill(Skill("Shadow Claw", SkillType::Physical, 0, 20));
    enemy.addSkill(Skill("Arcane Blast", SkillType::Magical, 25, 45));
    enemy.addSkill(Skill("Devastating Roar", SkillType::Special, 30, 50));

    enemy.addBulletPattern(BulletPattern(
        "Shadow Rain",
        5.0f,
        0.35f,
        140.0f,
        14.0f,
        5));

    enemy.addBulletPattern(BulletPattern(
        "Dark Downpour",
        6.0f,
        0.22f,
        190.0f,
        12.0f,
        4));

    return enemy;
}
