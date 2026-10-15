#ifndef ITEM_HPP
#define ITEM_HPP

#include <string>

/*
Item is the immutable definition of a consumable.
it does not know who owns it or how many copies exist. 
those are the inventory's responsibilities. 
Keeping the item itself independent makes it
possible for multiple characters 
to share the same item definitions.
 */

class Item
{
public:

    Item(const std::string& name, double healthRestore, int manaRestore);

    const std::string& getName() const;
    double getHealthRestore() const;
    int getManaRestore() const;

private:
    std::string name;
    double healthRestore;
    int manaRestore;
};

#endif
