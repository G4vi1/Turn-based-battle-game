#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "Skill.hpp"
#include <string>
#include <vector>

using namespace std;

class Character {

private:

    // Attributes

    string name;
    double health, maxHealth;
    double evasion;
    double defense;

    int mana;
    int maxMana;

    bool isDefending;

    vector<Skill> skills;

public:

    // Constructor

    Character(const string& name,
              double health,
              double evasion,
              double defense,
              int mana);

    // State Queries

    bool isAlive() const;
    bool hasEnoughMana(int manaCost) const;
    bool isDefendingState() const;

    // Getters

    string getName() const;
    double getHealth() const;
    double getMaxHealth() const;
    double getDefense() const;
    double getEvasion() const;
    double getMana() const;
    double getMaxMana() const;

    const vector<Skill>& getSkills() const;

    // Mutators

    void defend();
    void resetDefense();

    bool useMana(int manaCost);

    void takeDamage(double damage);

    void attack(Character& target, const Skill& skill);

    void addSkill(const Skill& skill);

    void setHealth(double health);
};

#endif