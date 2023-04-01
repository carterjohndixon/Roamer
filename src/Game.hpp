#pragma once

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <iostream>
#include <vector>

class AssetManager;

class Game
{
public:
    Game();
    ~Game();

    void init(const char *title, int width, int height, bool fullscreen);

    void handleEvents();
    void update();
    bool running() { return isRunning; }
    void render();
    void clean();

    static SDL_Renderer *renderer;
    static SDL_Event event;
    static bool isRunning;
    static SDL_Rect camera;
    static AssetManager *assets;

    enum groupLabels : std::size_t
    {
        groupMap,
        groupPlayers,
    };

private:
    SDL_Window *window;

    int topCol = -29;
    int rsCol = -34;
    int btCol = 1150;
    int lsCol = 1526;

    SDL_Rect invenSrcR = {
        0,
        0,
        30,
        160,
    };

    SDL_Rect swordSrcR = {
        0,
        0,
        32,
        32,
    };
};