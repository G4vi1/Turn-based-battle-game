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

// Restores health without exceeding the character's maximum health.
void Character::heal(double amount)
{
    if (amount <= 0)
    {
        return;
    }

    health += amount;

    if (health > maxHealth)
    {
        health = maxHealth;
    }
}

// Restores mana without exceeding the character's maximum mana.
void Character::restoreMana(int amount)
{
    if (amount <= 0)
    {
        return;
    }

    mana += amount;

    if (mana > maxMana)
    {
        mana = maxMana;
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

void Character::addItem(const Item& item, int quantity)
{
    inventory.addItem(item, quantity);
}

const Inventory& Character::getInventory() const
{
    return inventory;
}

Inventory& Character::getInventory()
{
    return inventory;
}

bool Character::canUseItem(size_t index) const
{
    const vector<InventorySlot>& slots = inventory.getSlots();

    if (!inventory.hasAvailableItem(index))
    {
        return false;
    }

    const Item& item = slots[index].item;
    const bool canRestoreHealth = item.getHealthRestore() > 0 && health < maxHealth;
    const bool canRestoreMana = item.getManaRestore() > 0 && mana < maxMana;

    return canRestoreHealth || canRestoreMana;
}

bool Character::useItem(size_t index)
{
    if (!canUseItem(index))
    {
        return false;
    }

    // Read the item before consuming it. Slots remain in the vector when their
    // quantity reaches zero, which lets menus show that an item is out of stock.
    const Item& item = inventory.getSlots()[index].item;
    heal(item.getHealthRestore());
    restoreMana(item.getManaRestore());

    return inventory.consume(index);
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
