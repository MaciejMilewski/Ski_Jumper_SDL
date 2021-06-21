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

    loadedSurface = IMG_Load(selectedImagePath);
    selectedImage = SDL_CreateTextureFromSurface(&render, loadedSurface);

    if (!selectedImage)
        std::cout << " Nie stworzono 'surface' z pliku " << selectedImagePath << ", " << SDL_GetError() << '\n';
    SDL_FreeSurface(loadedSurface);

    loadedSurface = IMG_Load(normalImagePath);
    normalImage = SDL_CreateTextureFromSurface(&render, loadedSurface);
    if (!normalImage)
        std::cout << " Nie stworzono 'surface' z " << normalImagePath << ", " << SDL_GetError() << '\n';
    SDL_FreeSurface(loadedSurface);
}

Button::~Button()
{
    SDL_DestroyTexture(normalImage);
        normalImage = nullptr;
    SDL_DestroyTexture(selectedImage);
        selectedImage = nullptr;
}

int Button::GetX()
{
    return posX;
}

int Button::GetY()
{
    return posY;
}

enum Mode Button::Clicked(SDL_Event& e)
{
    SDL_GetMouseState(&mouse_XY.x, &mouse_XY.y);

    mode = (mouse_XY.x >= posX && mouse_XY.x <= posX + width && mouse_XY.y >= posY && mouse_XY.y <= posY + height) ?
        Mode::pressed : Mode::normal;

    bool mousedown = 0;

    if (e.type == SDL_MOUSEBUTTONDOWN)
        mousedown = 1;

    SDL_Delay(30);

    if (mousedown || e.type == SDL_MOUSEBUTTONUP)
    {
        if (mode == Mode::pressed)  mode = Mode::selected;
        
        SDL_Delay(30);
       
        mousedown = 0;
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
    case Mode::normal:
        SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
        SDL_RenderDrawRect(render, &dest);
        SDL_RenderCopy(render, normalImage, NULL, &dest);
        break;
    case Mode::pressed:
        SDL_SetRenderDrawColor(render, 0, 123, 0, 255);
        SDL_RenderDrawRect(render, &dest);
        SDL_RenderCopy(render, normalImage, NULL, &dest);
        break;
    case Mode::selected:
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


