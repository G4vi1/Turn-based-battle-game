#include "Character.hpp"
#include <iostream>

using namespace std;

// Constructor

Character::Character(const string& name, double health, double evasion, double defense, int mana)
    : name(name), health(health), maxHealth(health),
      evasion(evasion), defense(defense),
      mana(mana), maxMana(mana),
      isDefending(false) {
}

// State Queries

// Checks whether the character is alive.

bool Character::isAlive() const {
    return health > 0;
}

// Checks whether the character has enough mana to use a skill.

bool Character::hasEnoughMana(int manaCost) const {
    return manaCost <= mana;
}

// Checks whether the character is currently defending.

bool Character::isDefendingState() const {
    return isDefending;
}

// Getters

string Character::getName() const {
    return name;
}

double Character::getHealth() const {
    return health;
}

double Character::getMaxHealth() const {
    return maxHealth;
}

double Character::getDefense() const {
    return defense;
}

double Character::getMana() const {
    return mana;
}

double Character::getMaxMana() const {
    return maxMana;
}

double Character::getEvasion() const {
    return evasion;
}

const vector<Skill>& Character::getSkills() const {
    return skills;
}

// Mutators

// Consumes the character's mana.

bool Character::useMana(int manaCost) {
    if (hasEnoughMana(manaCost)) {
        mana -= manaCost;
        return true;
    }
    return false;
}

// Applies damage to the character.

void Character::takeDamage(double damage) {
    if (damage < 0) {
        cout << "Error";
        return;
    }

    health -= damage;

    if (health < 0) {
        health = 0;
    }
}

// Activates the character's defensive state.

void Character::defend() {
    isDefending = true;
}

// Resets the character's defensive state.

void Character::resetDefense() {
    isDefending = false;
}

// Adds a new skill to the character.

void Character::addSkill(const Skill& skill) {
    skills.push_back(skill);
}

// Centralizes the character's attack logic.
// Based on the selected skill, checks if the attack
// can be executed, calculates the final damage,
// and updates the target's state.

void Character::attack(Character& target, const Skill& skill) {

    // Checks if there is enough mana to perform the attack.

    int manaCost = skill.getManaCost();

    if (!useMana(manaCost)) {
        return;
    }

    double attackValue = skill.getAttackValue();

    double resistance = target.getDefense() * target.getEvasion();

    // Increases resistance if the target is defending.

    if (target.isDefendingState()) {
        resistance *= 1.3;
    }

    // Calculates and applies the final damage.

    double damage = attackValue - resistance;

    if (damage < 0) {
        damage = 0;
    }

    target.takeDamage(damage);
}

void Character::setHealth(double health)
{
    this->health = health;

    if (this->health < 0)
    {
        this->health = 0;
    }

    if (this->health > maxHealth)
    {
        this->health = maxHealth;
    }
}