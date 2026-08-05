#ifndef BATTLE_SCREEN_HPP
#define BATTLE_SCREEN_HPP

#include "Renderer.hpp"

class BattleScreen
{
public:

    // Constructor

    BattleScreen(Renderer& renderer);

    // Draws the entire battle screen.

    void draw();

private:

    void drawBackground();

    void drawHeroPanel();

    void drawEnemyPanel();

    void drawActionPanel();

    void drawActionSlots();

    Renderer& renderer;
};

#endif