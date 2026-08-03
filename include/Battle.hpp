#ifndef BATTLE_HPP
#define BATTLE_HPP

#include "Character.hpp"
#include "Actions/Action.hpp"
#include <vector>

using namespace std;

class Battle {

public:

    // Constructor

    Battle(Character& hero, Character& enemy);

    // Destructor

    ~Battle();

    // Public methods

    void start();

private:

    // Private attributes

    Character& hero;
    Character& enemy;

    // Stores the available actions for the battle.

    std::vector<Action*> actions;

    // Private methods

    string drawBar(double current, double maximum, char symbol);
    void displayStatus();
    bool turn(Character& attacker, Character& defender);
    int menu();
};

#endif