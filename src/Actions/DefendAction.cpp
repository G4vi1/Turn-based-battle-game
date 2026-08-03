#include "Actions/DefendAction.hpp"

#include <iostream>

using namespace std;

// Executes the defend action.

void DefendAction::execute(Character& attacker, Character& defender)
{
    // Activates the defensive state.

    attacker.defend();

    // Displays the action result.

    cout << "\n"
         << attacker.getName()
         << " is now defending!\n";
}

// Returns the action name displayed in the menu.

string DefendAction::getName() const
{
    return "Defend";
}