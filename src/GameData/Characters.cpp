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
        BulletPatternType::Rain,
        8.0f,
        0.35f,
        140.0f,
        14.0f,
        5,
        400.0f,
        250.0f));

    enemy.addBulletPattern(BulletPattern(
        "Dark Downpour",
        BulletPatternType::Rain,
        10.0f,
        0.22f,
        190.0f,
        12.0f,
        4,
        400.0f,
        250.0f));

    enemy.addBulletPattern(BulletPattern(
        "Abyssal Barrage",
        BulletPatternType::SideBarrage,
        9.0f,
        0.45f,
        180.0f,
        16.0f,
        6,
        350.0f,
        220.0f));

    enemy.addBulletPattern(BulletPattern(
        "Twin Eclipse",
        BulletPatternType::Crossfire,
        11.0f,
        0.65f,
        155.0f,
        13.0f,
        5,
        380.0f,
        230.0f));

    enemy.addBulletPattern(BulletPattern(
        "Abyss Spiral",
        BulletPatternType::Spiral,
        12.0f,
        0.18f,
        150.0f,
        10.0f,
        4,
        380.0f,
        240.0f));

    enemy.addBulletPattern(BulletPattern(
        "Void Gate",
        BulletPatternType::WallGap,
        11.0f,
        1.15f,
        120.0f,
        20.0f,
        7,
        320.0f,
        200.0f));

    enemy.addBulletPattern(BulletPattern(
        "Falling Scythes",
        BulletPatternType::DiagonalCross,
        10.0f,
        0.55f,
        170.0f,
        15.0f,
        6,
        350.0f,
        220.0f));

    enemy.addBulletPattern(BulletPattern(
        "Abyss Hunter",
        BulletPatternType::Homing,
        12.0f,
        0.90f,
        115.0f,
        16.0f,
        7,
        360.0f,
        230.0f));

    enemy.addBulletPattern(BulletPattern(
        "Falling Lances",
        BulletPatternType::LanceDrop,
        10.0f,
        0.80f,
        240.0f,
        12.0f,
        8,
        280.0f,
        200.0f));

    enemy.addBulletPattern(BulletPattern(
        "Dread Maze",
        BulletPatternType::Maze,
        12.0f,
        0.75f,
        85.0f,
        18.0f,
        6,
        300.0f,
        190.0f));

    return enemy;
}
