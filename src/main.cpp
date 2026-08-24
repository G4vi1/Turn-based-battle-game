#include "UI/Renderer.hpp"
#include "UI/BattleScreen.hpp"

#include <SDL3/SDL.h>
#include <iostream>

using namespace std;

int main()
{
    // Creates the renderer responsible for
    // communicating with SDL.

    Renderer renderer;

    // Initializes SDL, the window and the renderer.

    if (!renderer.initialize())
    {
        cout << "Failed to initialize SDL.\n";
        return 1;
    }


    // Creates the hero skills.

    Skill holyStrike(
        "Holy Strike",
        SkillType::Physical,
        10,
        25.0
    );

    Skill divineSpear(
        "Divine Spear",
        SkillType::Physical,
        15,
        35.0
    );

    Skill healingBlessing(
        "Healing Blessing",
        SkillType::Special,
        20,
        15.0
    );

    // Creates the villain skills.

    Skill shadowClaw(
        "Shadow Claw",
        SkillType::Physical,
        8,
        20.0
    );

    Skill arcaneBlast(
        "Arcane Blast",
        SkillType::Magical,
        25,
        45.0
    );

    Skill devastatingRoar(
        "Devastating Roar",
        SkillType::Special,
        30,
        50.0
    );

    //Creates the hero and the villain characters.

    Character hero(
        "Archangel",
        100.0,
        0.8,
        12,
        60
    );

    Character enemy(
        "Leviathan",
        100.0,
        0.4,
        15,
        70
    );

    // --------------------------------------------------
    // Add hero skills
    // --------------------------------------------------

    hero.addSkill(holyStrike);
    hero.addSkill(divineSpear);
    hero.addSkill(healingBlessing);

    // --------------------------------------------------
    // Add enemy skills
    // --------------------------------------------------

    enemy.addSkill(shadowClaw);
    enemy.addSkill(arcaneBlast);
    enemy.addSkill(devastatingRoar);

    // Creates the battle screen.
    // It will be responsible for drawing
    // the entire battle interface.

    BattleScreen battleScreen(
        renderer,
        hero,
        enemy
    );

    // Main application loop control.

    bool running = true;

    SDL_Event event;

    // Main loop.

    while (running)
    {
        // Processes every SDL event generated
        // since the previous frame.

        while (SDL_PollEvent(&event)){
        
            if (event.type == SDL_EVENT_QUIT){
                running = false;
            }

            battleScreen.handleEvent(event);
        }

        // Draws the entire battle screen.

        battleScreen.draw();
    }

    // Releases SDL resources.

    renderer.shutdown();

    return 0;
}