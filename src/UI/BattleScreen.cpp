#include "UI/BattleScreen.hpp"
#include <iostream>

using namespace std;

// Constructor

BattleScreen::BattleScreen(Renderer& renderer)
    : renderer(renderer),
      attackButton(80, 555, 220, 90, "ATTACK"),
      skillButton(340, 555, 220, 90, "SKILL"),
      itemButton(600, 555, 220, 90, "ITEM"),
      defendButton(860, 555, 220, 90, "DEFEND")
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
    if (event.type == SDL_EVENT_MOUSE_MOTION)
    {
        float mouseX = event.motion.x;
        float mouseY = event.motion.y;

        attackButton.setHovered(
            attackButton.isHovered(mouseX, mouseY)
        );

        skillButton.setHovered(
            skillButton.isHovered(mouseX, mouseY)
        );

        itemButton.setHovered(
            itemButton.isHovered(mouseX, mouseY)
        );

        defendButton.setHovered(
            defendButton.isHovered(mouseX, mouseY)
        );
    }

    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        if (attackButton.isClicked(event))
        {
            attackButton.setPressed(true);
        }

        if (skillButton.isClicked(event))
        {
            skillButton.setPressed(true);
        }

        if (itemButton.isClicked(event))
        {
            itemButton.setPressed(true);
        }

        if (defendButton.isClicked(event))
        {
            defendButton.setPressed(true);
        }
    }

    if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
    {
        attackButton.setPressed(false);
        skillButton.setPressed(false);
        itemButton.setPressed(false);
        defendButton.setPressed(false);
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