#pragma once

#include <iostream>

#include <SDL2/SDL.h>

enum class Mode { normal, pressed, selected };

class Button
{
public:
    Button(const char* normalImagePath, const char* selectedImagePath,
        unsigned int width, unsigned int height,
        unsigned int posX, unsigned int posY, SDL_Renderer& render);

    virtual~Button();

    void ShowButton();
    void SetX(int x);
    void SetY(int y);
    int  GetX();
    int  GetY();
    Mode Clicked(SDL_Event& e);

private:
    SDL_Renderer* render = nullptr;
    SDL_Surface* loadedSurface = nullptr;
    SDL_Texture* normalImage, * selectedImage;
    SDL_Point    mouse_XY;

    Mode mode = Mode::normal;

    int width = 0, height = 0, posX = 0, posY = 0;

    std::string imagePath = "", selectedImagePath = "";
};
