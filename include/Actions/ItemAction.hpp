#ifndef ITEM_ACTION_HPP
#define ITEM_ACTION_HPP

#include "Action.hpp"

// Concrete action that lets the player select and consume an inventory item.
class ItemAction : public Action
{
public:

    /* Executes the item action by allowing the player 
    to select an item from their inventory and consume it.
    */
    bool execute(Character& attacker, Character& defender) override;

    //Getter
    
    std::string getName() const override;
};

#endif
