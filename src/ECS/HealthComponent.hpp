#pragma once

#include <SDL2/SDL.h>

#include "Components.hpp"

class HealthComponent : public Component
{
public:
    HealthComponent(int maxHealth, int currentHealth, int xPos, int yPos, int width, int height)
        : m_maxHealth(maxHealth), m_currentHealth(currentHealth), m_xPos(xPos), m_yPos(yPos), m_width(width), m_height(height)
    {
        m_barColor = {0, 255, 0, 255};
    }

    void setCurrentHealth(int currentHealth)
    {
        m_currentHealth = currentHealth;
    }

    void updateHealthSmallPotion()
    {
        m_currentHealth += smallPotion;
    }

    void updateHealthBigPotion()
    {
        m_currentHealth += bigPotion;
    }

    void draw(SDL_Renderer *renderer)
    {
        SDL_Rect bgRect = {m_xPos, m_yPos, m_width, m_height};
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderFillRect(renderer, &bgRect);

        int barWidth = static_cast<int>((m_currentHealth / static_cast<float>(m_maxHealth)) * m_width);
        SDL_Rect barRect = {m_xPos, m_yPos, barWidth, m_height};
        SDL_SetRenderDrawColor(renderer, m_barColor.r, m_barColor.g, m_barColor.b, m_barColor.a);
        SDL_RenderFillRect(renderer, &barRect);
    }

private:
    int m_maxHealth;
    int m_currentHealth;
    int m_xPos;
    int m_yPos;
    int m_width;
    int m_height;
    SDL_Color m_barColor;
    int smallPotion = 25;
    int bigPotion = 50;
};