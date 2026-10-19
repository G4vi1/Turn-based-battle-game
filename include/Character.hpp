#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "Skill.hpp"
#include "Inventory.hpp"
#include "BulletHell/BulletPattern.hpp"
#include <cstddef>
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
    vector<BulletPattern> bulletPatterns;
    Inventory inventory;

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
    const vector<BulletPattern>& getBulletPatterns() const;
    vector<BulletPattern>& getBulletPatterns();

    // Mutators

    void defend();
    void resetDefense();

    bool useMana(int manaCost);

    void takeDamage(double damage);
    void heal(double amount);
    void reset();
    void restoreMana(int amount);

    void attack(Character& target, const Skill& skill);

    void addSkill(const Skill& skill);
    void addBulletPattern(const BulletPattern& pattern);

    void setHealth(double health);
    // Inventory operations are kept on Character so the Battle class and UI
    // can ask a character to use an item without manipulating slot data.
    void addItem(const Item& item, int quantity);
    const Inventory& getInventory() const;
    Inventory& getInventory();
    bool canUseItem(std::size_t index) const;
    bool useItem(std::size_t index);
};

#endif
