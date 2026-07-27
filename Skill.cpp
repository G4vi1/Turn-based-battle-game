#include "Skill.hpp"

using namespace std;

// Constructor

Skill::Skill(const string& name, SkillType type, int manaCost, double attackValue)
    : name(name), type(type), manaCost(manaCost), attackValue(attackValue) {
}

// Getters

string Skill::getName() const {
    return name;
}

SkillType Skill::getType() const {
    return type;
}

int Skill::getManaCost() const {
    return manaCost;
}

double Skill::getAttackValue() const {
    return attackValue;
}