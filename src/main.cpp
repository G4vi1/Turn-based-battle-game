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

    // Creates the battle screen.
    // It will be responsible for drawing
    // the entire battle interface.

    BattleScreen battleScreen(renderer);

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