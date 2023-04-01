#pragma once

#include <map>
#include <string>
#include <SDL2_ttf/SDL_ttf.h>

#include "TextureManager.hpp"
#include "Vector2D.hpp"
#include "ECS/ECS.hpp"

class AssetManager
{
public:
    AssetManager(Manager *man);
    ~AssetManager();

    void AddTexture(std::string id, const char *path);
    SDL_Texture *GetTexture(std::string id);

    void DrawTexture(std::string id, SDL_Rect mRect, int xpos, int ypos, int destW, int destH, float scale);

    void AddFont(std::string id, std::string path, int fontSize);
    TTF_Font *GetFont(std::string id);

private:
    Manager *manager;
    std::map<std::string, SDL_Texture *> textures;
    std::map<std::string, TTF_Font *> fonts;
};