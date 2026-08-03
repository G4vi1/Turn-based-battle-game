#ifndef ACTION_HPP
#define ACTION_HPP

#include "Character.hpp"
#include <string>

// Abstract class that represents any action
// a character can perform during its turn.
//
// The Battle class knows only the concept of an Action.
// Each derived class is responsible for implementing
// its own behavior.

class Action {

public:

    // Virtual destructor to ensure proper cleanup
    // when using polymorphism.

    virtual ~Action() = default;

    // Executes the selected action.
    //
    // Each derived class implements
    // its own execution logic.

    virtual void execute(Character& attacker,
                         Character& defender) = 0;

    // Returns the action name.
    //
    // Used by the battle menu to display
    // the available actions dynamically.

    virtual std::string getName() const = 0;
};

#endif