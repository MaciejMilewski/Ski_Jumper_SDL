#pragma once

#include <iostream>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

enum class tryby { normal, pressed, selected };

class CheckBox
{
public:
    CheckBox(const char* normalImagePath, const char* selectedImagePath, const char* label, const char* fontName,
        int fontSize, int width, int height, int posX, int posY, SDL_Renderer& render);

    virtual~CheckBox();

    void ShowButton();
    void set_Mode(tryby t);
    tryby Clicked(SDL_Event& e);

private:
    SDL_Renderer* render = NULL;
    SDL_Surface* loaded_surface = NULL;
    SDL_Texture* normalImage, * selectedImage, * mTekst;

    TTF_Font* pFont;

    SDL_Point mouse_XY;

    SDL_Color textColor = { 34, 123, 255 };

    tryby mode = tryby::normal;

    int width = 0, height = 0, posX = 0, posY = 0, fontSize = 0;

    std::string imagePath, selectedImagePath, fontName;
};
