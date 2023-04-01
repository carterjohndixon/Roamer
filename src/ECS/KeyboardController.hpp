#pragma once

#include "../Game.hpp"
#include "ECS.hpp"
#include "Components.hpp"

class KeyboardController : public Component
{
public:
    TransformComponent *transfrom;
    SpriteComponent *sprite;

    void init() override
    {
        transfrom = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();
    }

    void update() override
    {
        if (Game::event.type == SDL_KEYDOWN)
        {
            switch (Game::event.key.keysym.sym)
            {
            case SDLK_w:
                transfrom->velocity.y = -1;
                sprite->Play("Walk");
                break;
            case SDLK_a:
                transfrom->velocity.x = -1;
                sprite->Play("Walk");
                sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
                break;
            case SDLK_d:
                transfrom->velocity.x = 1;
                sprite->Play("Walk");
                break;
            case SDLK_s:
                transfrom->velocity.y = 1;
                sprite->Play("Walk");
                break;
            default:
                break;
            }
        }

        if (Game::event.type == SDL_KEYUP)
        {
            switch (Game::event.key.keysym.sym)
            {
            case SDLK_w:
                transfrom->velocity.y = 0;
                sprite->Play("Idle");
                break;
            case SDLK_a:
                transfrom->velocity.x = 0;
                sprite->Play("Idle");
                sprite->spriteFlip = SDL_FLIP_NONE;
                break;
            case SDLK_d:
                transfrom->velocity.x = 0;
                sprite->Play("Idle");
                break;
            case SDLK_s:
                transfrom->velocity.y = 0;
                sprite->Play("Idle");
                break;
            default:
                break;
            }
        }
    }
};