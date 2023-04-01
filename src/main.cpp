#include <SDL2/SDL.h>

#include "Game.hpp"

Game *game = nullptr;

int main()
{
    const int FPS = 60;
    const int FrameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    game = new Game();
    game->init("Roamer", 800, 640, false);

    while (game->running())
    {
        frameStart = SDL_GetTicks();

        game->handleEvents();
        game->update();
        game->render();

        frameTime = SDL_GetTicks() - frameStart;

        if (FrameDelay > frameTime)
        {
            SDL_Delay(FrameDelay - frameTime);
        }
    }

    game->clean();

    return 0;
}