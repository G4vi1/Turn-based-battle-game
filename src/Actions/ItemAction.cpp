#include "Actions/ItemAction.hpp"

#include <iostream>

using namespace std;

/* Executes the item action by allowing the player to select
an item from their inventory and consume it. */

bool ItemAction::execute(Character& attacker, Character& defender)
{
    // Items do not affect the enemy yet.
    (void)defender;

    // Gets the items owned by the character whose turn it is.
    const vector<InventorySlot>& slots = attacker.getInventory().getSlots();

    // Ends the action if the inventory is empty.
    if (slots.empty())
    {
        cout << "\nNo items available!\n";
        return false;
    }

    int index;

    do
    {
        cout << "\nChoose an item:\n";

        // Shows each item, its quantity, and its effects.
        for (size_t itemIndex = 0; itemIndex < slots.size(); ++itemIndex)
        {
            const InventorySlot& slot = slots[itemIndex];

            cout << itemIndex << " - " << slot.item.getName()
                 << " (x" << slot.quantity << ")"
                 << " | HP +" << slot.item.getHealthRestore()
                 << " | MP +" << slot.item.getManaRestore() << "\n";
        }

        cout << "-1 - Back\n";

        cin >> index;

        // Returns to the battle menu without consuming an item or ending the turn.
        if (index == -1)
        {
            return false;
        }

        // Checks that the item exists, is available, and would have an effect.
        if (index < 0 || index >= static_cast<int>(slots.size()))
        {
            cout << "\nInvalid item!\n";
        }
        else if (!attacker.getInventory().hasAvailableItem(index))
        {
            cout << "\nThat item is out of stock!\n";
        }
        else if (!attacker.canUseItem(index))
        {
            cout << "\nThat item would have no effect right now!\n";
        }
    }
    while (index < 0 || index >= static_cast<int>(slots.size()) ||
           !attacker.getInventory().hasAvailableItem(index) ||
           !attacker.canUseItem(index));

    // Stores the name before consuming one unit of the item.
    const string itemName = slots[index].item.getName();
    attacker.useItem(index);

    // Shows the attributes after applying the item's effect.
    cout << "\n" << attacker.getName() << " used " << itemName << "!\n"
         << "HP: " << attacker.getHealth() << " / " << attacker.getMaxHealth()
         << " | MP: " << attacker.getMana() << " / " << attacker.getMaxMana()
         << "\n";

    return true;
}

string ItemAction::getName() const
{
    return "Item";
}
