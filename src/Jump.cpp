#include <iostream>
#include <sstream>
#include <math.h>

#include "Jump.h"

Jump::Jump(SDL_Renderer* render, Physics* physics, Player* player): ren(render), physics(physics), player(player)
{
    ramp = IMG_LoadTexture(ren,"img/rampa.png");    

    surfaceRamp = IMG_Load("img/rampa.png");    

        SDL_Surface* loadSurface = IMG_Load("img/rozbieg.png");

        SDL_SetColorKey(loadSurface, SDL_TRUE, SDL_MapRGB(loadSurface->format, 255, 255, 255));

    jumperOnRamp = SDL_CreateTextureFromSurface(ren, loadSurface);

        SDL_FreeSurface(loadSurface);

        SDL_Surface* loadWybicie = IMG_Load("img/rozbieg.png");

        SDL_SetColorKey(loadWybicie, SDL_TRUE, SDL_MapRGB(loadWybicie->format, 255, 255, 255)); 

    jumperStartingJump = SDL_CreateTextureFromSurface(ren, loadWybicie);

        SDL_FreeSurface(loadWybicie);

        SDL_Surface* loadPochylony = IMG_Load("img/lot_pochylony.png");

        SDL_SetColorKey(loadPochylony, SDL_TRUE, SDL_MapRGB(loadPochylony->format, 255, 255, 255));

    jumperLeaning = SDL_CreateTextureFromSurface(ren, loadPochylony);

        SDL_FreeSurface(loadPochylony);

        SDL_Surface* loadSzybuje = IMG_Load("img/lot_szeroko.png");

        SDL_SetColorKey(loadSzybuje, SDL_TRUE, SDL_MapRGB(loadSzybuje->format, 255, 255, 255));

    jumperSoars = SDL_CreateTextureFromSurface(ren, loadSzybuje);

        SDL_FreeSurface(loadSzybuje);

        SDL_Surface* loadLaduje = IMG_Load("img/do_ladowania.png");

        SDL_SetColorKey(loadLaduje, SDL_TRUE, SDL_MapRGB(loadLaduje->format, 255, 255, 255));

    jumperToLanding = SDL_CreateTextureFromSurface(ren, loadLaduje);

        SDL_FreeSurface(loadLaduje);

        SDL_Surface* loadTelemark = IMG_Load("img/telemark.png");

        SDL_SetColorKey(loadTelemark, SDL_TRUE, SDL_MapRGB(loadTelemark->format, 255, 255, 255));

    jumperTelemark = SDL_CreateTextureFromSurface(ren, loadTelemark);

        SDL_FreeSurface(loadTelemark);

        SDL_Surface* ladujSurface = IMG_Load("img/Banner.png");

        SDL_SetColorKey(ladujSurface, SDL_TRUE, SDL_MapRGB(ladujSurface->format, 255, 255, 255));

    banner = SDL_CreateTextureFromSurface(ren, ladujSurface);

        SDL_FreeSurface(ladujSurface);

    windArrow = IMG_LoadTexture(ren,"img/arrow-wind.png");
    windTable = IMG_LoadTexture(ren,"img/wind-table.png");

    Particle::Style style = Particle::Style::NONE;
    
    switch (player->getWeather())
    {
        case 0: style = Particle::Style::RAIN; break;
        case 1: style = Particle::Style::SUN ; break;
        case 2: style = Particle::Style::SNOW; break;
        case 3: style = Particle::Style::NONE; break;
    }

    particle->setRenderer(ren);
    particle->setPosition(400, 50);                   
    particle->setStyle(style);                        
    particle->setStartSpin(0);
    particle->setEndSpin(190);
    particle->setStartSpinVar(90);
    particle->setEndSpinVar(190);
};

