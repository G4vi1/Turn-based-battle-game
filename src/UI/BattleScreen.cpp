#include "UI/BattleScreen.hpp"
#include <iostream>
#include <string>
#include "Actions/AttackAction.hpp"

using namespace std;

// Constructor

BattleScreen::BattleScreen(
    Renderer& renderer,
    Character& hero,
    Character& enemy
)
    : renderer(renderer),
      hero(hero),
      enemy(enemy),
      skillButton(190, 555, 280, 90, "SKILL"),
      skill1Button(80, 545, 250, 70, "SKILL 1"),
      skill2Button(345, 545, 250, 70, "SKILL 2"),
      skill3Button(610, 545, 250, 70, "SKILL 3"),
      skill4Button(875, 545, 250, 70, "SKILL 4"),
      itemButton(500, 555, 280, 90, "ITEM"),
      defendButton(810, 555, 280, 90, "DEFEND"),
      heroHealthBar(70, 130, 360, 25, hero.getMaxHealth()),
      enemyHealthBar(850, 130, 360, 25, enemy.getMaxHealth())
{
}


void BattleScreen::draw()
{
    renderer.clear();

    drawBackground();

    heroHealthBar.setHealth(hero.getHealth());
    enemyHealthBar.setHealth(enemy.getHealth());

    drawHeroPanel();

    drawEnemyPanel();

    if (state == BattleScreenState::ACTIONS)
    {
        drawActionPanel();
        drawActionSlots();
    }
    else if (state == BattleScreenState::SKILLS){
        drawSkillPanel();
        skill1Button.draw(renderer);
        skill2Button.draw(renderer);
        skill3Button.draw(renderer);
        skill4Button.draw(renderer);
    }

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

    renderer.drawText(
        "HERO",
        70,
        60,
        28
    );

    heroHealthBar.draw(renderer);

    std::string healthText =
        std::to_string(static_cast<int>(hero.getHealth())) +
        " / " +
        std::to_string(static_cast<int>(hero.getMaxHealth()));

    renderer.drawText(
        healthText.c_str(),
        70,
        165,
        20
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

    renderer.drawText(
        "ENEMY",
        850,
        60,
        28
    );

    enemyHealthBar.draw(renderer);

    std::string healthText =
        std::to_string(static_cast<int>(enemy.getHealth())) +
        " / " +
        std::to_string(static_cast<int>(enemy.getMaxHealth()));

    renderer.drawText(
        healthText.c_str(),
        850,
        165,
        20
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

        if (state == BattleScreenState::ACTIONS)
        {
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
        else if (state == BattleScreenState::SKILLS)
        {
            skill1Button.setHovered(
                skill1Button.isHovered(mouseX, mouseY)
            );

            skill2Button.setHovered(
                skill2Button.isHovered(mouseX, mouseY)
            );

            skill3Button.setHovered(
                skill3Button.isHovered(mouseX, mouseY)
            );

            skill4Button.setHovered(
                skill4Button.isHovered(mouseX, mouseY)
            );
        }
    }

    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        if (state == BattleScreenState::ACTIONS)
        {
            if (skillButton.isClicked(event))
            {
                skillButton.setPressed(true);
                state = BattleScreenState::SKILLS;
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
        else if (state == BattleScreenState::SKILLS)
        {
            if (skill1Button.isClicked(event))
            {
                skill1Button.setPressed(true);
            }

            if (skill2Button.isClicked(event))
            {
                skill2Button.setPressed(true);
            }

            if (skill3Button.isClicked(event))
            {
                skill3Button.setPressed(true);
            }

            if (skill4Button.isClicked(event))
            {
                skill4Button.setPressed(true);
            }
        }
    }

    if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
    {
        skillButton.setPressed(false);
        skill1Button.setPressed(false);
        skill2Button.setPressed(false);
        skill3Button.setPressed(false);
        skill4Button.setPressed(false);
        itemButton.setPressed(false);
        defendButton.setPressed(false);
    }
}

// Draws the action slots, even for future ones.

void BattleScreen::drawActionSlots()
{

    skillButton.draw(renderer);

    itemButton.draw(renderer);

    defendButton.draw(renderer);
}

// Draws the skill panel, which will be displayed
// when the player clicks the "SKILL" button.

void BattleScreen::drawSkillPanel()
{
    renderer.setDrawColor(
        70,
        70,
        70,
        255
    );

    renderer.drawFilledRect(
        40,
        520,
        1200,
        160
    );
}