#include <iostream>

#include "Jump.h"

Jump::Jump(SDL_Renderer* render, Physics* physics, Player* player): ren(render), physics(physics), player(player)
{
    rampa = IMG_LoadTexture(ren,"img/rampa.png");    

    surfaceRamp = IMG_Load("img/rampa.png");    

        SDL_Surface* loadSurface = IMG_Load("img/rozbieg.png");

        SDL_SetColorKey(loadSurface, SDL_TRUE, SDL_MapRGB(loadSurface->format, 255, 255, 255));

    jumper_na_rampie = SDL_CreateTextureFromSurface(ren, loadSurface);

        SDL_FreeSurface(loadSurface);

        SDL_Surface* loadWybicie = IMG_Load("img/rozbieg.png");

        SDL_SetColorKey(loadWybicie, SDL_TRUE, SDL_MapRGB(loadWybicie->format, 255, 255, 255)); 

    jumper_wybicie = SDL_CreateTextureFromSurface(ren, loadWybicie);

        SDL_FreeSurface(loadWybicie);

        SDL_Surface* loadPochylony = IMG_Load("img/lot_pochylony.png");

        SDL_SetColorKey(loadPochylony, SDL_TRUE, SDL_MapRGB(loadPochylony->format, 255, 255, 255));

    jumper_pochylony = SDL_CreateTextureFromSurface(ren, loadPochylony);

        SDL_FreeSurface(loadPochylony);

        SDL_Surface* loadSzybuje = IMG_Load("img/lot_szeroko.png");

        SDL_SetColorKey(loadSzybuje, SDL_TRUE, SDL_MapRGB(loadSzybuje->format, 255, 255, 255));

    jumper_szybuje = SDL_CreateTextureFromSurface(ren, loadSzybuje);

        SDL_FreeSurface(loadSzybuje);

        SDL_Surface* loadLaduje = IMG_Load("img/do_ladowania.png");

        SDL_SetColorKey(loadLaduje, SDL_TRUE, SDL_MapRGB(loadLaduje->format, 255, 255, 255));

    jumper_do_ladowania = SDL_CreateTextureFromSurface(ren, loadLaduje);

        SDL_FreeSurface(loadLaduje);

        SDL_Surface* loadTelemark = IMG_Load("img/telemark.png");

        SDL_SetColorKey(loadTelemark, SDL_TRUE, SDL_MapRGB(loadTelemark->format, 255, 255, 255));

    jumper_telemark = SDL_CreateTextureFromSurface(ren, loadTelemark);

        SDL_FreeSurface(loadTelemark);

        SDL_Surface* ladujSurface = IMG_Load("img/Banner.png");

        SDL_SetColorKey(ladujSurface, SDL_TRUE, SDL_MapRGB(ladujSurface->format, 255, 255, 255));

    banner = SDL_CreateTextureFromSurface(ren, ladujSurface);

        SDL_FreeSurface(ladujSurface);

    wind_arrow = IMG_LoadTexture(ren,"img/arrow-wind.png");
    wind_table = IMG_LoadTexture(ren,"img/wind-table.png");
};

Jump::~Jump()
{
    SDL_DestroyTexture(rampa);
    SDL_DestroyTexture(jumper_na_rampie);
    SDL_DestroyTexture(jumper_wybicie);
    SDL_DestroyTexture(jumper_pochylony);
    SDL_DestroyTexture(jumper_szybuje);
    SDL_DestroyTexture(jumper_do_ladowania);
    SDL_DestroyTexture(jumper_telemark);
    SDL_FreeSurface(surfaceRamp);
    SDL_DestroyTexture(banner);
    SDL_DestroyTexture(wind_arrow);
    SDL_DestroyTexture(wind_table);

    rampa = nullptr;
    jumper_na_rampie = nullptr;
    jumper_wybicie = nullptr;
    jumper_pochylony = nullptr;
    jumper_szybuje = nullptr;
    jumper_do_ladowania = nullptr;
    jumper_telemark = nullptr;
    banner = nullptr;
    wind_arrow = nullptr;
    wind_table = nullptr;
    surfaceRamp = nullptr;

    physics = nullptr; 
    player = nullptr;
    ren = nullptr;
};
Fazy_skoku Jump::Czeka()
{
    rect_skoczek = { 88,  85,  38,  38 };
    rect_wyskok  = { 310, 220,  38, 38 };

    SDL_RenderCopy(ren, rampa, NULL, NULL);
    SDL_RenderCopy(ren, jumper_na_rampie, NULL, &rect_skoczek);
    SDL_RenderCopyEx(ren, wind_table, NULL, &rect_wind, 0.0, NULL, SDL_FLIP_NONE);
    SDL_RenderCopyEx(ren, wind_arrow, NULL, &rect_arrow, physics->get_GammaW(), NULL, SDL_FLIP_NONE);

    return Fazy_skoku::CZEKA;
}