void Jump::ShowDashboard() 
{
    std::stringstream Vm_ss;
    Vm_ss << physics->getVelocity();
    Label jumper_velocity_label(Vm_ss.str().c_str(), dashboardFontSize, dashboardXpos, dashboardYpos, ren);
    jumper_velocity_label.Show();

    std::stringstream alfa_ss;
    alfa_ss << alfa;
    Label jump_angle_label(alfa_ss.str().c_str(), dashboardFontSize, dashboardXpos, dashboardYpos+dashboardInterval, ren);
    jump_angle_label.Show();

    std::string weather;
    switch (player->getWeather()) // "rain", "sun",  "snow", "wind"
    {
        case 0: weather = "rain"; break;
        case 1: weather = "sun"; break;
        case 2: weather = "snow"; break;
        case 3: weather = "wind"; break;
    }

    Label weather_label(weather.c_str(), dashboardFontSize, dashboardXpos, dashboardYpos+2*dashboardInterval, ren);
    weather_label.Show();

    std::stringstream Vw_ss;
    Vw_ss << Vw;
    Label wind_velocity_label(Vw_ss.str().c_str(), dashboardFontSize, dashboardXpos, dashboardYpos+3*dashboardInterval, ren);
    wind_velocity_label.Show();

    std::stringstream Gamma_ss;
    Gamma_ss << physics->getGammaW();
    Label wind_angle_label(Gamma_ss.str().c_str(), dashboardFontSize, dashboardXpos, dashboardYpos+4*dashboardInterval, ren);
    wind_angle_label.Show();
}

Jump::~Jump()
{
    SDL_DestroyTexture(ramp);
    SDL_DestroyTexture(jumperOnRamp);
    SDL_DestroyTexture(jumperStartingJump);
    SDL_DestroyTexture(jumperLeaning);
    SDL_DestroyTexture(jumperSoars);
    SDL_DestroyTexture(jumperToLanding);
    SDL_DestroyTexture(jumperTelemark);
    SDL_FreeSurface(surfaceRamp);
    SDL_DestroyTexture(banner);
    SDL_DestroyTexture(windArrow);
    SDL_DestroyTexture(windTable);

    ramp = nullptr;
    jumperOnRamp = nullptr;
    jumperStartingJump = nullptr;
    jumperLeaning = nullptr;
    jumperSoars = nullptr;
    jumperToLanding = nullptr;
    jumperTelemark = nullptr;
    banner = nullptr;
    windArrow = nullptr;
    windTable = nullptr;
    surfaceRamp = nullptr;

    delete particle;
    physics = nullptr; 
    player = nullptr;
    ren = nullptr;
};

JumpPhase Jump::Wait()
{
    rectJumper = { 88,  85,  38,  38 };
    rectStartJump  = { 310, 220,  38, 38 };

    physics->setJumpStart(false);

    SDL_RenderCopy(ren, ramp, NULL, NULL);

    SDL_RenderCopyEx(ren, banner, NULL, &rectBanner, 0.0, NULL, SDL_FLIP_NONE);
    ShowDashboard();

    SDL_RenderCopy(ren, jumperOnRamp, NULL, &rectJumper);
    particle->draw();
    SDL_RenderCopyEx(ren, windTable, NULL, &rectWind, 0.0, NULL, SDL_FLIP_NONE);
    SDL_RenderCopyEx(ren, windArrow, NULL, &rectArrow, physics->getGammaW(), NULL, SDL_FLIP_NONE);

    return JumpPhase::WAIT;
}

JumpPhase Jump::Landing()
{
    double angle_skoczek = 0.0;
    double angle_tor     = 0.0;

    Uint32 startTime = SDL_GetTicks();

    JumpPhase skok = JumpPhase::TELEMARK;

    SDL_Event e;

    while (skok == JumpPhase::TELEMARK)
    {
        frameStart = SDL_GetTicks();

        SDL_RenderCopy(ren, ramp, NULL, NULL);

        SDL_RenderCopyEx(ren, banner, NULL, &rectBanner, 0.0, NULL, SDL_FLIP_NONE);
        ShowDashboard();

        SDL_RenderCopyEx(ren, jumperTelemark, NULL, &rectStartJump, angle_skoczek, NULL, SDL_FLIP_NONE);
        particle->draw();
        SDL_RenderCopyEx(ren, windTable, NULL, &rectWind, 0.0, NULL, SDL_FLIP_NONE);
        SDL_RenderCopyEx(ren, windArrow, NULL, &rectArrow, physics->getGammaW(), NULL, SDL_FLIP_NONE);
        SDL_RenderPresent(ren);

        rectStartJump.x += int(physics->roadOnRamp(startTime) * (cos(angle_tor * physics->Deg2Rad)));
        rectStartJump.y += int(physics->roadOnRamp(startTime) * (sin(angle_tor * physics->Deg2Rad)));

        while (!physics->checkCollision(surfaceRamp, rectStartJump.x, rectStartJump.y))
        {
            rectStartJump.y++;
        }

        if (rectStartJump.x > 515)
        {
            angle_skoczek = -35.0f; rectStartJump.y -= 18;
        }

        if (rectStartJump.x > 720)
        {
            skok = JumpPhase::WAIT;
        }

        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT) skok = JumpPhase::END;
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym)
                {
                case SDLK_RETURN:
                    break;
                case SDLK_ESCAPE: skok = JumpPhase::END;
                    break;
                }
            }
        }

        frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    return JumpPhase::END;
};

