#include "Dashboard.h"
#include <iostream>
#include <sstream>

Dashboard::Dashboard(const char* label, unsigned int fontSize, int posX, int posY, SDL_Renderer* render)
{
    this->posX = posX;
    this->posY = posY;
    this->fontSize = fontSize;
    this->label = label;
    this->render = render;

    pFont = TTF_OpenFont("img/arial.ttf", fontSize);
    if(!pFont)
    {
        std::cout << "Brak pliku arial.ttf!\n" << TTF_GetError() << '\n';
    }

    canvas = TTF_RenderText_Blended(pFont, label, textColor);
    if(!canvas)
    {
        std::cout << "Nie stworzono surface - canvas\n" << TTF_GetError() << '\n';
    }
    TTF_SetFontStyle(pFont, TTF_STYLE_BOLD);

    mText = SDL_CreateTextureFromSurface(this->render, canvas);
    if(!mText)
    {
        std::cout << "Nie stworzono texture - mText\n" << SDL_GetError() << '\n';
    }

    dest.x = posX;
    dest.y = posY;
    dest.w = canvas->w;
    dest.h = canvas->h;

    SDL_FreeSurface(canvas);
    TTF_CloseFont(pFont);

}

Dashboard::~Dashboard()
{
    canvas = nullptr;
    SDL_DestroyTexture(mText);
    mText = nullptr;
    render = nullptr;
    pFont = nullptr;
} 

void Dashboard::Show()
{
    SDL_RenderCopy(this->render, mText, NULL, &dest);
}