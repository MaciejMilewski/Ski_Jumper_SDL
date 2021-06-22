#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include "TextureManager.h"


class RollBox
{
public:
    RollBox(std::string tab[],
        unsigned int width, unsigned int height,
        unsigned int posX, unsigned int posY, int size, SDL_Renderer* render);

    virtual~RollBox();

    void Show();

    void Clicked(SDL_Event& e);
    int  GetSelected();

private:
    SDL_Renderer* render = NULL;
    SDL_Texture* texture = NULL;

    SDL_Point mouseCoords{ 0,0 };

    std::string* t = nullptr;

    int width = 0, height = 0, posX = 0, posY = 0, sizeTab = 0, pictureID = 0;
};