JumpPhase Jump::Flight()
{
    Uint32 poczatek_skoku = SDL_GetTicks();             // Zaczynamy liczy� czas skoku po wybiciu z rampy 

    dt   = 0.0;                                  // czas skoku w powietrzu dt = aktualny_czas - poczatek-skoku     
    alfa = 0.0;                                  // brak wybicia skoczka w g�r�, czyli k�t 0 stopni
    mass = player->getWeight();                 // mass skoczka
    Cd = 0.0;                                    // wsp�czynnik oporu powietrza                                                                                   
    Cw = 0.0;                                    // wsp�czynnik wiatru  
    Vw = 3.0;                                    // pr�dko�� wiatru - STABLE 3 m/s; FICKLE 1-3 m/s (losowa funkcja 1-4 m/s)
                                                        // double GammaW = 0.0; kierunek wiatru - k�t w stosunku do skoczka
                                                        // 0 stopni to idea� (RAND -45/+45 stopni)
    Vm = physics->getVelocity();                 // pr�dko�� skoczka na progu !     

    physics->getJumpStart() ? alfa = 30.0 : alfa = 0.0;  // Gdy wybicie z progu - to RZUT UKO�NY, nie to POZIOMY i k�t r�wny 0 !   
                                                        // wz�r jest ten sam, ale istotny jest 'k�t alfa' kt�ry decyduje o odleg�o�ci
    double Vx = Vm * cos(alfa * physics->Deg2Rad);      // obliczone wsp�rz�dne x 
    double Vy = Vm * sin(alfa * physics->Deg2Rad);      //    -//-       -//-    y
    g  = 9.81;                                   // przy�pieszenie ziemskie  
    Sx1 = 310.0;                                 // punkt wsp�rz�dnej x - miejsce wybicia z rampy skoczni 
    Sy1 = 220.0;                                 // punkt wsp�rz�dnej y - miejsce wybicia z rampy skoczni
    SX = 0.0, SY = 0.0;                          // wsp�rz�dne x,y po przeliczeniu   

    Uint32 czas_lotu  = 0;
    bool chce_ladowac = false;

    JumpPhase jump = JumpPhase::JUMP;

    SDL_Event e;

    switch (player->getWeather()) // "rain", "sun",  "snow", "wind"
    {
        case 0: Cd = physics->Rain; break;
        case 1: Cd = physics->Sun;  break;
        case 2: Cd = physics->Snow; break;
        case 3: Cd = physics->Wind; break;
    }

    while (jump == JumpPhase::JUMP)
    {
        frameStart = SDL_GetTicks();

        SDL_RenderCopy(ren, ramp, NULL, NULL);

        SDL_RenderCopyEx(ren, banner, NULL, &rectBanner, 0.0, NULL, SDL_FLIP_NONE);
        particle->draw();
        ShowDashboard();

        SDL_RenderCopyEx(ren, windTable, NULL, &rectWind, 0.0, NULL, SDL_FLIP_NONE);
        SDL_RenderCopyEx(ren, windArrow, NULL, &rectArrow, physics->getGammaW(), NULL, SDL_FLIP_NONE);

        player->getWind() ? Vw = physics->randWind(1.0, 4.0) : Vw = 3.0;        

        dt = (SDL_GetTicks() - poczatek_skoku) * 0.001;   

        SX = ((mass / Cd) * exp(-(Cd * dt) / mass) * ((-Cw * Vw * cos(physics->getGammaW() * physics->Deg2Rad)) / Cd - Vx)
            - (Cw * Vw * cos(physics->getGammaW() * physics->Deg2Rad) * dt) / Cd)
            - ((mass / Cd) * ((-Cw * Vw * cos(physics->getGammaW() * physics->Deg2Rad)) / Cd - Vx)) + Sx1;

        SY = Sy1 + (-(Vy + (mass * g) / Cd) * (mass / Cd) * exp(-(Cd * dt) / mass)
            - (mass * g * dt) / Cd) + ((mass / Cd) * (Vy + (mass * g) / Cd));

        rectStartJump.x = int(SX);
        rectStartJump.y = int(440 - SY);

        if (physics->checkCollision(surfaceRamp, rectStartJump.x, rectStartJump.y + 6))
        {
            jump = JumpPhase::TELEMARK;    // skoczek wyladowal
            int score = (int)sqrt((rectStartJump.x - 310)*(rectStartJump.x - 310) + (rectStartJump.y - 220)*(rectStartJump.y - 220));
            player->setScore(score);
        }

        if (chce_ladowac)
            SDL_RenderCopyEx(ren, jumperToLanding, NULL, &rectStartJump, 0.0, NULL, SDL_FLIP_NONE);
        else
        {
            czas_lotu = SDL_GetTicks() - poczatek_skoku;

            if (czas_lotu < 600)
                SDL_RenderCopyEx(ren, jumperStartingJump, NULL, &rectStartJump, 0.0, NULL, SDL_FLIP_NONE);

            if (600 < czas_lotu && czas_lotu < 1500)
                SDL_RenderCopyEx(ren, jumperLeaning, NULL, &rectStartJump, 0.0, NULL, SDL_FLIP_NONE);

            if (czas_lotu >= 1500)
                SDL_RenderCopyEx(ren, jumperSoars, NULL, &rectStartJump, 0.0, NULL, SDL_FLIP_NONE);
        }

        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT) jump = JumpPhase::END;
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym)
                {
                case SDLK_RETURN: chce_ladowac = true;
                    break;
                case SDLK_ESCAPE: jump = JumpPhase::END;
                    break;
                }
            }
        }

        SDL_RenderPresent(ren);

        frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }
    return jump;
}

