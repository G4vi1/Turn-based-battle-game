#include "UI/BattleScreen.hpp"

// Constructor

BattleScreen::BattleScreen(Renderer& renderer)
    : renderer(renderer) {
}

void BattleScreen::draw()
{
    renderer.clear();

    drawBackground();

    drawHeroPanel();

    drawEnemyPanel();

    drawActionPanel();

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

// Draws the action slots, even for future ones.

void BattleScreen::drawActionSlots()
{

    renderer.setDrawColor(
        140,
        140,
        140,
        255
    );

    // Attack slot

    renderer.drawFilledRect(
        80,
        555,
        220,
        90
    );

    // Skill slot

    renderer.drawFilledRect(
        340,
        555,
        220,
        90
    );

    // Item slot

    renderer.drawFilledRect(
        600,
        555,
        220,
        90
    );

    // Defend slot

    renderer.drawFilledRect(
        860,
        555,
        220,
        90
    );
}
