#include <iostream>

#include "RollBox.h"

RollBox::RollBox(std::string tab[], unsigned int width, unsigned int height, unsigned int posX, unsigned int posY, int size, SDL_Renderer* render)
{
    this->sizeTab = size;
    this->width = width;
    this->height = height;
    this->posX = posX;
    this->posY = posY;
    this->render = render;
    this->texture = texture;
    this->t = tab;
}

RollBox::~RollBox()
{
    texture = nullptr;
    render = nullptr;
    t = nullptr;
}

void RollBox::Show()
{
    TextureManager::Instance()->Draw(t[pictureID], posX, posY, width, height, 1.0, 0, render, SDL_FLIP_NONE);

    TextureManager::Instance()->Draw("przewijacz", posX + 5 + width, int(posY + height * 0.5 - 15), 30, 30, 1.0, 0, render, SDL_FLIP_NONE);
}

void RollBox::Clicked(SDL_Event& e)
{
    SDL_GetMouseState(&mouseCoords.x, &mouseCoords.y);

    int arrX = posX + 5 + width, arrY = int(posY + height * 0.5 - 15);

    bool mode = false;
    mode = (mouseCoords.x >= arrX && mouseCoords.x <= arrX + 30 && mouseCoords.y >= arrY && mouseCoords.y <= arrY + 30) ?
        true : false;

    if ((mode == true) && (e.type == SDL_MOUSEBUTTONDOWN)) {

        pictureID < sizeTab ? pictureID++ : pictureID = 0;

        TextureManager::Instance()->Draw(t[pictureID], posX, posY, width, height, 1.0, 0, render, SDL_FLIP_NONE);
    }
}

int RollBox::GetSelected()
{
    return pictureID;
};
