#include "UI/BattleScreen.hpp"
#include <iostream>
#include <string>

using namespace std;

namespace
{
constexpr float WINDOW_WIDTH = 1280.0f;
constexpr float WINDOW_HEIGHT = 720.0f;
constexpr float PANEL_X = 40.0f;
constexpr float PANEL_WIDTH = 1200.0f;
constexpr float ENEMY_PANEL_Y = 40.0f;
constexpr float ACTION_PANEL_Y = 260.0f;
constexpr float HERO_PANEL_Y = 540.0f;
constexpr float PANEL_HEIGHT = 140.0f;
constexpr float ACTION_BUTTON_Y = 320.0f;
constexpr float SKILL_BUTTON_Y = 300.0f;
constexpr float SKILL_BUTTON_HEIGHT = 55.0f;
}

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

      skillButton(290, ACTION_BUTTON_Y, 200, 70, "SKILL"),
      itemButton(540, ACTION_BUTTON_Y, 200, 70, "ITEM"),
      defendButton(790, ACTION_BUTTON_Y, 200, 70, "DEFEND"),

      backButton(90, 360, 150, 50, "BACK"),

      heroHealthBar(70, 612, 250, 15, hero.getMaxHealth(), {40, 180, 70, 255}),
      heroManaBar(70, 662, 250, 15, hero.getMaxMana(), {150, 50, 200, 255}),

      enemyHealthBar(70, 112, 250, 15, enemy.getMaxHealth(), {40, 180, 70, 255}),
      enemyManaBar(70, 162, 250, 15, enemy.getMaxMana(), {150, 50, 200, 255})
{
    itemButton.setEnabled(false);
    updateSkillButtons();
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
        updateSkillButtons();
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
        WINDOW_WIDTH,
        WINDOW_HEIGHT
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
        PANEL_X,
        HERO_PANEL_Y,
        PANEL_WIDTH,
        PANEL_HEIGHT
    );

    // Name

    renderer.drawText(
        hero.getName().c_str(),
        70,
        552,
        26
    );

    // Health text

    std::string healthText =
        std::to_string(static_cast<int>(hero.getHealth())) +
        " / " +
        std::to_string(static_cast<int>(hero.getMaxHealth()));

    renderer.drawText(
        ("HP " + healthText).c_str(),
        70,
        585,
        18
    );

    heroHealthBar.draw(renderer);

    // Mana text

    std::string manaText =
        std::to_string(static_cast<int>(hero.getMana())) +
        " / " +
        std::to_string(static_cast<int>(hero.getMaxMana()));

    renderer.drawText(
        ("MP " + manaText).c_str(),
        70,
        635,
        18
    );

    heroManaBar.draw(renderer);

    if (hero.isDefendingState())
    {
        renderer.drawText("DEFENDING", 390, 612, 18);
    }
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
        PANEL_X,
        ENEMY_PANEL_Y,
        PANEL_WIDTH,
        PANEL_HEIGHT
    );

    // Name

    renderer.drawText(
        enemy.getName().c_str(),
        70,
        52,
        26
    );

    // Health text

    std::string healthText =
        std::to_string(static_cast<int>(enemy.getHealth())) +
        " / " +
        std::to_string(static_cast<int>(enemy.getMaxHealth()));

    renderer.drawText(
        ("HP " + healthText).c_str(),
        70,
        85,
        18
    );

    enemyHealthBar.draw(renderer);

    // Mana text

    std::string manaText =
        std::to_string(static_cast<int>(enemy.getMana())) +
        " / " +
        std::to_string(static_cast<int>(enemy.getMaxMana()));

    renderer.drawText(
        ("MP " + manaText).c_str(),
        70,
        135,
        18
    );

    enemyManaBar.draw(renderer);

    if (enemy.isDefendingState())
    {
        renderer.drawText("DEFENDING", 390, 112, 18);
    }
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
        PANEL_X,
        ACTION_PANEL_Y,
        PANEL_WIDTH,
        PANEL_HEIGHT
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
            for (Button& skillButton : skillButtons)
            {
                skillButton.setHovered(skillButton.isHovered(mouseX, mouseY));
            }

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
            for (Button& skillButton : skillButtons)
            {
                if (skillButton.isClicked(event))
                {
                    skillButton.setPressed(true);
                }
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
        for (Button& skillButton : skillButtons)
        {
            skillButton.setPressed(false);
        }
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
    renderer.drawText("COMING SOON", 566, 400, 14);

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
        PANEL_X,
        ACTION_PANEL_Y,
        PANEL_WIDTH,
        PANEL_HEIGHT
    );
}

void BattleScreen::drawBottom()
{
    // Hero status
    drawHeroPanel();

    // Back button
    backButton.draw(renderer);

    // Skill buttons
    for (Button& skillButton : skillButtons)
    {
        skillButton.draw(renderer);
    }
}

// Rebuilds the skill controls from the hero's current skill list.

void BattleScreen::updateSkillButtons()
{
    const vector<Skill>& skills = hero.getSkills();
    skillButtons.clear();
    skillButtons.reserve(skills.size());

    for (size_t index = 0; index < skills.size(); ++index)
    {
        const float x = index % 2 == 0 ? 300.0f : 700.0f;
        const float y = SKILL_BUTTON_Y + (index / 2) * (SKILL_BUTTON_HEIGHT + 15.0f);
        const Skill& skill = skills[index];

        skillButtons.emplace_back(
            x,
            y,
            330,
            SKILL_BUTTON_HEIGHT,
            skill.getName() + " - " + to_string(skill.getManaCost()) + " MP"
        );

        skillButtons.back().setEnabled(hero.hasEnoughMana(skill.getManaCost()));
    }
}
