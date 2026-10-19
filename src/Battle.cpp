#include "Battle.hpp"
#include <iostream>
#include <random>
#include <vector>
#include "Actions/Actions.hpp"

using namespace std;

// Constructor
// Initializes the battle by defining the two combatants.

Battle::Battle(Character& hero, Character& enemy)
    : hero(hero),
      enemy(enemy),
      bulletHellArena(400.0f, 250.0f),
      bulletHellInProgress(false)
{
    actions.push_back(new AttackAction());
    actions.push_back(new DefendAction());
    actions.push_back(new ItemAction());
}

// Destructor
// Cleans up the dynamically allocated actions.

Battle::~Battle()
{
    for (auto action : actions)
    {
        delete action;
    }
}

// Draws the health and mana bars.

string Battle::drawBar(double current, double maximum, char symbol) {

    // Calculates the number of filled slots based on the ratio
    // between the current and maximum values.

    const int barSize = 20;
    int filledBars = (current / maximum) * barSize;

    string bar = "[";

    for (int i = 0; i < filledBars; i++) {
        bar += symbol;
    }

    for (int i = filledBars; i < barSize; i++) {
        bar += " ";
    }

    bar += "]";

    return bar;
}

// Displays the current status of both characters.

void Battle::displayStatus() {

    // Retrieves all information required to display the hero's status.

    string heroName = hero.getName();

    double heroHealth = hero.getHealth();
    double heroMaxHealth = hero.getMaxHealth();

    double heroMana = hero.getMana();
    double heroMaxMana = hero.getMaxMana();

    // Retrieves all information required to display the enemy's status.

    string enemyName = enemy.getName();

    double enemyHealth = enemy.getHealth();
    double enemyMaxHealth = enemy.getMaxHealth();

    double enemyMana = enemy.getMana();
    double enemyMaxMana = enemy.getMaxMana();

    // Displays the current battle status.

    cout << "\n========== BATTLE STATUS ==========\n\n"

         << heroName << "\n"
         << "HP "
         << drawBar(heroHealth, heroMaxHealth, '=')
         << " " << heroHealth << "\n"
         << "MP "
         << drawBar(heroMana, heroMaxMana, '*')
         << " " << heroMana << "\n\n"

         << enemyName << "\n"
         << "HP "
         << drawBar(enemyHealth, enemyMaxHealth, '=')
         << " " << enemyHealth << "\n"
         << "MP "
         << drawBar(enemyMana, enemyMaxMana, '*')
         << " " << enemyMana << "\n\n"

         << "==================================\n\n";
}

// Controls the main battle loop,
// alternating turns until one character is defeated.

void Battle::start() {

    cout << "=====================================\n"
         << "        BATTLE STARTS!\n"
         << "=====================================\n\n";

    // Main battle loop.
    // Continues while both characters are alive.

    while (hero.isAlive() && enemy.isAlive()) {

        // Executes one turn for each combatant,
        // stopping the battle if one is defeated.

        if (turn(hero, enemy)) {
            break;
        }

        string enemyMessage;
        enemyTurn(enemyMessage);
        cout << enemyMessage << "\n";

        if (!hero.isAlive()) {
            break;
        }
    }

    // Displays the final status of the battle.

    displayStatus();

    // Determines the winner and displays the battle result.

    if (!hero.isAlive()) {

        cout << "\n"
             << hero.getName() << " has been defeated!\n\n"
             << enemy.getName() << " WINS!!!!!!!!\n";

    } else {

        cout << "\n"
             << enemy.getName() << " has been defeated!\n\n"
             << hero.getName() << " WINS!!!!!!!!\n";
    }

    cout << "\nBattle ended!\n";
}

// Displays the action menu and returns the player's choice.

int Battle::menu()
{
    int choice;

    do
    {
        cout << "\nChoose an action:\n";

        // Displays all registered actions dynamically.

        for (int i = 0; i < (int)actions.size(); i++)
        {
            cout << i
                 << " - "
                 << actions[i]->getName()
                 << "\n";
        }

        cin >> choice;

        if (choice < 0 || choice >= (int)actions.size())
        {
            cout << "\nInvalid option!\n";
        }

    } while (choice < 0 || choice >= (int)actions.size());

    return choice;
}
// Executes a single turn for the attacking character.

bool Battle::turn(Character& attacker, Character& defender)
{
    // Displays the turn header.

    cout << "\n=====================================\n"
         << "Turn: " << attacker.getName() << "\n"
         << "=====================================\n\n";

    // Displays the current battle status.

    displayStatus();

    // Keeps asking for an action until one is performed. An action such as
    // "Back" from the item menu returns false and does not spend the turn.
    bool actionPerformed = false;

    while (!actionPerformed)
    {
        int index = menu();
        actionPerformed = actions[index]->execute(attacker, defender);
    }

    // Returns whether the defender has been defeated.

    return !defender.isAlive();
}

bool Battle::heroUseSkill(size_t skillIndex, string& message)
{
    const vector<Skill>& skills = hero.getSkills();

    if (skillIndex >= skills.size())
    {
        message = "Invalid skill.";
        return false;
    }

    const Skill& selectedSkill = skills[skillIndex];

    if (!hero.hasEnoughMana(selectedSkill.getManaCost()))
    {
        message = "Not enough mana.";
        return false;
    }

    if (selectedSkill.getType() == SkillType::Healing)
    {
        hero.useMana(selectedSkill.getManaCost());
        hero.heal(selectedSkill.getAttackValue());
        message = hero.getName() + " used " + selectedSkill.getName() +
                  " and recovered " +
                  to_string(static_cast<int>(selectedSkill.getAttackValue())) + " HP.";
    }
    else
    {
        hero.attack(enemy, selectedSkill);
        enemy.resetDefense();
        message = hero.getName() + " used " + selectedSkill.getName() +
                  "! " + enemy.getName() + " has " +
                  to_string(static_cast<int>(enemy.getHealth())) + " HP.";
    }
    return true;
}

