#pragma once

#include <SDL3/SDL.h>

class Renderer {

    public:

        // Constructor

        Renderer();


        // Destructor

        ~Renderer();

        // Initializes the SDL renderer.

        bool initialize();

        //Responsible for receiving and handling SDL events, such as user input or window events.

        void handleEvents();

        // Clears the screen with the specified color.

        void clear();

        // Presents the rendered frame to the window.

        void present();

        void setDrawColor(Uint8 r,
                      Uint8 g,
                      Uint8 b,
                      Uint8 a);

        void drawFilledRect(float x,
                        float y,
                        float width,
                        float height);

        void drawRect(
            float x,
            float y,
            float width,
            float height
        );


        // Shuts down the SDL renderer and cleans up resources.
        
        void shutdown();

    private:

        //Window

        SDL_Window* window;

        //Renderer

        SDL_Renderer* renderer;




};