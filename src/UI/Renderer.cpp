#include "Renderer.hpp"

bool Renderer::initialize()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        return false;
    }

    if (!TTF_Init())
    {
        SDL_Quit();
        return false;
    }

    window = SDL_CreateWindow(
        "Turn Based Battle Game",
        1280,
        720,
        0
    );

    if (window == nullptr)
    {
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, nullptr);

    if (renderer == nullptr)
    {
        SDL_DestroyWindow(window);
        window = nullptr;

        TTF_Quit();
        SDL_Quit();
        return false;
    }

    return true;
}

void Renderer::shutdown()
{
    if (font != nullptr)
    {
        TTF_CloseFont(font);
        font = nullptr;
    }

    if (renderer != nullptr)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window != nullptr)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    TTF_Quit();
    SDL_Quit();
}

void Renderer::clear()
{
    SDL_RenderClear(renderer);
}

void Renderer::present()
{
    SDL_RenderPresent(renderer);
}

void Renderer::setDrawColor(
    Uint8 r,
    Uint8 g,
    Uint8 b,
    Uint8 a
)
{
    SDL_SetRenderDrawColor(
        renderer,
        r,
        g,
        b,
        a
    );
}

void Renderer::drawFilledRect(
    float x,
    float y,
    float width,
    float height
)
{
    SDL_FRect rect{
        x,
        y,
        width,
        height
    };

    SDL_RenderFillRect(
        renderer,
        &rect
    );
}

void Renderer::drawRect(
    float x,
    float y,
    float width,
    float height
)
{
    SDL_FRect rect{
        x,
        y,
        width,
        height
    };

    SDL_RenderRect(
        renderer,
        &rect
    );
}

void Renderer::drawText(
    const char* text,
    float x,
    float y,
    float size
)
{
    if (font == nullptr)
    {
        return;
    }

    SDL_Color color{
        255,
        255,
        255,
        255
    };

    SDL_Surface* surface =
        TTF_RenderText_Blended(
            font,
            text,
            0,
            color
        );

    if (surface == nullptr)
    {
        return;
    }

    SDL_Texture* texture =
        SDL_CreateTextureFromSurface(
            renderer,
            surface
        );

    if (texture == nullptr)
    {
        SDL_DestroySurface(surface);
        return;
    }

    SDL_FRect destination{
        x,
        y,
        static_cast<float>(surface->w),
        static_cast<float>(surface->h)
    };

    SDL_RenderTexture(
        renderer,
        texture,
        nullptr,
        &destination
    );

    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
}