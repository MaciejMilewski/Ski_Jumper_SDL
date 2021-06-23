#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Physics.h"
#include "Player.h"
#include "Label.h"
#include "Particle.h"

enum class JumpPhase { WAIT, SLIDE, JUMP, TELEMARK, END };

class Jump   
{
public:
    Jump(SDL_Renderer* render, Physics* physics, Player* player);
    ~Jump();

    SDL_Renderer* ren;  
    SDL_Rect rectJumper = { 88,  85,  38,  38 };
    SDL_Rect rectStartJump = { 310, 220,  38,  38 };
    SDL_Rect rectArrow = { 650,  55,  88,  88 };
    SDL_Rect rectWind = { 600,  10, 180, 150 };
    SDL_Rect rectBanner = { 0, 422, 248, 178 };

    SDL_Texture* ramp = nullptr;
    SDL_Texture* jumperOnRamp = nullptr;
    SDL_Texture* jumperStartingJump = nullptr;
    SDL_Texture* jumperLeaning  = nullptr;
    SDL_Texture* jumperSoars = nullptr;
    SDL_Texture* jumperToLanding = nullptr;
    SDL_Texture* jumperTelemark = nullptr;

    SDL_Texture* banner = nullptr;
    SDL_Texture* windArrow = nullptr;
    SDL_Texture* windTable = nullptr;
    SDL_Surface* surfaceRamp = nullptr;  

    JumpPhase Wait();
    JumpPhase Flight();
    JumpPhase Slide();
    JumpPhase Landing();

    void ShowDashboard();
    void SetRender(SDL_Renderer* r);

    Physics* physics = nullptr;
    Player*  player  = nullptr;

    Particle* particle = new Particle();

    const int dashboardXpos = 143;
    const int dashboardYpos = 431;
    const int dashboardInterval = 35;
    const int dashboardFontSize = 20;

    const int FPS = 20;

    Uint32 frameDelay = Uint32 (1000 / FPS);
    Uint32 frameStart = 0, frameTime = 0;

    const int takeOffThreshold = 300; 
    double dt   = 0.0;                                      
    double alfa = 0.0;                                 
    double mass = player->getWeight();                 
    double Cd = 0.0;                                                                                                                      
    double Cw = 0.0;                                     
    double Vw = 3.0;                                                                                                                                           
    double Vm = 0;                                      
    double g  = 9.81;                                   
    double Sx1 = 310.0;                                 
    double Sy1 = 220.0;                                 
    double SX = 0.0, SY = 0.0;                            
};