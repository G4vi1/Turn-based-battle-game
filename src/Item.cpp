#include "Item.hpp"

//Constructor for the Item class,

/*It includes the item's name
the amount of health it restores, 
and the amount of mana it restores.
*/

Item::Item(const std::string& name, double healthRestore, int manaRestore)
    : name(name),
      healthRestore(healthRestore),
      manaRestore(manaRestore)
{
}

//Getters

const std::string& Item::getName() const
{
    return name;
}

double Item::getHealthRestore() const
{
    return healthRestore;
}

int Item::getManaRestore() const
{
    return manaRestore;
}
