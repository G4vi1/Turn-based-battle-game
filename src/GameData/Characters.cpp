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
        0
    );

    Skill divineSpear(
        "Divine Spear",
        SkillType::Physical,
        0,
        0
    );

    Skill healingBlessing(
        "Healing Blessing",
        SkillType::Magical,
        0,
        0
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

    return enemy;
}