JumpPhase Jump::Slide()
{
    double angle_skoczek = 30.0;
    double angle_tor     = 0.0;

    Uint32 startTime = SDL_GetTicks();

    JumpPhase skok  = JumpPhase::SLIDE;

    SDL_Event e;

    while (skok == JumpPhase::SLIDE)
    {
        frameStart = SDL_GetTicks();

        SDL_RenderCopy(ren, ramp, NULL, NULL);

        SDL_RenderCopyEx(ren, banner, NULL, &rectBanner, 0.0, NULL, SDL_FLIP_NONE);
        ShowDashboard();

        SDL_RenderCopyEx(ren, jumperOnRamp, NULL, &rectJumper, angle_skoczek, NULL, SDL_FLIP_NONE);
        particle->draw();
        SDL_RenderCopyEx(ren, windTable, NULL, &rectWind, 0.0, NULL, SDL_FLIP_NONE);
        SDL_RenderCopyEx(ren, windArrow, NULL, &rectArrow, physics->getGammaW(), NULL, SDL_FLIP_NONE);
        SDL_RenderPresent(ren);

        if (rectJumper.x < 130)
            angle_tor = 44.8f;
        else
            angle_tor = 37.0f;

        if (rectJumper.x > 210) 
        {
            angle_tor = 26.8f; angle_skoczek = 10.0f;
        }

        if (rectJumper.x > 270) { angle_tor = 5.8f; }

        rectJumper.x += int(physics->roadOnRamp(startTime) * (cos(angle_tor * physics->Deg2Rad)));
        rectJumper.y += int(physics->roadOnRamp(startTime) * (sin(angle_tor * physics->Deg2Rad)));

        if (rectJumper.x > 330)
        {
            skok = JumpPhase::JUMP;
        }

        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT) skok = JumpPhase::END;
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym)
                {
                case SDLK_RETURN:
                {
                    physics->setJumpStart((rectJumper.x > 300) ? true : false);

                    skok = JumpPhase::JUMP;
                } break;
                case SDLK_ESCAPE: skok = JumpPhase::END;
                    break;
                }
            }
        }

        frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }
    return skok;
};

void Jump::SetRender(SDL_Renderer* r)
{
    ren = r;
};