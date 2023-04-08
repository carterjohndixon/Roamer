#pragma once

#include <string>

#include "Components.hpp"
#include "Animation.hpp"
#include "../Game.hpp"
#include "../Vector2D.hpp"
#include "../TextureManager.hpp"

extern Manager manager;

class EnemyComponent : public Component
{
public:
    SDL_Texture *texture;
    SDL_Rect srcRect, destRect;
    int scale;

    EnemyComponent(std::string id, int xpos, int ypos, int scale)
    {
        this->addPlantEnemy(id, xpos, ypos, scale);
    }

    void addPlantEnemy(std::string id, int xpos, int ypos, int scale)
    {
        this->scale = scale;
        SDL_Surface *enemySurface = IMG_Load("assets/plantMonster.png");
        if (!enemySurface)
        {
            SDL_Log("Failed to load enemy texture: %s", SDL_GetError());
            return;
        }

        auto &enemy(manager.addEntity());
        enemy.addComponent<TransformComponent>(xpos, ypos);
        srcRect = {0, 0, enemySurface->w, enemySurface->h};
        destRect = {xpos, ypos, enemySurface->w, enemySurface->h};
        texture = SDL_CreateTextureFromSurface(Game::renderer, enemySurface);
        // enemy.addComponent<SpriteComponent>(id, true);
        // enemy.addComponent<Colli
        SDL_FreeSurface(enemySurface);
    }

    void draw() override
    {
        SDL_Rect mapDestRect = {destRect.x - Game::camera.x, destRect.y - Game::camera.y, destRect.w * scale, destRect.h * scale};
        TextureManager::Draw(texture, srcRect, mapDestRect, SDL_FLIP_NONE);
    }

    // void AddTile(int srcX, int srcY, int xpos, int ypos)
    // {
    //     auto &tile(manager.addEntity());
    //     tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, Scale, texID);
    //     tile.addGroup(Game::groupMap);
    // }
};

// Make the monster bigger
// Put the monster on the map not on the screen