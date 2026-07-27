#ifndef SKILL_HPP
#define SKILL_HPP

#include <string>

using namespace std;

// Skill categories

enum class SkillType {
    Physical,
    Magical,
    Special,
    Move
};

class Skill {

private:

    // Attributes

    string name;
    SkillType type;
    int manaCost;
    double attackValue;

public:

    // Constructor

    Skill(const string& name,
          SkillType type,
          int manaCost,
          double attackValue);

    // Getters

    string getName() const;
    SkillType getType() const;
    int getManaCost() const;
    double getAttackValue() const;
};

#endif