Fazy_skoku Jump::Landing()
{
    double angle_skoczek = 0.0;
    double angle_tor     = 0.0;

    Uint32 startTime = SDL_GetTicks();

    Fazy_skoku skok = Fazy_skoku::TELEMARK;

    SDL_Event e;

    while (skok == Fazy_skoku::TELEMARK)
    {
        frameStart = SDL_GetTicks();

        SDL_RenderCopy(ren, rampa, NULL, NULL);
        SDL_RenderCopyEx(ren, banner, NULL, &rect_banner, 0.0, NULL, SDL_FLIP_NONE);
        SDL_RenderCopyEx(ren, jumper_telemark, NULL, &rect_wyskok, angle_skoczek, NULL, SDL_FLIP_NONE);
        SDL_RenderCopyEx(ren, wind_table, NULL, &rect_wind, 0.0, NULL, SDL_FLIP_NONE);
        SDL_RenderCopyEx(ren, wind_arrow, NULL, &rect_arrow, physics->get_GammaW(), NULL, SDL_FLIP_NONE);
        SDL_RenderPresent(ren);

        rect_wyskok.x += int(physics->Road_Ramp(startTime) * (cos(angle_tor * physics->Deg2Rad)));
        rect_wyskok.y += int(physics->Road_Ramp(startTime) * (sin(angle_tor * physics->Deg2Rad)));

        while (!physics->check_Collision(surfaceRamp, rect_wyskok.x, rect_wyskok.y))
        {
            rect_wyskok.y++;
        }

        if (rect_wyskok.x > 515)
        {
            angle_skoczek = -35.0f; rect_wyskok.y -= 18;
        }

        if (rect_wyskok.x > 720)
        {
            skok = Fazy_skoku::CZEKA;
        }

        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT) skok = Fazy_skoku::END;
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym)
                {
                case SDLK_RETURN:
                    break;
                case SDLK_ESCAPE: skok = Fazy_skoku::END;
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

    return Fazy_skoku::END;
};

Fazy_skoku Jump::Wyskok()
{
    Uint32 poczatek_skoku = SDL_GetTicks();             // Zaczynamy liczyæ czas skoku po wybiciu z rampy 

    double dt   = 0.0;                                  // czas skoku w powietrzu dt = aktualny_czas - poczatek-skoku     
    double alfa = 0.0;                                  // brak wybicia skoczka w górê, czyli k¹t 0 stopni
    double masa = player->get_Weight();                 // masa skoczka
    double Cd = 0.0;                                    // wspó³czynnik oporu powietrza                                                                                   
    double Cw = 0.0;                                    // wspó³czynnik wiatru  
    double Vw = 3.0;                                    // prêdkoœæ wiatru - STABLE 3 m/s; FICKLE 1-3 m/s (losowa funkcja 1-4 m/s)
                                                        // double GammaW = 0.0; kierunek wiatru - k¹t w stosunku do skoczka
                                                        // 0 stopni to idea³ (RAND -45/+45 stopni)
    double Vm = physics->getVelocity();                 // prêdkoœæ skoczka na progu !     

    physics->get_Wybicie() ? alfa = 30.0 : alfa = 0.0;  // Gdy wybicie z progu - to RZUT UKOŒNY, nie to POZIOMY i k¹t równy 0 !   
                                                        // wzór jest ten sam, ale istotny jest 'k¹t alfa' który decyduje o odleg³oœci
    double Vx = Vm * cos(alfa * physics->Deg2Rad);      // obliczone wspó³rzêdne x 
    double Vy = Vm * sin(alfa * physics->Deg2Rad);      //    -//-       -//-    y
    double g  = 9.81;                                   // przyœpieszenie ziemskie  
    double Sx1 = 310.0;                                 // punkt wspó³rzêdnej x - miejsce wybicia z rampy skoczni 
    double Sy1 = 220.0;                                 // punkt wspó³rzêdnej y - miejsce wybicia z rampy skoczni
    double SX = 0.0, SY = 0.0;                          // wspó³rzêdne x,y po przeliczeniu   

    Uint32 czas_lotu  = 0;
    bool chce_ladowac = false;

    Fazy_skoku skok = Fazy_skoku::SKOK;

    SDL_Event e;

    switch (player->get_Weather()) // "rain", "sun",  "snow", "wind"
    {
        case 0: Cd = physics->Rain; break;
        case 1: Cd = physics->Sun;  break;
        case 2: Cd = physics->Snow; break;
        case 3: Cd = physics->Wind; break;
    }

    while (skok == Fazy_skoku::SKOK)
    {
        frameStart = SDL_GetTicks();

        SDL_RenderCopy(ren, rampa, NULL, NULL);
        SDL_RenderCopyEx(ren, wind_table, NULL, &rect_wind, 0.0, NULL, SDL_FLIP_NONE);
        SDL_RenderCopyEx(ren, wind_arrow, NULL, &rect_arrow, physics->get_GammaW(), NULL, SDL_FLIP_NONE);

        player->get_Wind() ? Vw = physics->rand_Wind(1.0, 4.0) : Vw = 3.0;        

        dt = (SDL_GetTicks() - poczatek_skoku) * 0.001;   

        SX = ((masa / Cd) * exp(-(Cd * dt) / masa) * ((-Cw * Vw * cos(physics->get_GammaW() * physics->Deg2Rad)) / Cd - Vx)
            - (Cw * Vw * cos(physics->get_GammaW() * physics->Deg2Rad) * dt) / Cd)
            - ((masa / Cd) * ((-Cw * Vw * cos(physics->get_GammaW() * physics->Deg2Rad)) / Cd - Vx)) + Sx1;

        SY = Sy1 + (-(Vy + (masa * g) / Cd) * (masa / Cd) * exp(-(Cd * dt) / masa)
            - (masa * g * dt) / Cd) + ((masa / Cd) * (Vy + (masa * g) / Cd));

        rect_wyskok.x = int(SX);
        rect_wyskok.y = int(440 - SY);

        if (physics->check_Collision(surfaceRamp, rect_wyskok.x, rect_wyskok.y + 6))
        {
            skok = Fazy_skoku::TELEMARK;    // skoczek wyl¹dowa³
        }

        if (chce_ladowac)
            SDL_RenderCopyEx(ren, jumper_do_ladowania, NULL, &rect_wyskok, 0.0, NULL, SDL_FLIP_NONE);
        else
        {
            czas_lotu = SDL_GetTicks() - poczatek_skoku;

            if (czas_lotu < 600)
                SDL_RenderCopyEx(ren, jumper_wybicie, NULL, &rect_wyskok, 0.0, NULL, SDL_FLIP_NONE);

            if (600 < czas_lotu && czas_lotu < 1500)
                SDL_RenderCopyEx(ren, jumper_pochylony, NULL, &rect_wyskok, 0.0, NULL, SDL_FLIP_NONE);

            if (czas_lotu >= 1500)
                SDL_RenderCopyEx(ren, jumper_szybuje, NULL, &rect_wyskok, 0.0, NULL, SDL_FLIP_NONE);
        }

        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT) skok = Fazy_skoku::END;
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym)
                {
                case SDLK_RETURN: chce_ladowac = true;
                    break;
                case SDLK_ESCAPE: skok = Fazy_skoku::END;
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
    return skok;
}

