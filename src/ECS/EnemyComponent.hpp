#pragma once

#include <map>
#include <SDL2/SDL.h>

#include "Components.hpp"
#include "../TextureManager.hpp"
#include "Animation.hpp"
#include "../AssetManager.hpp"

class EnemyComponent : public Component
{
private:
    SDL_Texture *texture;
    SDL_Rect srcRect, destRect;

    std::string curTex;

    bool animated = false;
    int frames = 0;
    int speed = 100;

    int xpos, ypos, scale;

public:
    int animIndex = 0;
    std::map<const char *, Animation> animations;

    SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

    EnemyComponent() = default;
    EnemyComponent(std::string id)
    {
        curTex = id;
        setTex(id);
    }

    EnemyComponent(std::string id, int i_xpos, int i_ypos, int i_scale, bool isAnimated)
    {
        animated = isAnimated;

        this->xpos = i_xpos;
        this->ypos = i_ypos;
        this->scale = i_scale;

        Animation plantEnemySpawn = Animation(0, 5, 600);

        animations.emplace("plantEnemySpawn", plantEnemySpawn);

        Play("plantEnemySpawn");

        curTex = id;

        setTex(id);
    }

    ~EnemyComponent()
    {
    }

    void drawEnemy(const char *imagePath, int scale)
    {
        SDL_Surface *imageSurface = IMG_Load(imagePath);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(Game::renderer, imageSurface);
        SDL_FreeSurface(imageSurface);

        SDL_QueryTexture(texture, NULL, NULL, &srcRect.w, &srcRect.h);
        srcRect.w /= 8;
        srcRect.x = 0;
        srcRect.y = 0;

        int screenW, screenH;
        SDL_GetRendererOutputSize(Game::renderer, &screenW, &screenH);

        int x_i = 0;
        int y_i = 0;

        if (destRect.w < screenW && destRect.h < screenH)
        {
            x_i = rand() % (2251);
            y_i = rand() % (2401);
        }
        else
        {
            x_i = rand() % (2251);
            y_i = rand() % (2401);
        }

        std::cout << x_i << " " << y_i << std::endl;

        destRect.x = x_i;
        destRect.y = y_i;
        destRect.h = srcRect.h * scale;
        destRect.w = srcRect.w * scale;

        int speed = 400;
        int frameIndex = ((SDL_GetTicks() / speed) % 6);
        srcRect.x = frameIndex * srcRect.w;

        SDL_RenderCopy(Game::renderer, texture, &srcRect, &destRect);

        SDL_DestroyTexture(texture);
    }

    void setTex(std::string id)
    {
        texture = Game::assets->GetTexture(id);
    }

    void update() override
    {
        if (animated)
            srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);

        destRect.x = static_cast<int>(xpos) - Game::camera.x;
        destRect.y = static_cast<int>(ypos) - Game::camera.y;
        destRect.w = destRect.w * scale;
        destRect.h = destRect.h * scale;

        srcRect.y = animIndex * destRect.h;
    }

    void draw() override
    {
        SDL_Rect mapDestRect = {destRect.x - Game::camera.x, destRect.y - Game::camera.y, destRect.w * scale, destRect.h * scale};
        TextureManager::Draw(texture, srcRect, mapDestRect, SDL_FLIP_NONE);
    }

    void Play(const char *animName)
    {
        frames = animations[animName].frames;
        animIndex = animations[animName].index;
        speed = animations[animName].speed;
    }

    std::string getTex()
    {
        return curTex;
    }
};