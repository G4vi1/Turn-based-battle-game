#ifndef BATTLE_SCREEN_HPP
#define BATTLE_SCREEN_HPP

#include "Renderer.hpp"
#include "Button.hpp"
#include <SDL3/SDL.h>
#include <vector>
#include "HealthBar.hpp"
#include "Character.hpp"

class BattleScreen
{
public:

    BattleScreen(
        Renderer& renderer,
        Character& hero,
        Character& enemy
    );

    void draw();

    void handleEvent(const SDL_Event& event);

private:

    void drawBackground();
    void drawHeroPanel();
    void drawEnemyPanel();
    void drawActionPanel();
    void drawActionSlots();
    void drawBottom();
    void updateSkillButtons();

    HealthBar heroHealthBar;
    HealthBar heroManaBar;

    HealthBar enemyHealthBar;
    HealthBar enemyManaBar;

    Renderer& renderer;

    Character& hero;
    Character& enemy;

//Buttons for the action slots.

    Button skillButton;

        //Skillbuttons

    std::vector<Button> skillButtons;

    Button defendButton;
    Button itemButton;
    Button backButton;
    

    enum class BattleScreenState{
        ACTIONS,
        SKILLS
    };

    BattleScreenState state = BattleScreenState::ACTIONS;

    void drawSkillPanel();
};

#endif
