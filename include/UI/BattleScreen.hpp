#ifndef BATTLE_SCREEN_HPP
#define BATTLE_SCREEN_HPP

#include "Renderer.hpp"
#include "Button.hpp"
#include <SDL3/SDL.h>

class BattleScreen
{
public:

    BattleScreen(Renderer& renderer);

    void draw();

    void handleEvent(const SDL_Event& event);

private:

    void drawBackground();
    void drawHeroPanel();
    void drawEnemyPanel();
    void drawActionPanel();
    void drawActionSlots();

    Renderer& renderer;

    Button attackButton;
    Button skillButton;
    Button defendButton;
    Button itemButton;
};

#endif