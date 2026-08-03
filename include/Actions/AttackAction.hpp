#ifndef ATTACKACTION_HPP
#define ATTACKACTION_HPP

#include "Action.hpp"

// Concrete action responsible for executing
// the complete attack flow.

class AttackAction : public Action {

public:

    void execute(Character& attacker,
                 Character& defender) override;

    std::string getName() const override;
};

#endif