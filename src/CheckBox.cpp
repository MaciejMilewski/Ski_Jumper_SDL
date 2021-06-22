#include "CheckBox.h"

CheckBox::~CheckBox()
{
    SDL_DestroyTexture(normalImage);
        normalImage = nullptr;
    SDL_DestroyTexture(selectedImage);
        selectedImage = nullptr;
}

CheckBox::CheckBox(const char* normalImagePath, const char* selectedImagePath,
    const char* label, const char* fontName,
    int fontSize, int width, int height, int posX, int posY, SDL_Renderer& render)
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

    loadedSurface = IMG_Load(selectedImagePath);
    selectedImage = SDL_CreateTextureFromSurface(&render, loadedSurface);
    if (!selectedImage)
        std::cout << " Nie stworzono 'surface' z " << selectedImagePath << ", " << SDL_GetError() << '\n';
    SDL_FreeSurface(loadedSurface);

    loadedSurface = IMG_Load(normalImagePath);
    normalImage = SDL_CreateTextureFromSurface(&render, loadedSurface);
    if (!normalImage)
        std::cout << " Nie stworzono 'surface' z " << normalImagePath << ", " << SDL_GetError() << '\n';
    SDL_FreeSurface(loadedSurface);

    if (width) {
        pFont = TTF_OpenFont(fontName, fontSize);
        if (!pFont)
        {
            std::cout << " Brak pliku " << fontName << " : " << TTF_GetError() << '\n';
        }
        else
        {
            SDL_Surface* textSurface = TTF_RenderText_Blended(pFont, label, textColor);
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
                    width = textSurface ->w; 
                    height = textSurface->h;
                    TTF_CloseFont(pFont);
                }
                SDL_FreeSurface(textSurface);
            }
        }
    }
}

CheckboxMode CheckBox::Clicked(SDL_Event& e)
{
    SDL_GetMouseState(&mouseCoords.x, &mouseCoords.y);

    int posXcheck = posX + width + 10;

    bool InOut = ((mouseCoords.x >= posXcheck) && (mouseCoords.x <= (posXcheck + height)) && (mouseCoords.y >= posY) && (mouseCoords.y <= (posY + height))) ? 1 : 0;

    if (InOut && e.type == SDL_MOUSEBUTTONUP)
    {
        (mode == CheckboxMode::selected) ? mode = CheckboxMode::normal : mode = CheckboxMode::selected;
        SDL_WarpMouseGlobal(760, 650);
    }
    return mode;
}

void CheckBox::ShowButton()
{
    SDL_Rect dest = { posX, posY, width, height };
    SDL_Rect check = { posX + width + 10, posY, height, height };

    switch (mode) {
    case CheckboxMode::normal:
        if (width) {
            SDL_SetRenderDrawColor(render, 0, 123, 0, 255);
            SDL_RenderDrawRect(render, &dest);
            SDL_RenderCopy(render, mTekst, NULL, &dest);
        }
        SDL_RenderCopy(render, normalImage, NULL, &check);
        break;
    case CheckboxMode::selected:
        if (width) {
            SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
            SDL_RenderDrawRect(render, &dest);
            SDL_RenderCopy(render, mTekst, NULL, &dest);
        }
        SDL_RenderCopy(render, selectedImage, NULL, &check);
        break;
    }
}

void CheckBox::setMode(CheckboxMode t)
{
    mode = t;
}

