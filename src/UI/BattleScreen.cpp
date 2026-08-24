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
      skill1Button(70, 560, 350, 80, "SKILL 1"),
      skill2Button(465, 560, 350, 80, "SKILL 2"),
      skill3Button(860, 560, 350, 80, "SKILL 3"),
      itemButton(500, 555, 280, 90, "ITEM"),
      defendButton(810, 555, 280, 90, "DEFEND"),
      heroHealthBar(75, 120, 350, 25, hero.getMaxHealth()),
      enemyHealthBar(855, 120, 350, 25, enemy.getMaxHealth())
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

        skill1Button.setText(hero.getSkills()[0].getName());
        skill2Button.setText(hero.getSkills()[1].getName());
        skill3Button.setText(hero.getSkills()[2].getName());

        drawSkillPanel();

        skill1Button.draw(renderer);
        skill2Button.draw(renderer);
        skill3Button.draw(renderer);
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
        50,
        50,
        400,
        140
    );

    renderer.drawText(
        hero.getName().c_str(),
        75,
        65,
        30
    );

    heroHealthBar.draw(renderer);

    std::string healthText =
        std::to_string(static_cast<int>(hero.getHealth())) +
        " / " +
        std::to_string(static_cast<int>(hero.getMaxHealth()));

    renderer.drawText(
        healthText.c_str(),
        75,
        160,
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
        830,
        50,
        400,
        140
    );

    renderer.drawText(
        enemy.getName().c_str(),
        855,
        65,
        30
    );

    enemyHealthBar.draw(renderer);

    std::string healthText =
        std::to_string(static_cast<int>(enemy.getHealth())) +
        " / " +
        std::to_string(static_cast<int>(enemy.getMaxHealth()));

    renderer.drawText(
        healthText.c_str(),
        855,
        160,
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
        }
    }

    if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
    {
        skillButton.setPressed(false);
        skill1Button.setPressed(false);
        skill2Button.setPressed(false);
        skill3Button.setPressed(false);
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