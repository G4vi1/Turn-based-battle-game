#include "Inventory.hpp"

/* Adds an item object to the inventory 
    with the specified quantity.*/

void Inventory::addItem(const Item& item, int quantity)
{
    // Ignore invalid additions. 
    if (quantity <= 0)
    {
        return;
    }

    /* If an item with the same name is already in the inventory,
    its existing slot is increased instead of creating a duplicate.
    */

    for (InventorySlot& slot : slots)
    {

        // Check if the item names match to determine if it's the same item.

        if (slot.item.getName() == item.getName())
        {   

            // Increase the quantity of the existing slot.
            slot.quantity += quantity;
            return;
        }
    }

    // If the item is not found, create a new slot for it.

    slots.push_back({item, quantity});
}

/* Returns a constant reference to the vector of inventory slots,
   allowing read-only access to the inventory's contents.
*/

const std::vector<InventorySlot>& Inventory::getSlots() const
{
    return slots;
}

/* Checks if an item at the specified index is available 
(has a quantity greater than zero).*/
bool Inventory::hasAvailableItem(std::size_t index) const
{
    // Check if the index is valid and the quantity is greater than zero.
    return index < slots.size() && slots[index].quantity > 0;
}

/* Consumes an item at the specified index 
reducing its quantity by one. */

bool Inventory::consume(std::size_t index)
{

    /* 
    */
    if (!hasAvailableItem(index))
    {
        return false;
    }

    --slots[index].quantity;
    return true;
}
