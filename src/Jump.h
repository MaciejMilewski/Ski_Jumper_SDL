#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Physics.h"
#include "Player.h"
#include "Label.h"
#include "Particle.h"

enum class Fazy_skoku { CZEKA, ZJAZD, SKOK, TELEMARK, END };

class Jump   
{
public:
    Jump(SDL_Renderer* render, Physics* physics, Player* player);
    ~Jump();

    SDL_Renderer* ren;  
    SDL_Rect rect_skoczek = { 88,  85,  38,  38 };
    SDL_Rect rect_wyskok = { 310, 220,  38,  38 };
    SDL_Rect rect_arrow = { 650,  55,  88,  88 };
    SDL_Rect rect_wind = { 600,  10, 180, 150 };
    SDL_Rect rect_banner = { 0, 422, 248, 178 };

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

    void ShowDashboard();
    void set_Render(SDL_Renderer* r);

    Physics* physics = nullptr;
    Player*  player  = nullptr;

    Particle* particle = new Particle();

    const int dashboard_x_pos = 143;
    const int dashboard_y_pos = 431;
    const int dashboard_interval = 35;
    const int dashboard_font_size = 20;

    const int FPS = 20;

    Uint32 frameDelay = Uint32 (1000 / FPS);
    Uint32 frameStart = 0, frameTime = 0;

    double dt   = 0.0;                                  // czas skoku w powietrzu dt = aktualny_czas - poczatek-skoku     
    double alfa = 0.0;                                  // brak wybicia skoczka w g�r�, czyli k�t 0 stopni
    double masa = player->get_Weight();                 // masa skoczka
    double Cd = 0.0;                                    // wsp�czynnik oporu powietrza                                                                                   
    double Cw = 0.0;                                    // wsp�czynnik wiatru  
    double Vw = 3.0;                                    // pr�dko�� wiatru - STABLE 3 m/s; FICKLE 1-3 m/s (losowa funkcja 1-4 m/s)
                                                        // double GammaW = 0.0; kierunek wiatru - k�t w stosunku do skoczka
                                                        // 0 stopni to idea� (RAND -45/+45 stopni)
    double Vm = 0;                                      // pr�dko�� skoczka na progu !     

    double g  = 9.81;                                   // przy�pieszenie ziemskie  
    double Sx1 = 310.0;                                 // punkt wsp�rz�dnej x - miejsce wybicia z rampy skoczni 
    double Sy1 = 220.0;                                 // punkt wsp�rz�dnej y - miejsce wybicia z rampy skoczni
    double SX = 0.0, SY = 0.0;                          // wsp�rz�dne x,y po przeliczeniu   
};