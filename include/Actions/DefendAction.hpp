#ifndef DEFENDACTION_HPP
#define DEFENDACTION_HPP

#include "Action.hpp"

// Concrete action responsible for executing
// the defend action.

class DefendAction : public Action {

public:

    void execute(Character& attacker,
                 Character&defender) override;

    std::string getName() const override;
};

#endif