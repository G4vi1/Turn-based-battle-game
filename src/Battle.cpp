#include "Battle.hpp"
#include <iostream>
#include <random>
#include <vector>
#include "Actions/Actions.hpp"

using namespace std;

// Constructor
// Initializes the battle by defining the two combatants.

Battle::Battle(Character& hero, Character& enemy)
    : hero(hero), enemy(enemy)
{
    actions.push_back(new AttackAction());
    actions.push_back(new DefendAction());
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

        if (enemyTurn()) {
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

    // Prompts the player to choose an action.

    int index = menu();

    // Executes the selected action.

    actions[index]->execute(attacker, defender);

    // Returns whether the defender has been defeated.

    return !defender.isAlive();
}

// Executes the enemy's turn automatically.

bool Battle::enemyTurn()
{
    cout << "\n=====================================\n"
         << "Turn: " << enemy.getName() << "\n"
         << "=====================================\n\n";

    displayStatus();

    // Keeps only skills the enemy can afford to use.

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

    // When health is low, the enemy has a 60% chance to defend.
    double healthRatio = enemy.getHealth() / enemy.getMaxHealth();
    uniform_int_distribution<int> chance(1, 100);

    if (availableSkills.empty() || (healthRatio <= 0.30 && chance(generator) <= 60))
    {
        enemy.defend();

        cout << "\n"
             << enemy.getName()
             << " is now defending!\n";

        return false;
    }

    // Chooses one affordable skill at random.

    uniform_int_distribution<size_t> skillChoice(0, availableSkills.size() - 1);
    const Skill& selectedSkill = *availableSkills[skillChoice(generator)];

    enemy.attack(hero, selectedSkill);

    cout << "\n"
         << enemy.getName()
         << " used "
         << selectedSkill.getName()
         << "!\n\n"
         << hero.getName()
         << " now has "
         << hero.getHealth()
         << " HP.\n";

    // An attack consumes the target's defensive state.

    hero.resetDefense();

    return !hero.isAlive();
}
