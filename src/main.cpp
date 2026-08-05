#include "UI/Renderer.hpp"
#include "UI/BattleScreen.hpp"

#include <SDL3/SDL.h>
#include <iostream>

using namespace std;

int main()
{
    Renderer renderer;

    if (!renderer.initialize())
    {
        cout << "Failed to initialize SDL.\n";
        return 1;
    }

    BattleScreen battleScreen(renderer);

    bool running = true;

    SDL_Event event;

    while (running)
    {
        // Process SDL events.
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }

        // Draw the current battle screen.
        battleScreen.draw();
    }

    renderer.shutdown();

    return 0;
}