#ifndef BATTLE_SCREEN_HPP
#define BATTLE_SCREEN_HPP

#include "Renderer.hpp"
#include "Button.hpp"
#include <SDL3/SDL.h>
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
    HealthBar heroHealthBar;
    HealthBar enemyHealthBar;

    Renderer& renderer;

    Character& hero;
    Character& enemy;

//Buttons for the action slots.

    Button skillButton;

        //Skillbuttons

        Button skill1Button;
        Button skill2Button;
        Button skill3Button;
        Button skill4Button;

    Button defendButton;
    Button itemButton;
    

    enum class BattleScreenState{
        ACTIONS,
        SKILLS
    };

    BattleScreenState state = BattleScreenState::ACTIONS;

    void drawSkillPanel();
};

#endif