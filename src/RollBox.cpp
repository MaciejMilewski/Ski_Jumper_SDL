#include <iostream>

#include "RollBox.h"

RollBox::RollBox(std::string tab[], unsigned int width, unsigned int height, unsigned int posX, unsigned int posY, int size, SDL_Renderer* render)
{    
    this->sizeTab = size;
    this->width   = width;
    this->height  = height;
    this->posX    = posX;
    this->posY    = posY;
    this->render  = render;  
    this->texture = texture;
    this->t       = tab;
}

RollBox::~RollBox()
{
    texture = nullptr;
    render  = nullptr;
          t = nullptr;
}

void RollBox::Show()
{  
  TextureManager::Instance()->Draw(t[nr_Pict], posX, posY, width, height, 1.0, 0, render, SDL_FLIP_NONE);  

  TextureManager::Instance()->Draw("przewijacz", posX + 5 + width, int(posY + height * 0.5 - 15), 30, 30, 1.0, 0, render, SDL_FLIP_NONE);
}

void RollBox::Clicked(SDL_Event& e)
{
    SDL_GetMouseState(&mouse_XY.x, &mouse_XY.y);

    int arrX = posX + 5 + width, arrY = int(posY + height * 0.5 - 15);

    bool mode = (mouse_XY.x >= arrX && mouse_XY.x <= arrX + 30 && mouse_XY.y >= arrY && mouse_XY.y <= arrY + 30) ?
         mode = true : mode = false;

    if ((mode == true) && (e.type == SDL_MOUSEBUTTONDOWN))  {       
        
        nr_Pict < sizeTab ? nr_Pict++ : nr_Pict = 0;           

        TextureManager::Instance()->Draw(t[nr_Pict], posX, posY, width, height, 1.0, 0, render, SDL_FLIP_NONE);       
    }   
}