Fazy_skoku Jump::Zjazd()
{
    double angle_skoczek = 30.0;
    double angle_tor     = 0.0;

    Uint32 startTime = SDL_GetTicks();

    Fazy_skoku skok  = Fazy_skoku::ZJAZD;

    SDL_Event e;

    while (skok == Fazy_skoku::ZJAZD)
    {
        frameStart = SDL_GetTicks();

        SDL_RenderCopy(ren, rampa, NULL, NULL);
        SDL_RenderCopyEx(ren, banner, NULL, &rect_banner, 0.0, NULL, SDL_FLIP_NONE);
        SDL_RenderCopyEx(ren, jumper_na_rampie, NULL, &rect_skoczek, angle_skoczek, NULL, SDL_FLIP_NONE);
        SDL_RenderCopyEx(ren, wind_table, NULL, &rect_wind, 0.0, NULL, SDL_FLIP_NONE);
        SDL_RenderCopyEx(ren, wind_arrow, NULL, &rect_arrow, physics->get_GammaW(), NULL, SDL_FLIP_NONE);
        SDL_RenderPresent(ren);

        if (rect_skoczek.x < 130)
            angle_tor = 44.8f;
        else
            angle_tor = 37.0f;

        if (rect_skoczek.x > 210) 
        {
            angle_tor = 26.8f; angle_skoczek = 10.0f;
        }

        if (rect_skoczek.x > 270) { angle_tor = 5.8f; }

        rect_skoczek.x += int(physics->Road_Ramp(startTime) * (cos(angle_tor * physics->Deg2Rad)));
        rect_skoczek.y += int(physics->Road_Ramp(startTime) * (sin(angle_tor * physics->Deg2Rad)));

        if (rect_skoczek.x > 330)
        {
            skok = Fazy_skoku::SKOK;
        }

        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT) skok = Fazy_skoku::END;
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym)
                {
                case SDLK_RETURN:
                {
                    physics->set_Wybicie((rect_skoczek.x > 300) ? true : false);

                    skok = Fazy_skoku::SKOK;
                } break;
                case SDLK_ESCAPE: skok = Fazy_skoku::END;
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
void Jump::set_Render(SDL_Renderer* r)
{
    ren = r;
};