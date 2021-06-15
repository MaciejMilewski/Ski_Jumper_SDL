#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Physics.h"
#include "Player.h"

enum class Fazy_skoku { CZEKA, ZJAZD, SKOK, TELEMARK, END };

class Jump   
{
public:
    Jump(SDL_Renderer* render, Physics* physics, Player* player);
    ~Jump();

    SDL_Rect rect_skoczek = { 88,  85,  38,  38 };
    SDL_Rect rect_wyskok = { 310, 220,  38,  38 };
    SDL_Rect rect_arrow = { 650,  55,  88,  88 };
    SDL_Rect rect_wind = { 600,  10, 180, 150 };
    SDL_Rect rect_banner = { 30, 300, 350, 260 };

    SDL_Texture* rampa = nullptr;
    SDL_Texture* jumper_na_rampie = nullptr;
    SDL_Texture* jumper_wybicie = nullptr;
    SDL_Texture* jumper_pochylony = nullptr;
    SDL_Texture* jumper_szybuje = nullptr;
    SDL_Texture* jumper_do_ladowania = nullptr;
    SDL_Texture* jumper_telemark = nullptr;

    SDL_Texture* banner = nullptr;
    SDL_Texture* wind_arrow = nullptr;
    SDL_Texture* wind_table = nullptr;
    SDL_Surface* surfaceRamp = nullptr;  

    Fazy_skoku Czeka();
    Fazy_skoku Wyskok();
    Fazy_skoku Zjazd();
    Fazy_skoku Landing();

    void set_Render(SDL_Renderer* r);

    Physics *physics = nullptr;
    Player*  player  = nullptr;

    const int FPS = 20;

    Uint32 frameDelay = Uint32 (1000 / FPS);
    Uint32 frameStart = 0, frameTime = 0;

    SDL_Renderer* ren;         
};