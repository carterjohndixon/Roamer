#include "Game.hpp"
#include "TextureManager.hpp"
#include "Map.hpp"
#include "ECS/Components.hpp"
#include "Vector2D.hpp"
#include "AssetManager.hpp"
#include <sstream>

Map *map;
Manager manager;

SDL_Renderer *Game::renderer = nullptr;
SDL_Event Game::event;

SDL_Rect Game::camera = {0, 0, 800, 640};

AssetManager *Game::assets = new AssetManager(&manager);

bool Game::isRunning = false;

auto &player(manager.addEntity());
auto &label(manager.addEntity());
auto &health(manager.addEntity());

std::vector<std::string> inventory{"Sword", "Potion", "Metal"};

Game::Game()
{
}

Game::~Game()
{
}

void Game::init(const char *title, int width, int height, bool fullscreen)
{
    int flags = 0;

    if (fullscreen)
        flags = SDL_WINDOW_FULLSCREEN;

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
        renderer = SDL_CreateRenderer(window, 0, -1);

        if (renderer)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }
        isRunning = true;
    }

    if (TTF_Init() == -1)
        std::cout << "Error: SDL_TTF" << std::endl;

    assets->AddTexture("terrain", "assets/terrain_ss.png");
    assets->AddTexture("player", "assets/player_anims.png");
    assets->AddTexture("playerSword", "assets/playerSwordAnims.png");
    assets->AddTexture("playerBigPotion", "assets/playerBigPotionAnim.png");
    assets->AddTexture("playerSmallPotion", "assets/playerSmallPotionAnim.png");
    assets->AddTexture("inventoryBox", "assets/inventoryBox.png");
    assets->AddTexture("sword", "assets/sword.png");
    assets->AddTexture("bigPotion", "assets/bigPotion.png");
    assets->AddTexture("smallPotion", "assets/smallPotion.png");

    assets->AddFont("arial", "assets/arial.ttf", 16);

    map = new Map("terrain", 3, 32);

    map->LoadMap("assets/map.map", 25, 20);

    player.addComponent<TransformComponent>(800.0f, 640.0f, 32, 32, 4);
    player.addComponent<SpriteComponent>("player", true);
    player.addComponent<KeyboardController>();
    player.addGroup(groupPlayers);

    SDL_Color white = {225,
                       225,
                       225,
                       225};

    // label.addComponent<InventoryComponent>(10, 10, inventory, "arial", white);
    health.addComponent<HealthComponent>(100, 25, 550, 612, 150, 25);
}

auto &tiles(manager.getGroup(Game::groupMap));
auto &players(manager.getGroup(Game::groupPlayers));

void Game::handleEvents()
{

    SDL_PollEvent(&event);
    switch (event.type)
    {
    case SDL_QUIT:
        isRunning = false;
        break;
    case SDL_MOUSEBUTTONDOWN:
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        if (mouseX >= 10 && mouseX <= 54 && mouseY >= 2 && mouseY <= 46)
        {
            player.getComponent<SpriteComponent>().UpdateTex("playerSword", true);
        }
        if (mouseX >= 10 && mouseX <= 54 && mouseY >= 55 && mouseY <= 105)
        {
            player.getComponent<SpriteComponent>().UpdateTex("playerBigPotion", true);
        }
        if (mouseX >= 10 && mouseX <= 54 && mouseY >= 106 && mouseY <= 150)
        {
            player.getComponent<SpriteComponent>().UpdateTex("playerSmallPotion", true);
        }
        break;
    case SDL_KEYDOWN:
        switch (event.key.keysym.sym)
        {
        case SDLK_e:
            if (player.getComponent<SpriteComponent>().getTex() == "playerSmallPotion")
            {
                health.getComponent<HealthComponent>().updateHealthSmallPotion();
            }
            if (player.getComponent<SpriteComponent>().getTex() == "playerBigPotion")
            {
                health.getComponent<HealthComponent>().updateHealthBigPotion();
            }
            break;
        }
        break;
    default:
        break;
    }
}

void Game::update()
{
    Vector2D playerPos = player.getComponent<TransformComponent>().position;

    // int mouseX, mouseY;
    // SDL_GetMouseState(&mouseX, &mouseY);
    // std::cout << "X: " << mouseX << " Y: " << mouseY << " (" << mouseX << "," << mouseY << ")" << std::endl;

    // label.getComponent<InventoryComponent>().SetLabelVector(inventory, "arial");

    manager.refresh();
    manager.update();

    camera.x = static_cast<int>(player.getComponent<TransformComponent>().position.x - 400);
    camera.y = static_cast<int>(player.getComponent<TransformComponent>().position.y - 320);

    if (camera.x < 0)
        camera.x = 0;
    if (camera.y < 0)
        camera.y = 0;
    if (camera.x > camera.w)
        camera.x = camera.w;
    if (camera.y > camera.h)
        camera.y = camera.h;
    if (playerPos.y <= topCol)
        player.getComponent<TransformComponent>().position.y = topCol + 1;
    if (playerPos.x <= rsCol)
        player.getComponent<TransformComponent>().position.x = rsCol + 1;
    if (playerPos.y >= btCol)
        player.getComponent<TransformComponent>().position.y = btCol - 1;
    if (playerPos.x >= lsCol)
        player.getComponent<TransformComponent>().position.x = lsCol - 1;
}

void Game::render()
{
    SDL_RenderClear(renderer);
    for (auto &t : tiles)
        t->draw();

    for (auto &p : players)
        p->draw();

    assets->DrawTexture("inventoryBox", invenSrcR, 15, 10, 30.f, 160.f, 1.5f);
    assets->DrawTexture("sword", swordSrcR, 20, 12, 32, 32, 1.2);
    assets->DrawTexture("bigPotion", swordSrcR, 18, 60, 32, 32, 1.3f);
    assets->DrawTexture("smallPotion", swordSrcR, 10, 98, 32, 32, 2.f);
    health.getComponent<HealthComponent>().draw(renderer);

    label.draw();

    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

// Don't allow for the user's health bar to go over 100 HP
// Label the current amount of HP the user has