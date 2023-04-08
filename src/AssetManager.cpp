#include "AssetManager.hpp"
#include "ECS/Components.hpp"

AssetManager::AssetManager(Manager *man)
    : manager(man)
{
}

AssetManager::~AssetManager()
{
}

void AssetManager::AddTexture(std::string id, const char *path)
{
    textures.emplace(id, TextureManager::LoadTexture(path));
}

SDL_Texture *AssetManager::GetTexture(std::string id)
{
    return textures[id];
}

void AssetManager::DrawTexture(std::string id, SDL_Rect mRect, int xpos, int ypos, int destW, int destH, float scale)
{
    if (FindTexture(id))
    {
        SDL_Texture *texture = GetTexture(id);
        SDL_Rect srcRect, destRect;

        srcRect.x = mRect.x;
        srcRect.y = mRect.y;
        srcRect.w = mRect.w;
        srcRect.h = mRect.h;

        destRect.x = xpos;
        destRect.y = ypos;
        destRect.w = destW * scale;
        destRect.h = destH * scale;

        TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
    }
    return;
}

void AssetManager::DeleteTexture(std::string id)
{
    SDL_Texture *texture = GetTexture(id);
    textures.erase(id);
    SDL_DestroyTexture(texture);
}

bool AssetManager::FindTexture(std::string id)
{
    auto textureIt = textures.find(id);

    if (textureIt == textures.end())
        return false;

    return true;
}

void AssetManager::AddFont(std::string id, std::string path, int fontSize)
{
    fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));
}

TTF_Font *AssetManager::GetFont(std::string id)
{
    return fonts[id];
}