#pragma once

#include <map>
#include <SDL2/SDL.h>

#include "Components.hpp"
#include "../TextureManager.hpp"
#include "Animation.hpp"
#include "../AssetManager.hpp"

class SpriteComponent : public Component
{
private:
    TransformComponent *transform;
    SDL_Texture *texture;
    SDL_Rect srcRect, destRect;

    std::string curTex;

    bool animated = false;
    int frames = 0;
    int speed = 100;

public:
    int animIndex = 0;
    std::map<const char *, Animation> animations;

    SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

    SpriteComponent() = default;
    SpriteComponent(std::string id)
    {
        curTex = id;
        setTex(id);
    }

    SpriteComponent(std::string id, bool isAnimated)
    {
        animated = isAnimated;

        Animation idle = Animation(0, 3, 100);
        Animation walk = Animation(1, 8, 100);

        animations.emplace("Idle", idle);
        animations.emplace("Walk", walk);

        Play("Idle");

        curTex = id;

        setTex(id);
    }

    ~SpriteComponent()
    {
    }

    void swordSwing(std::string id, bool isAnimated)
    {
        animated = isAnimated;

        Animation idleSwingRight = Animation(0, 2, 150);
        Animation walkSwing = Animation(1, 8, 150);
        Animation idleSwingLeft = Animation(2, 2, 150);

        animations.emplace("idleSwingRight", idleSwingRight);
        animations.emplace("walkSwing", walkSwing);
        animations.emplace("idleSwingLeft", idleSwingLeft);

        if (id == "idleSwingRight")
            Play("idleSwingRight");
        if (id == "idleSwingLeft")
            Play("idleSwingLeft");

        curTex = id;

        setTex(id);
    }

    void setTex(std::string id)
    {
        texture = Game::assets->GetTexture(id);
    }

    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();

        srcRect.x = srcRect.y = 0;
        srcRect.w = transform->width;
        srcRect.h = transform->height;
    }

    void update() override
    {
        if (animated)
            srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);

        srcRect.y = animIndex * transform->height;

        destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
        destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
        destRect.w = transform->width * transform->scale;
        destRect.h = transform->height * transform->scale;
    }

    void draw() override
    {
        TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
    }

    void Play(const char *animName)
    {
        frames = animations[animName].frames;
        animIndex = animations[animName].index;
        speed = animations[animName].speed;
    }

    void UpdateTex(std::string id, bool isAnimated)
    {
        animated = isAnimated;

        Animation idle = Animation(0, 3, 100);
        Animation walk = Animation(1, 8, 100);

        animations.emplace("Idle", idle);
        animations.emplace("Walk", walk);

        Play("Idle");

        curTex = id;

        setTex(id);
    }

    std::string getTex()
    {
        return curTex;
    }
};
