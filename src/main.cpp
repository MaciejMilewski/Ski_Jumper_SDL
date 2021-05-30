#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Texture_Manager.h"
#include "Button.h"


enum class Status { MENU, START, SCORE, INFO, CONFIG, EXIT };

namespace Global 
{
    const int  SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    SDL_Window* window      = NULL;
    SDL_Renderer* renderer  = NULL;
    
    SDL_Event input;

    Status status = Status::MENU;   
}

void Loading_Resourses();
void Release_Resourses();

void Splash();

Status Menu();
Status Start();
Status Scoreboard();
Status Info();
Status Config();

int main(int argc, char* argv[]) ////// Początek i podłączenie kolejnych bibliotek /////
{
    try {        
       
        if (SDL_Init(SDL_INIT_EVERYTHING)) {
            throw SDL_GetError();
            return 1;
        }

        if (!IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG)) {
            throw IMG_GetError();
            return 2;
        }

        if (TTF_Init() == -1) {
            throw TTF_GetError();
            return 3;
        }

        Global::window = SDL_CreateWindow("Skoki narciarskie", SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, Global::SCREEN_WIDTH, Global::SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

        if (!Global::window) {
            throw SDL_GetError();
            return 4;
        }

        Global::renderer = SDL_CreateRenderer(Global::window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
        
        if (Global::renderer == NULL) {
            SDL_GetError();
            return 5;
        }
                
        Loading_Resourses();        

        Splash();       
        
        
        // ------------------------------------------------------------------------------------------------------- //

        while (Global::status != Status::EXIT)
        {
            SDL_SetRenderDrawColor(Global::renderer, 0, 0, 0, 0);
            SDL_RenderClear(Global::renderer);

            while (SDL_PollEvent(&Global::input) > 0)
            {
                if (Global::input.type == SDL_QUIT) Global::status = Status::EXIT;
            }                        
            
            switch (Global::status) {
                case Status::MENU      : Menu();       break;   //  Wywołanie procedur odpowiednio do wykonywanej funkcji
                case Status::START     : Start();      break;   //  każda procedura zwraca status:: który kieruje wywołania 
                case Status::SCORE     : Scoreboard(); break;   //  do kolejnych procedur lub klass, ustawienie zmiennej
                case Status::INFO      : Info();       break;   // 'Global::Status = Status::Exit;' kończy program ... .
                case Status::CONFIG    : Config();     break;
                default: Menu();
            }                

            // SDL_RenderPresent(Global::renderer);
        }        

        Release_Resourses();
        
    } catch (const char* msg)
    {
        std::cerr << " Awaria : " << msg << '\n';
    }
    
  return 0;  
}

// ------------------------------------------------------------------------------------------------------- //

void Splash()   //  pierwszy ekran  
{
    TextureManager::Instance()->Draw("splash", 0, 0, Global::SCREEN_WIDTH, Global::SCREEN_HEIGHT, 1, 0, Global::renderer);
   
    SDL_Surface* app_ico = IMG_Load("img/ski_Icon.jpg");        // Ikona dla aplikacji 

    if (!app_ico)
    {
        throw SDL_GetError();
    }

    SDL_SetWindowIcon(Global::window, app_ico);

    SDL_FreeSurface(app_ico);

    SDL_RenderPresent(Global::renderer);                        // wyświetla bufor na ekranie

    SDL_Delay(3000);                                            // Opóźnienie 3 sekundy 

    TextureManager::Instance()->clearFromTextureMap("splash");  // usuwa 'splash' z pamięci kontenera map
}

Status Menu()   // zarządza Menu Głównym
{    
    SDL_WarpMouseGlobal(400, 400);

    TextureManager::Instance()->Draw("menu", 0, 0, Global::SCREEN_WIDTH, Global::SCREEN_HEIGHT, 1, 0, Global::renderer);

    Button* buttonInfos = new Button("img/help_button.png",  "img/helpS_button.png",  100, 52,  50, 530, *Global::renderer);
    Button* buttonStart = new Button("img/Start_button.png", "img/StartS_button.png", 100, 52, 210, 530, *Global::renderer);
    Button* buttonStore = new Button("img/score_button.png", "img/scoreS_button.png", 130, 60, 410, 530, *Global::renderer);
    Button* buttonExits = new Button("img/Exits_button.png", "img/ExitsS_button.png", 100, 52, 610, 530, *Global::renderer);   

    SDL_Delay(500);

    while (Global::status == Status::MENU)
    {
        while (SDL_PollEvent(&Global::input) > 0)
        {
            if (Global::input.type == SDL_QUIT) Global::status = Status::EXIT;
        }
        
        buttonInfos->Clicked(Global::input);
        buttonInfos->ShowButton();
        buttonStart->Clicked(Global::input);
        buttonStart->ShowButton();
        buttonStore->Clicked(Global::input);
        buttonStore->ShowButton();
        buttonExits->Clicked(Global::input);
        buttonExits->ShowButton();

        if (tryb::selected == buttonInfos->Clicked(Global::input))
            Global::status = Status::INFO;
        if (tryb::selected == buttonStart->Clicked(Global::input))
            Global::status = Status::CONFIG;
        if (tryb::selected == buttonStore->Clicked(Global::input))
            Global::status = Status::SCORE;
        if (tryb::selected == buttonExits->Clicked(Global::input))
            Global::status = Status::EXIT;

        SDL_RenderPresent(Global::renderer);
    }   

    delete buttonInfos;
    delete buttonStart;
    delete buttonStore;
    delete buttonExits;

    return Global::status;
}
Status Start() // zarządza START - under constraction !
{
    SDL_WarpMouseGlobal(400, 400);
    
    TextureManager::Instance()->Draw("start", 0, 0, Global::SCREEN_WIDTH, Global::SCREEN_HEIGHT, 1, 0, Global::renderer);

    Button* buttonMenu = new Button("img/back_button.png", "img/backS_button.png", 100, 52, 50, 530, *Global::renderer);   

    SDL_Delay(500);

    while (Global::status == Status::START)
    {
        while (SDL_PollEvent(&Global::input) > 0)
        {
            if (Global::input.type == SDL_QUIT) Global::status = Status::EXIT;
        }

        buttonMenu->Clicked(Global::input);
        buttonMenu->ShowButton();

        if (tryb::selected == buttonMenu->Clicked(Global::input))
            Global::status = Status::MENU;

        SDL_RenderPresent(Global::renderer);
    }

    delete buttonMenu;

    return Global::status;
}

Status Scoreboard() // zarządza SCOREBOARD 
{
    SDL_WarpMouseGlobal(400, 400);
    
    TextureManager::Instance()->Draw("score", 0, 0, Global::SCREEN_WIDTH, Global::SCREEN_HEIGHT, 1, 0, Global::renderer);

    Button* buttonScore = new Button("img/back_button.png", "img/backS_button.png", 100, 52, 40, 535, *Global::renderer); 

    SDL_Delay(500);

    while (Global::status == Status::SCORE)
    {
        while (SDL_PollEvent(&Global::input) > 0)
        {
            if (Global::input.type == SDL_QUIT) Global::status = Status::EXIT;
        }

        buttonScore->Clicked(Global::input);
        buttonScore->ShowButton();

        if (tryb::selected == buttonScore->Clicked(Global::input))
            Global::status = Status::MENU;

        SDL_RenderPresent(Global::renderer);
    }

    delete buttonScore;
    
    return Global::status;
}

Status Info() // zarządza ekranem INFO
{
    TextureManager::Instance()->Draw("info", 0, 0, Global::SCREEN_WIDTH, Global::SCREEN_HEIGHT, 1, 0, Global::renderer);

    Button* buttonInfo = new Button("img/back_button.png", "img/backS_button.png", 90, 52, 40, 540, *Global::renderer);

    SDL_Delay(500);
    
    while (Global::status == Status::INFO)
    {
        while (SDL_PollEvent(&Global::input) > 0)
        {
            if (Global::input.type == SDL_QUIT) Global::status = Status::EXIT;
        }

        buttonInfo->Clicked(Global::input);
        buttonInfo->ShowButton();

        if (tryb::selected == buttonInfo->Clicked(Global::input))
            Global::status = Status::MENU;

        SDL_RenderPresent(Global::renderer);
    }

    delete buttonInfo;

    return Global::status;
}
Status Config() 
{    
    TextureManager::Instance()->Draw("config", 0, 0, Global::SCREEN_WIDTH, Global::SCREEN_HEIGHT, 1, 0, Global::renderer);

    Button* buttonBack  = new Button("img/back_button.png",  "img/backS_button.png" , 100, 52, 250, 540, *Global::renderer);    
    Button* buttonStart = new Button("img/Start_button.png", "img/StartS_button.png", 100, 52, 410, 540, *Global::renderer);  

    SDL_Delay(500);

    while (Global::status == Status::CONFIG)
    {
        while (SDL_PollEvent(&Global::input) > 0)
        {
            if (Global::input.type == SDL_QUIT) Global::status = Status::EXIT;
        }
        
        buttonBack ->Clicked(Global::input);
        buttonBack ->ShowButton();        
        buttonStart->Clicked(Global::input);
        buttonStart->ShowButton();

        if (tryb::selected == buttonBack->Clicked(Global::input))
            Global::status = Status::MENU;
        if (tryb::selected == buttonStart->Clicked(Global::input))
            Global::status = Status::START;

        SDL_RenderPresent(Global::renderer);
    }

    delete buttonBack;
    delete buttonStart;

    return Global::status;
}

void Loading_Resourses()  // wczytuje pliki graficzne do Texture Managera, sprawdza obecność wszystkich obrazków - OK ?
{
    TextureManager::Instance()->Load("img/Splash.png"  , "splash", Global::renderer);
    TextureManager::Instance()->Load("img/Menu.png"    , "menu"  , Global::renderer);
    TextureManager::Instance()->Load("img/bilboard.jpg", "score" , Global::renderer);
    TextureManager::Instance()->Load("img/Info.png"    , "info"  , Global::renderer);
    TextureManager::Instance()->Load("img/under_con.png","start" , Global::renderer);
    TextureManager::Instance()->Load("img/Configure.png","config", Global::renderer);
}

void Release_Resourses()  // zwalnianie zasobów w odwrotnym porządku do ich tworzenia
{
    SDL_DestroyRenderer(Global::renderer);
    SDL_DestroyWindow(Global::window);    

    TextureManager::Instance()->DestroyInstance();  // likwiduje singletona TextureManager

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();   
}