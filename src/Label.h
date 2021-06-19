#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Label {
    public:
        Label(const char* label, unsigned int fontSize, int posX, int posY, SDL_Renderer* render);
        virtual ~Label();

        void Show();
    private:
        SDL_Renderer* render = nullptr;
        SDL_Surface* canvas = nullptr;
        SDL_Texture* mText = nullptr;
        TTF_Font* pFont = nullptr;
        int posX = 0, posY = 0, fontSize = 0;
        SDL_Color textColor = { 3, 94, 252 };
        const char* label = nullptr;
        SDL_Rect dest = { 0, 0, 0, 0 };
 
};