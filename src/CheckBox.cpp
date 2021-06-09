#include "CheckBox.h"


CheckBox::~CheckBox()
{
    SDL_DestroyTexture(normalImage);
    SDL_DestroyTexture(selectedImage);
}

CheckBox::CheckBox(const char* normalImagePath, const char* selectedImagePath,
    const char* label, const char* fontName,
    unsigned int fontSize, unsigned int width, unsigned int height,
    unsigned int posX, unsigned int posY, SDL_Renderer& render)
{
    this->width = width;
    this->height = height;
    this->posX = posX;
    this->posY = posY;
    this->fontSize = fontSize;
    this->render = &render;
    this->imagePath = normalImagePath;
    this->selectedImagePath = selectedImagePath;
    this->fontName = fontName;

    loaded_surface = IMG_Load(selectedImagePath);
    selectedImage = SDL_CreateTextureFromSurface(&render, loaded_surface);
    if (!selectedImage)
        std::cout << " Nie stworzono 'surface' z " << selectedImagePath << ", " << SDL_GetError() << '\n';
    SDL_FreeSurface(loaded_surface);

    loaded_surface = IMG_Load(normalImagePath);
    normalImage = SDL_CreateTextureFromSurface(&render, loaded_surface);
    if (!normalImage)
        std::cout << " Nie stworzono 'surface' z " << normalImagePath << ", " << SDL_GetError() << '\n';
    SDL_FreeSurface(loaded_surface);

    if (width) {
        pFont = TTF_OpenFont(fontName, fontSize);
        if (!pFont)
        {
            std::cout << " Brak pliku " << fontName << " : " << TTF_GetError() << '\n';
        }
        else
        {
            SDL_Surface* textSurface = TTF_RenderText_Solid(pFont, label, textColor);
            if (!textSurface) {
                std::cout << " Nie stworzono 'surface' z pliku .ttf : " << TTF_GetError() << '\n';
            }
            else
            {
                mTekst = SDL_CreateTextureFromSurface(this->render, textSurface);
                if (!mTekst) {
                    std::cout << " Nie stworzono textury z renderowanego tekstu : " << SDL_GetError() << '\n';
                }
                else
                {
                    width = textSurface->w; // zapisz rozmiar stworzonego obrazka
                    height = textSurface->h;
                    TTF_CloseFont(pFont);
                }
              SDL_FreeSurface(textSurface);
            }
        }
    }
}

tryby CheckBox::Clicked(SDL_Event& e)
{
    SDL_GetMouseState(&mouse_XY.x, &mouse_XY.y);

    int posXcheck = posX + width + 10;

    bool InOut = (mouse_XY.x >= posXcheck && mouse_XY.x <= posXcheck + height && mouse_XY.y >= posY && mouse_XY.y <= posY + height) ? 1 : 0;

    if (InOut && e.type == SDL_MOUSEBUTTONUP)
    {       
        (mode == tryby::selected) ? mode = tryby::normal : mode = tryby::selected;       
    }
    return mode;
}

void CheckBox::ShowButton()
{
    SDL_Rect dest = { posX, posY, width, height };
    SDL_Rect check = { posX + width + 10, posY, height, height };

    switch (mode) {
    case tryby::normal:
        if (width) {
            SDL_SetRenderDrawColor(render, 0, 123, 0, 255);
            SDL_RenderDrawRect(render, &dest);
            SDL_RenderCopy(render, mTekst, NULL, &dest);
        }       
        SDL_RenderCopy(render, normalImage, NULL, &check);
        break;
    case tryby::selected:
        if (width) {
            SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
            SDL_RenderDrawRect(render, &dest);
            SDL_RenderCopy(render, mTekst, NULL, &dest);
        }        
        SDL_RenderCopy(render, selectedImage, NULL, &check);
        break;
    }
}

void CheckBox::set_Mode(tryby t)
{
    mode = t;
}

