#include <SDL2/SDL_image.h>

#include "Button.h"

Button::Button(const char* normalImagePath, const char* selectedImagePath,
    unsigned int width, unsigned int height,
    unsigned int posX, unsigned int posY, SDL_Renderer& render)
{
    this->width = width;
    this->height = height;
    this->posX = posX;
    this->posY = posY;
    this->render = &render;
    this->imagePath = normalImagePath;
    this->selectedImagePath = selectedImagePath;

    loaded_surface = IMG_Load(selectedImagePath);
    selectedImage = SDL_CreateTextureFromSurface(&render, loaded_surface);
    if (!selectedImage)
        std::cout << " Nie stworzono 'surface' z pliku" << selectedImagePath << ", " << SDL_GetError() << '\n';
    SDL_FreeSurface(loaded_surface);

    loaded_surface = IMG_Load(normalImagePath);
    normalImage = SDL_CreateTextureFromSurface(&render, loaded_surface);
    if (!normalImage)
        std::cout << " Nie stworzono 'surface' z " << normalImagePath << ", " << SDL_GetError() << '\n';
    SDL_FreeSurface(loaded_surface);
}

Button::~Button()
{
    SDL_DestroyTexture(normalImage);
    SDL_DestroyTexture(selectedImage);
}

int Button::GetX()
{
    return posX;
}

int Button::GetY()
{
    return posY;
}

enum tryb Button::Clicked(SDL_Event& e)
{
    SDL_GetMouseState(&mouse_XY.x, &mouse_XY.y);     // odczytuje pozycj� myszki 

    mode = (mouse_XY.x >= posX && mouse_XY.x <= posX + width && mouse_XY.y >= posY && mouse_XY.y <= posY + height) ?
        tryb::pressed : tryb::normal;

    if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
    {
        if (mode == tryb::pressed)  mode = tryb::selected;
    }
    return mode;
}

void Button::ShowButton()
{
    SDL_Rect dest;
    dest.x = posX;
    dest.y = posY;
    dest.w = width;
    dest.h = height;

    switch (mode) {
    case tryb::normal:
        SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
        SDL_RenderDrawRect(render, &dest);
        SDL_RenderCopy(render, normalImage, NULL, &dest);
        break;
    case tryb::pressed:
        SDL_SetRenderDrawColor(render, 0, 123, 0, 255);
        SDL_RenderDrawRect(render, &dest);
        SDL_RenderCopy(render, normalImage, NULL, &dest);
        break;
    case tryb::selected:
        SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
        SDL_RenderDrawRect(render, &dest);
        SDL_RenderCopy(render, selectedImage, NULL, &dest);
        break;
    }
}

void Button::SetX(int x)
{
    posX = x;
}

void Button::SetY(int y)
{
    posY = y;
}


