#pragma once

#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>

#include "Components.hpp"
#include "../AssetManager.hpp"

class InventoryComponent : public Component
{
public:
    InventoryComponent(int xpos, int ypos, std::string text, std::string font, SDL_Color &color)
        : labelText(text), labelFont(font), textColor(color)
    {
        position.x = xpos;
        position.y = ypos;

        SetLabelText(labelText, labelFont);
    }

    InventoryComponent(int xpos, int ypos, std::vector<std::string> textVec, std::string font, SDL_Color &color)
        : inventory(textVec), labelFont(font), textColor(color)
    {
        position.x = xpos;
        position.y = ypos;

        SetLabelText(labelText, labelFont);
    }

    ~InventoryComponent() {}

    void SetLabelText(std::string text, std::string font)
    {
        SDL_Surface *surf = TTF_RenderText_Blended(Game::assets->GetFont(font), text.c_str(), textColor);
        labelTexture = SDL_CreateTextureFromSurface(Game::renderer, surf);
        SDL_FreeSurface(surf);

        SDL_QueryTexture(labelTexture, nullptr, nullptr, &position.w, &position.h);
    }

    void SetLabelVector(std::vector<std::string> textV, std::string font)
    {
        SDL_Color textColor = {255, 255, 255, 0};
        int totalHeight = 0;
        int maxWidth = 0;

        for (const auto &text : textV)
        {
            SDL_Surface *temp = TTF_RenderText_Blended(Game::assets->GetFont(font), text.c_str(), textColor);
            if (temp == nullptr)
            {
                SDL_Log("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
                return;
            }

            totalHeight += temp->h;
            maxWidth = std::max(maxWidth, temp->w);

            SDL_FreeSurface(temp);
        }

        SDL_Surface *surf = SDL_CreateRGBSurface(0, maxWidth, totalHeight, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
        if (surf == nullptr)
        {
            SDL_Log("Unable to create surface for text! SDL Error: %s\n", SDL_GetError());
            return;
        }

        SDL_SetSurfaceBlendMode(surf, SDL_BLENDMODE_BLEND);

        SDL_Rect destRect = {0, 0, 0, 0};
        for (const auto &text : textV)
        {
            SDL_Surface *temp = TTF_RenderText_Blended(Game::assets->GetFont(font), text.c_str(), textColor);
            if (temp == nullptr)
            {
                SDL_Log("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
                return;
            }

            SDL_Rect rect = {0, destRect.y, temp->w, temp->h};
            SDL_BlitSurface(temp, nullptr, surf, &rect);
            destRect.y += temp->h;

            SDL_FreeSurface(temp);
        }

        labelTexture = SDL_CreateTextureFromSurface(Game::renderer, surf);
        SDL_FreeSurface(surf);

        SDL_QueryTexture(labelTexture, nullptr, nullptr, &position.w, &position.h);
    }

    void AddItem(std::string item)
    {
        inventory.push_back(item);
    }

    std::vector<std::string> GetInventory()
    {
        return inventory;
    }

    void draw() override
    {
        SDL_RenderCopy(Game::renderer, labelTexture, nullptr, &position);
    }

private:
    // struct InventoryItem
    // {
    //     std::string name;
    //     int quantity;
    // };

    // std::vector<InventoryItem> inventory;
    SDL_Rect position;
    std::vector<std::string> inventory;
    std::string labelText;
    std::string labelFont;
    SDL_Color textColor;
    SDL_Texture *labelTexture;
};

// Implement later on for this to work with vectors