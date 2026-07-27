#include "Battle.hpp"
#include <iostream>
#include <vector>

using namespace std;

// Constructor
// Initializes the battle by defining the two combatants.

Battle::Battle(Character& hero, Character& enemy)
    : hero(hero), enemy(enemy) {
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

        if (turn(enemy, hero)) {
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

int Battle::menu() {

    int choice;

    // Repeats until the player enters a valid option.

    do {

        // Displays the available actions.

        cout << "\n-------------------------------------\n"
             << "[ 1 ] Attack\n"
             << "[ 2 ] Defend\n"
             << "-------------------------------------\n";

        cin >> choice;

    } while (choice != 1 && choice != 2);

    return choice;
}

// Executes a single turn for the attacking character.

bool Battle::turn(Character& attacker, Character& defender) {

    // Displays the turn header.

    cout << "\n=====================================\n"
         << "Turn: " << attacker.getName() << "\n"
         << "=====================================\n\n";

    // Displays the current battle status.

    displayStatus();

    // Asks the player to choose an action.

    int choice = menu();

    // Attack option.

    if (choice == 1) {

        // Retrieves the attacker's skills and prompts the player
        // to choose a valid one.

        const vector<Skill>& skills = attacker.getSkills();
        int index;

        // Displays the available skills.

        do {

            cout << "\nChoose a skill:\n";

            for (int i = 0; i < (int)skills.size(); i++) {
                cout << i << " - " << skills[i].getName() << "\n";
            }

            cin >> index;

            // Checks whether the selected index is valid.

            if (index < 0 || index >= (int)skills.size()) {

                cout << "\nInvalid skill!\n\n";

            }

            // Checks whether the attacker has enough mana.

            else if (!attacker.hasEnoughMana(skills[index].getManaCost())) {

                cout << "\nNot enough mana!\n"
                     << "Current mana: " << attacker.getMana()
                     << " | Cost: " << skills[index].getManaCost()
                     << "\n";
            }

            // Repeats until a valid skill can be used.

        } while (index < 0
              || index >= (int)skills.size()
              || !attacker.hasEnoughMana(skills[index].getManaCost()));

        // Executes the attack.

        attacker.attack(defender, skills[index]);

        // Displays the attack result.

        cout << "\n"
             << attacker.getName() << " used "
             << skills[index].getName() << "!\n\n"
             << defender.getName() << " now has "
             << defender.getHealth() << " HP.\n";

        // Removes the defender's defensive state.

        defender.resetDefense();

    }

    // Defense option.

    else {

        // Activates the defensive state.

        attacker.defend();

        // Displays the chosen action.

        cout << "\n"
             << attacker.getName()
             << " is now defending!\n";
    }

    // Returns whether the defender has been defeated.

    return !defender.isAlive();
}