bool Battle::heroUseItem(size_t itemIndex, string& message)
{
    const vector<InventorySlot>& slots = hero.getInventory().getSlots();

    if (itemIndex >= slots.size())
    {
        message = "Invalid item.";
        return false;
    }

    const Item& item = slots[itemIndex].item;
    if (!hero.useItem(itemIndex))
    {
        message = item.getName() + " cannot be used right now.";
        return false;
    }

    message = hero.getName() + " used " + item.getName() + "!";
    return true;
}

void Battle::heroDefend(string& message)
{
    hero.defend();
    message = hero.getName() + " is now defending!";
}

void Battle::enemyTurn(string& message)
{
    const vector<Skill>& skills = enemy.getSkills();
    vector<const Skill*> availableSkills;

    for (const Skill& skill : skills)
    {
        if (enemy.hasEnoughMana(skill.getManaCost()))
        {
            availableSkills.push_back(&skill);
        }
    }

    static random_device randomDevice;
    static mt19937 generator(randomDevice());
    uniform_int_distribution<int> chance(1, 100);
    const double healthRatio = enemy.getHealth() / enemy.getMaxHealth();

    if (availableSkills.empty() || (healthRatio <= 0.30 && chance(generator) <= 60))
    {
        enemy.defend();
        message = enemy.getName() + " is now defending!";
        return;
    }

    uniform_int_distribution<size_t> skillChoice(0, availableSkills.size() - 1);
    const Skill& selectedSkill = *availableSkills[skillChoice(generator)];
    enemy.attack(hero, selectedSkill);
    hero.resetDefense();

    message = enemy.getName() + " used " + selectedSkill.getName() +
              "! " + hero.getName() + " has " +
              to_string(static_cast<int>(hero.getHealth())) + " HP.";
}

bool Battle::startEnemyBulletHell(string& message)
{
    if (hasEnded())
    {
        message = "The battle has already ended.";
        return false;
    }

    if (bulletHellInProgress)
    {
        message = "A Bullet Hell pattern is already active.";
        return false;
    }

    vector<BulletPattern>& patterns = enemy.getBulletPatterns();

    if (patterns.empty())
    {
        message = enemy.getName() + " has no Bullet Hell patterns.";
        return false;
    }

    static random_device randomDevice;
    static mt19937 generator(randomDevice());
    uniform_int_distribution<size_t> patternChoice(0, patterns.size() - 1);

    BulletPattern& selectedPattern = patterns[patternChoice(generator)];
    const Soul& soul = bulletHellArena.getSoul();
    bulletHellArena.placeSoul(
        (bulletHellArena.getWidth() - soul.getSize()) / 2.0f,
        (bulletHellArena.getHeight() - soul.getSize()) / 2.0f,
        220.0f);
    bulletHellArena.startPattern(selectedPattern);
    bulletHellInProgress = bulletHellArena.isPatternActive();
    activeBulletPatternName = selectedPattern.getName();

    if (!bulletHellInProgress)
    {
        message = selectedPattern.getName() + " could not start.";
        return false;
    }

    message = enemy.getName() + " used " + activeBulletPatternName + "!";
    return true;
}

void Battle::updateBulletHell(float deltaTime)
{
    if (bulletHellInProgress)
    {
        bulletHellArena.update(deltaTime);
    }
}

void Battle::moveBulletHellSoulUp(float deltaTime)
{
    if (bulletHellInProgress)
    {
        bulletHellArena.moveSoulUp(deltaTime);
    }
}

void Battle::moveBulletHellSoulDown(float deltaTime)
{
    if (bulletHellInProgress)
    {
        bulletHellArena.moveSoulDown(deltaTime);
    }
}

void Battle::moveBulletHellSoulLeft(float deltaTime)
{
    if (bulletHellInProgress)
    {
        bulletHellArena.moveSoulLeft(deltaTime);
    }
}

void Battle::moveBulletHellSoulRight(float deltaTime)
{
    if (bulletHellInProgress)
    {
        bulletHellArena.moveSoulRight(deltaTime);
    }
}

bool Battle::isBulletHellActive() const
{
    return bulletHellInProgress;
}

bool Battle::isBulletHellReadyToResolve() const
{
    return bulletHellInProgress && !bulletHellArena.isPatternActive();
}

bool Battle::resolveBulletHell(string& message)
{
    if (!bulletHellInProgress)
    {
        message = "No Bullet Hell pattern is active.";
        return false;
    }

    if (!isBulletHellReadyToResolve())
    {
        message = "The Bullet Hell pattern has not finished.";
        return false;
    }

    const int damageTaken = bulletHellArena.getDamageTaken();
    hero.takeDamage(damageTaken);
    hero.resetDefense();
    bulletHellInProgress = false;

    message = hero.getName() + " took " + to_string(damageTaken) +
              " damage from " + activeBulletPatternName + ".";
    activeBulletPatternName.clear();
    return true;
}

const Arena& Battle::getBulletHellArena() const
{
    return bulletHellArena;
}

bool Battle::hasEnded() const
{
    return !hero.isAlive() || !enemy.isAlive();
}

void Battle::reset()
{
    hero.reset();
    enemy.reset();
    bulletHellArena.reset();
    bulletHellInProgress = false;
    activeBulletPatternName.clear();
}

Character& Battle::getHero()
{
    return hero;
}

Character& Battle::getEnemy()
{
    return enemy;
}
