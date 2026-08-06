#include "UI/BattleScreen.hpp"
#include <iostream>

using namespace std;

// Constructor

BattleScreen::BattleScreen(Renderer& renderer)
    : renderer(renderer),

      attackButton(80, 555, 220, 90),

      skillButton(340, 555, 220, 90),

      itemButton(600, 555, 220, 90),

      defendButton(860, 555, 220, 90)
{
}

void BattleScreen::draw()
{
    renderer.clear();

    drawBackground();

    drawHeroPanel();

    drawEnemyPanel();

    drawActionPanel();

    drawActionSlots();

    // Displays the completed frame.

    renderer.present();
}

// Draws the background of the battle screen.

void BattleScreen::drawBackground()
{
    // Dark background.

    renderer.setDrawColor(
        20,
        20,
        20,
        255
    );

    renderer.drawFilledRect(
        0,
        0,
        1280,
        720
    );
}

// Draws the hero panel.

void BattleScreen::drawHeroPanel()
{
    renderer.setDrawColor(
        60,
        60,
        60,
        255
    );

    renderer.drawFilledRect(
        40,
        40,
        420,
        150
    );
}

// Draws the enemy panel.

void BattleScreen::drawEnemyPanel()
{
    renderer.setDrawColor(
        60,
        60,
        60,
        255
    );

    renderer.drawFilledRect(
        820,
        40,
        420,
        150
    );
}

// Draws the action panel.

void BattleScreen::drawActionPanel()
{
    renderer.setDrawColor(
        80,
        80,
        80,
        255
    );

    
    renderer.drawFilledRect(
        40,
        520,
        1200,
        160
    );
}

void BattleScreen::handleEvent(const SDL_Event& event)
{
    if (attackButton.isClicked(event))
    {
        cout << "Attack pressed!\n";
    }

    if (skillButton.isClicked(event))
    {
        cout << "Skill pressed!\n";
    }

    if (itemButton.isClicked(event))
    {
        cout << "Item pressed!\n";
    }

    if (defendButton.isClicked(event))
    {
        cout << "Defend pressed!\n";
    }
}

// Draws the action slots, even for future ones.

void BattleScreen::drawActionSlots()
{
    attackButton.draw(renderer);

    skillButton.draw(renderer);

    itemButton.draw(renderer);

    defendButton.draw(renderer);
}