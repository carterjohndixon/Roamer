#include "Game.hpp"
#include "TextureManager.hpp"
#include "Map.hpp"
#include "ECS/Components.hpp"
#include "Vector2D.hpp"
#include "AssetManager.hpp"
#include <sstream>

Map *map;
// Enemy *enemy;
Manager manager;

SDL_Renderer *Game::renderer = nullptr;
SDL_Event Game::event;

SDL_Rect Game::camera = {0, 0, 2250, 2400};

AssetManager *Game::assets = new AssetManager(&manager);

bool Game::isRunning = false;

auto &player(manager.addEntity());
auto &label(manager.addEntity());
auto &health(manager.addEntity());
auto &plantEnemy(manager.addEntity());

// int Game::curHP{};

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
    assets->AddTexture("inventoryBox", "assets/inventoryBox.png");
    assets->AddTexture("sword", "assets/sword.png");
    // assets->AddTexture("plantMonsterSpawn", "assets/plantMonsterSpawnAnim.png");
    // assets->AddTexture("plantMonster", "assets/plantMonster.png");

    assets->AddFont("arial", "assets/arial.ttf", 16);

    map = new Map("terrain", 3, 32);

    // map->LoadMap("assets/map.map", 25, 20);
    map->LoadMap("assets/map2.map", 32, 32);

    player.addComponent<TransformComponent>(800.0f, 640.0f, 32, 32, 4);
    player.addComponent<SpriteComponent>("player", true);
    player.addComponent<KeyboardController>();
    player.addGroup(groupPlayers);

    health.addComponent<HealthComponent>(100, 100, 635, 10, 150, 25);

    plantEnemy.addComponent<EnemyComponent>("plantMonster", 260, 216, 5);
    // plantEnemy.getComponent<EnemyComponent>().addPlantEnemy("plantMonster", 260, 216);

    // SDL_Color white = {225, 225, 225, 225};
    // curHP = health.getComponent<HealthComponent>().getHealth();
    // label.addComponent<InventoryComponent>(690, 33, std::to_string(curHP), "arial", white);
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
        if (mouseX >= 15 && mouseX <= 58 && mouseY >= 15 && mouseY <= 50)
        {
            assets->AddTexture("playerSword", "assets/playerSwordAnims.png");
            player.getComponent<SpriteComponent>().UpdateTex("playerSword", true);
        }
        if (mouseX >= 67 && mouseX <= 105 && mouseY >= 15 && mouseY <= 50 && assets->FindTexture("bigPotion"))
        {
            assets->AddTexture("playerBigPotion", "assets/playerBigPotionAnim.png");
            player.getComponent<SpriteComponent>().UpdateTex("playerBigPotion", true);
        }
        if (mouseX >= 115 && mouseX <= 155 && mouseY >= 15 && mouseY <= 50 && assets->FindTexture("smallPotion"))
        {
            assets->AddTexture("playerSmallPotion", "assets/playerSmallPotionAnim.png");
            player.getComponent<SpriteComponent>().UpdateTex("playerSmallPotion", true);
        }
        if (player.getComponent<SpriteComponent>().getTex() == "playerSword")
        {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            std::cout << "Mouse X: " << mouseX << " PlayerX: " << player.getComponent<TransformComponent>().position.x << std::endl;
            if (mouseX < player.getComponent<TransformComponent>().position.x || mouseX == player.getComponent<TransformComponent>().position.x)
                player.getComponent<SpriteComponent>().swordSwing("idleSwingLeft", true);
            if (mouseX > player.getComponent<TransformComponent>().position.x)
                player.getComponent<SpriteComponent>().swordSwing("idleSwingRight", true);
            assets->AddTexture("playerSwordSwing", "assets/playerSwordSwingAnim.png");
            player.getComponent<SpriteComponent>().swordSwing("playerSwordSwing", true);
        }
        break;

    case SDL_MOUSEBUTTONUP:
        if (player.getComponent<SpriteComponent>().getTex() == "playerSwordSwing")
        {
            player.getComponent<SpriteComponent>().UpdateTex("playerSword", true);
        }
        break;

    case SDL_KEYDOWN:
        switch (event.key.keysym.sym)
        {
        case SDLK_e:
            if (player.getComponent<SpriteComponent>().getTex() == "playerSmallPotion" && health.getComponent<HealthComponent>().getHealth() < 50)
            {
                assets->DeleteTexture("smallPotion");
                health.getComponent<HealthComponent>().updateHealthSmallPotion();
                player.getComponent<SpriteComponent>().UpdateTex("player", true);
            }
            if (player.getComponent<SpriteComponent>().getTex() == "playerBigPotion" && health.getComponent<HealthComponent>().getHealth() != 100)
            {
                assets->DeleteTexture("bigPotion");
                if (health.getComponent<HealthComponent>().getHealth() == 75)
                {
                    health.getComponent<HealthComponent>().setCurrentHealth(100);
                    return;
                }
                health.getComponent<HealthComponent>().updateHealthBigPotion();
                player.getComponent<SpriteComponent>().UpdateTex("player", true);
            }
            break;
            // case SDLK_p:
            // assets->AddTexture("bigPotion", "assets/bigPotion.png");
            // assets->AddTexture("smallPotion", "assets/smallPotion.png");
            // assets->AddTexture("plantMonsterSpawn", "assets/plantMonsterSpawnAnim.png");
            // assets->AddTexture("plantMonster", "assets/plantMonster.png");
            // plantEnemy.addComponent<SpriteComponent>() break;
        }
    default:
        break;
    }
}

void Game::update()
{
    Vector2D playerPos = player.getComponent<TransformComponent>().position;
    // std::cout << "X: " << playerPos.x << " Y: " << playerPos.y << " (" << playerPos.x << "," << playerPos.y << ")" << std::endl;

    // std::cout << "X: " << mouseX << " Y: " << mouseY << " (" << mouseX << "," << mouseY << ")" << std::endl;

    // curHP = health.getComponent<HealthComponent>().getHealth();
    // label.getComponent<InventoryComponent>().SetLabelText(std::to_string(curHP), "arial");

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
    if (playerPos.x <= lsCol)
        player.getComponent<TransformComponent>().position.x = lsCol + 1;
    if (playerPos.y >= btCol)
        player.getComponent<TransformComponent>().position.y = btCol - 1;
    if (playerPos.x >= rsCol)
        player.getComponent<TransformComponent>().position.x = rsCol - 1;
}

void Game::render()
{
    SDL_RenderClear(renderer);
    for (auto &t : tiles)
        t->draw();

    plantEnemy.getComponent<EnemyComponent>().draw();

    for (auto &p : players)
        p->draw();

    // for (auto &e : enemies)
    //     e->draw();

    assets->DrawTexture("inventoryBox", invenSrcR, 15, 10, 160.f, 30.f, 1.5f);
    assets->DrawTexture("sword", swordSrcR, 20, 12, 32, 32, 1.2);
    assets->DrawTexture("bigPotion", swordSrcR, 70, 12, 32, 32, 1.3f);
    assets->DrawTexture("smallPotion", swordSrcR, 110, 3, 32, 32, 1.8f);
    health.getComponent<HealthComponent>().draw();

    label.draw();

    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}