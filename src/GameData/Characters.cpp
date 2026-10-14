#include "GameData/Characters.hpp"
#include "Skill.hpp"

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
        SkillType::Special,
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

    return enemy;
}
