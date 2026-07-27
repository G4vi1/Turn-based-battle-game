#ifndef BATTLE_HPP
#define BATTLE_HPP

#include "Character.hpp"

using namespace std;

class Battle {

public:

    // Constructor

    Battle(Character& hero, Character& enemy);

    // Public methods

    void start();

private:

    // Private attributes

    Character& hero;
    Character& enemy;

    // Private methods

    string drawBar(double current, double maximum, char symbol);
    void displayStatus();
    bool turn(Character& attacker, Character& defender);
    int menu();
};

#endif