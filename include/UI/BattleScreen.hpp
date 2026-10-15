#ifndef BATTLE_SCREEN_HPP
#define BATTLE_SCREEN_HPP

#include "Renderer.hpp"
#include "Button.hpp"
#include <SDL3/SDL.h>
#include <string>
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

    bool shouldClose() const;

private:

    void drawBackground();
    void drawHeroPanel();
    void drawEnemyPanel();
    void drawActionSlots();
    void drawBottom();
    void updateSkillButtons();
    void updateItemButtons();
    void drawBattleLog();
    void drawTurnIndicator();
    void drawBattleEffect();
    void performHeroSkill(std::size_t skillIndex);
    void performHeroItem(std::size_t itemIndex);
    void performHeroDefense();
    void performEnemyTurn();
    void restartBattle();
    void addBattleLog(const std::string& message);
    void startEffect(int effect);

    Renderer& renderer;

    Battle& battle;
    Character& hero;
    Character& enemy;
    std::string battleMessage;
    std::vector<std::string> battleLog;

    enum class VisualEffect
    {
        NONE,
        HERO_HIT,
        ENEMY_HIT,
        HERO_HEAL,
        HERO_ITEM,
        HERO_DEFEND,
        ENEMY_DEFEND
    };

    VisualEffect activeEffect = VisualEffect::NONE;
    Uint64 effectEndsAt = 0;

    HealthBar heroHealthBar;
    HealthBar heroManaBar;

    HealthBar enemyHealthBar;
    HealthBar enemyManaBar;

//Buttons for the action slots.

    Button skillButton;

    //Skillbuttons

    std::vector<Button> skillButtons;
    std::vector<Button> itemButtons;

    Button defendButton;
    Button itemButton;
    Button backButton;
    Button continueButton;
    Button restartButton;
    Button exitButton;
    

    enum class BattleScreenState{
        ACTIONS,
        SKILLS,
        ITEMS,
        ENEMY_TURN,
        FINISHED
    };

    BattleScreenState state = BattleScreenState::ACTIONS;

    bool closeRequested = false;
};

#endif
