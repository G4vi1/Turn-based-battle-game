#include "Actions/AttackAction.hpp"

#include <iostream>

using namespace std;

// Executes the complete attack flow.

void AttackAction::execute(Character& attacker, Character& defender)
{
    // Retrieves the attacker's available skills.

    const vector<Skill>& skills = attacker.getSkills();

    int index;

    // Prompts the player to choose a valid skill.

    do {

        cout << "\nChoose a skill:\n";

        for (int i = 0; i < (int)skills.size(); i++)
        {
            cout << i
                 << " - "
                 << skills[i].getName()
                 << "\n";
        }

        cin >> index;

        // Checks whether the selected skill exists.

        if (index < 0 || index >= (int)skills.size())
        {
            cout << "\nInvalid skill!\n";
        }

        // Checks whether the attacker has enough mana.

        else if (!attacker.hasEnoughMana(skills[index].getManaCost()))
        {
            cout << "\nNot enough mana!\n"
                 << "Current mana: "
                 << attacker.getMana()
                 << " | Cost: "
                 << skills[index].getManaCost()
                 << "\n";
        }

    } while (index < 0
          || index >= (int)skills.size()
          || !attacker.hasEnoughMana(skills[index].getManaCost()));

    // Executes the selected attack.

    attacker.attack(defender, skills[index]);

    // Displays the attack result.

    cout << "\n"
         << attacker.getName()
         << " used "
         << skills[index].getName()
         << "!\n\n"
         << defender.getName()
         << " now has "
         << defender.getHealth()
         << " HP.\n";

    // Removes the defender's defensive state.

    defender.resetDefense();
}

// Returns the action name displayed in the menu.

string AttackAction::getName() const
{
    return "Attack";
}