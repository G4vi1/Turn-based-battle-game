#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include "Item.hpp"
#include <cstddef>
#include <vector>

/* InventorySlot is a struct that 
represents a slot in the inventory,
containing an item and its quantity.*/

struct InventorySlot
{
    Item item;
    int quantity;
};



class Inventory
{
public:
    
    /* Adds an item object to the inventory 
    with the specified quantity.*/

    void addItem(const Item& item, int quantity);

    /* Returns a constant reference to the vector of inventory slots,
    allowing read-only access to the inventory's contents.*/

    /*Think like a "dynamic" getter, or a getter for a list*/

    const std::vector<InventorySlot>& getSlots() const;

    /* Checks if an item at the specified 
    index is available (quantity > 0). */

    bool hasAvailableItem(std::size_t index) const;

    /*Removes exactly one copy of the item at the specified 
    index after another class has successfully 
    applied the item's effect.
    A failed consume never makes quantity negative.*/
    bool consume(std::size_t index);

private:

    std::vector<InventorySlot> slots;
};

#endif
