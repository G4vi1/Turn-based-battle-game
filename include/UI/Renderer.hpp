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

        // Clears the screen with a specified color.

        void draw();

    private:

        //Window

        SDL_Window* window;

        //Renderer

        SDL_Renderer* renderer;




};