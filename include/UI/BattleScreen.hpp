#ifndef BATTLE_SCREEN_HPP
#define BATTLE_SCREEN_HPP

#include "Renderer.hpp"
#include "Button.hpp"
#include <SDL3/SDL.h>
#include <vector>
#include "HealthBar.hpp"
#include "Battle.hpp"

class BattleScreen
{
public:

    BattleScreen(
        Renderer& renderer,
        Battle& battle
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
    void drawBattleMessage();
    void performHeroSkill(std::size_t skillIndex);
    void performHeroDefense();

    Renderer& renderer;

    Battle& battle;
    Character& hero;
    Character& enemy;
    std::string battleMessage;

    HealthBar heroHealthBar;
    HealthBar heroManaBar;

    HealthBar enemyHealthBar;
    HealthBar enemyManaBar;

//Buttons for the action slots.

    Button skillButton;

        //Skillbuttons

    std::vector<Button> skillButtons;

    Button defendButton;
    Button itemButton;
    Button backButton;
    

    enum class BattleScreenState{
        ACTIONS,
        SKILLS,
        FINISHED
    };

    BattleScreenState state = BattleScreenState::ACTIONS;

    void drawSkillPanel();
};

#endif
