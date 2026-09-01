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


      // Main actions

      skillButton(300, 580, 200, 80, "SKILL"),
        itemButton(540, 580, 200, 80, "ITEM"),
defendButton(780, 580, 200, 80, "DEFEND"),

      // Skills

      skill1Button(500, 540, 300, 55, "SKILL 1"),
      skill2Button(820, 540, 300, 55, "SKILL 2"),
      skill3Button(500, 605, 300, 55, "SKILL 3"),
      skill4Button(820, 605, 300, 55, "SKILL 4"), 

      backButton(300, 595, 150, 50, "BACK"),

      heroHealthBar(70, 615, 200, 15, hero.getMaxHealth(), {40, 180, 70, 255}),
      heroManaBar(70, 660, 170, 10, hero.getMaxMana(), {150, 50, 200, 255}),

      enemyHealthBar(460, 138, 360, 15, enemy.getMaxHealth(), {40, 180, 70, 255}),
      enemyManaBar(480, 180, 320, 10, enemy.getMaxMana(), {150, 50, 200, 255})
{
}


void BattleScreen::draw(){
    renderer.clear();

    drawBackground();

    heroHealthBar.setHealth(hero.getHealth());
    heroManaBar.setHealth(hero.getMana());

    enemyHealthBar.setHealth(enemy.getHealth());
    enemyManaBar.setHealth(enemy.getMana());

    // Enemy status at the top center
    drawEnemyPanel();

    if (state == BattleScreenState::ACTIONS)
    {
        // Bottom panel first
        drawActionPanel();

        // Hero status over the panel
        drawHeroPanel();

        // Buttons over the panel
        drawActionSlots();
    }
    else if (state == BattleScreenState::SKILLS){

        skill1Button.setText(hero.getSkills()[0].getName());
        skill2Button.setText(hero.getSkills()[1].getName());
        skill3Button.setText(hero.getSkills()[2].getName());
        skill4Button.setText(hero.getSkills()[3].getName());

        drawSkillPanel();

        drawBottom();
    }

    renderer.present();
}

// Draws the background of the battle screen.

void BattleScreen::drawBackground()
{
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
        560,
        1200,
        140
    );

    // Name

    renderer.drawText(
        hero.getName().c_str(),
        70,
        555,
        24
    );

    // Health text

    std::string healthText =
        std::to_string(static_cast<int>(hero.getHealth())) +
        " / " +
        std::to_string(static_cast<int>(hero.getMaxHealth()));

    renderer.drawText(
        healthText.c_str(),
        70,
        578,
        7
    );

    heroHealthBar.draw(renderer);

    // Mana text

    std::string manaText =
        std::to_string(static_cast<int>(hero.getMana())) +
        " / " +
        std::to_string(static_cast<int>(hero.getMaxMana()));

    renderer.drawText(
        manaText.c_str(),
        70,
        635,
        7
    );

    heroManaBar.draw(renderer);
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
        440,
        50,
        400,
        140
    );

    // Name

    renderer.drawText(
        enemy.getName().c_str(),
        465,
        65,
        26
    );

    // Health text

    std::string healthText =
        std::to_string(static_cast<int>(enemy.getHealth())) +
        " / " +
        std::to_string(static_cast<int>(enemy.getMaxHealth()));

    renderer.drawText(
        healthText.c_str(),
        465,
        98,
        7
    );

    enemyHealthBar.draw(renderer);

    // Mana text

    std::string manaText =
        std::to_string(static_cast<int>(enemy.getMana())) +
        " / " +
        std::to_string(static_cast<int>(enemy.getMaxMana()));

    renderer.drawText(
        manaText.c_str(),
        480,
        153,
        7
    );

    enemyManaBar.draw(renderer);
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

            backButton.setHovered(
                backButton.isHovered(mouseX, mouseY)
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

            if (backButton.isClicked(event)){
                backButton.setPressed(true);
                state = BattleScreenState::ACTIONS;
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
        backButton.setPressed(false);
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

void BattleScreen::drawBottom()
{
    // Hero status
    drawHeroPanel();

    // Back button
    backButton.draw(renderer);

    // Skill buttons
    skill1Button.draw(renderer);
    skill2Button.draw(renderer);
    skill3Button.draw(renderer);
    skill4Button.draw(renderer);
}