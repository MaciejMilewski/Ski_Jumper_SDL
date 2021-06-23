#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "TextureManager.h"
#include "Button.h"
#include "CheckBox.h"
#include "RollBox.h"
#include "Player.h"
#include "Physics.h"
#include "Jump.h"
#include "Label.h"

enum class Status { MENU, START, SCORE, INFO, CONFIG, EXIT };

namespace Global
{
    const int  SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;
    const int   NAME_LENGTH = 8;

    SDL_Window* window      = nullptr;
    SDL_Renderer* renderer  = nullptr;
    SDL_Event input;  
    Status status = Status::MENU;
    Player* player = new Player;
    Physics* physics = new Physics;

    const int FPS = 24;
    const int frameDelay = 1000 / FPS;
    Uint32    frameStart;
    int       frameTime;
}
    void LoadingResources();
    void ReleseResources();
    void Splash();

    Status Menu();
    Status Start();
    Status Scoreboard();
    Status Info();
    Status Config();

    Uint32 Callback(Uint32 interval, void* param);
      void Null();

int main(int argc, char* argv[])    
{
    srand(static_cast <unsigned int> (time(0)));
    
    try {

        if (SDL_Init(SDL_INIT_EVERYTHING)) {
            throw SDL_GetError();
            return 1;
        }

        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {	             
            throw " filtrowanie 'linear' textur nie jest ustawione ! \n";
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
            SDL_WINDOWPOS_CENTERED, Global::SCREEN_WIDTH, Global::SCREEN_HEIGHT, SDL_WINDOW_OPENGL);

        if (!Global::window) {
            throw SDL_GetError();
            return 4;
        }

        Global::renderer = SDL_CreateRenderer(Global::window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

        if (Global::renderer == NULL) {
            throw SDL_GetError();
            return 5;
        }
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        {            
          throw " brak aktywnego systemu AUDIO ...";
        }

        LoadingResources();
        Splash();

        // ------------------------ GAME LOOP ----------------------- //

        while (Global::status != Status::EXIT)
        {
            SDL_WarpMouseGlobal(600, 600);
            SDL_Delay(100);

            while (SDL_PollEvent(&Global::input) > 0)
            {
                switch (Global::input.type)
                {
                    case SDL_QUIT: Global::status = Status::EXIT;
                        break;
                }
            }

            switch (Global::status)
            {
            case Status::MENU:   Menu();       break;   
            case Status::START:  Start();      break;   
            case Status::SCORE:  Scoreboard(); break;   
            case Status::INFO:   Info();       break;   
            case Status::CONFIG: Config();     break;
            case Status::EXIT:                 break;
            }
        }

        ReleseResources();

    }
    catch (const char* msg)   
    {
        std::cerr << " Problem : " << msg << '\n';
    }

    return EXIT_SUCCESS;
}

// -------------------- END GAME LOOP ----------------------- //

void Splash()   
{
    TextureManager::Instance()->Draw("splash", 0, 0, Global::SCREEN_WIDTH, Global::SCREEN_HEIGHT, 1, 0, Global::renderer);

    SDL_Surface* appIcon = IMG_Load("img/ski_Icon.jpg");  
    if (!appIcon)
    {
        throw SDL_GetError();
    }

    Mix_Music* music = Mix_LoadMUS("img/mixkit-just-chill-16.wav");
    Mix_PlayMusic(music, 1); // 1 oznacza jedno odtworzenie pliku, -1 infinity

    SDL_SetWindowIcon(Global::window, appIcon);
    SDL_FreeSurface(appIcon);
    SDL_RenderPresent(Global::renderer);
    SDL_Delay(5000);
    Mix_FreeMusic(music);

    TextureManager::Instance()->clearFromTextureMap("splash");  
}

Status Menu() 
{
    TextureManager::Instance()->Draw("menu", 0, 0, Global::SCREEN_WIDTH, Global::SCREEN_HEIGHT, 1.0, 0, Global::renderer);

    Button* buttonInfos = new Button("img/help_button.png", "img/helpS_button.png", 100, 52, 50, 530, *Global::renderer);
    Button* buttonStart = new Button("img/Start_button.png", "img/StartS_button.png", 100, 52, 210, 530, *Global::renderer);
    Button* buttonStore = new Button("img/score_button.png", "img/scoreS_button.png", 130, 60, 410, 530, *Global::renderer);
    Button* buttonExits = new Button("img/Exits_button.png", "img/ExitsS_button.png", 100, 52, 610, 530, *Global::renderer);


    while (Global::status == Status::MENU)
    {
        Global::frameStart = SDL_GetTicks();                        

        while (SDL_PollEvent(&Global::input) > 0)
        {
            switch (Global::input.type) 
            {
                case SDL_QUIT: Global::status = Status::EXIT;
                    break;
            }
        }

        buttonInfos->Clicked(Global::input);
        buttonInfos->ShowButton();
        buttonStart->Clicked(Global::input);
        buttonStart->ShowButton();
        buttonStore->Clicked(Global::input);
        buttonStore->ShowButton();
        buttonExits->Clicked(Global::input);
        buttonExits->ShowButton();

        if (Mode::selected == buttonInfos->Clicked(Global::input))
            Global::status = Status::INFO;
        if (Mode::selected == buttonStart->Clicked(Global::input))
            Global::status = Status::CONFIG;
        if (Mode::selected == buttonStore->Clicked(Global::input))
            Global::status = Status::SCORE;
        if (Mode::selected == buttonExits->Clicked(Global::input))
            Global::status = Status::EXIT;

        SDL_RenderPresent(Global::renderer);

        Global::frameTime = SDL_GetTicks() - Global::frameStart;   

        if (Global::frameDelay > Global::frameTime)                
        {
            SDL_Delay(Global::frameDelay - Global::frameTime);
        }
    }

    delete buttonInfos;
    delete buttonStart;
    delete buttonStore;
    delete buttonExits;

    return Global::status;
}

Status Start() 
{
    SDL_TimerID timerID = SDL_AddTimer(1500, Callback, (void*)Null);
    Jump jump(Global::renderer, Global::physics, Global::player);
    JumpPhase skok = JumpPhase::WAIT;      
    SDL_Event event;
    Global::status  = Status::SCORE;

    /* SKI JUMP LOOP */
    while (skok != JumpPhase::END)
    {
        jump.frameStart = SDL_GetTicks();

        switch (skok)
        {
        case JumpPhase::WAIT:     skok = jump.Wait();
            break;
        case JumpPhase::SLIDE:    skok = jump.Slide();
            break;
        case JumpPhase::JUMP:     skok = jump.Flight();
            break;
        case JumpPhase::TELEMARK: skok = jump.Landing();
            break;
        case JumpPhase::END:
            break;
        }

        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT) skok = JumpPhase::END;
            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_RETURN: skok = JumpPhase::SLIDE; break;               
                }
            }
        }       

        SDL_RenderPresent(Global::renderer);

        jump.frameTime = SDL_GetTicks() - jump.frameStart;

        if (jump.frameDelay > jump.frameTime)
        {
            SDL_Delay(jump.frameDelay - jump.frameTime);
        }
    }
    /* SKI JUMP LOOP END */    

    SDL_RemoveTimer(timerID);    

    return Global::status;
}

Status Scoreboard() 
{
    TextureManager::Instance()->Draw("score", 0, 0, Global::SCREEN_WIDTH, Global::SCREEN_HEIGHT, 1.0, 0, Global::renderer);
    Button* buttonScore = new Button("img/back_button.png", "img/backS_button.png", 100, 52, 40, 535, *Global::renderer);

    if(Global::player->getName() != "")
    {
        Label playerNameLabel(Global::player->getName().c_str(), 24, 410, 250, Global::renderer);
        playerNameLabel.Show();

        std::string scoreStr = std::to_string(Global::player->getScore());
        Label playerScoreLabel(scoreStr.c_str(), 24, 620, 250, Global::renderer);
        playerScoreLabel.Show();
    }


    while (Global::status == Status::SCORE)
    {
        Global::frameStart = SDL_GetTicks();                        

        while (SDL_PollEvent(&Global::input) > 0)
        {
            switch (Global::input.type) 
            {
                case SDL_QUIT: Global::status = Status::EXIT;
                    break;
            }
        }

        buttonScore->Clicked(Global::input);
        buttonScore->ShowButton();

        if (Mode::selected == buttonScore->Clicked(Global::input))
            Global::status = Status::MENU;

        SDL_RenderPresent(Global::renderer);

        Global::frameTime = SDL_GetTicks() - Global::frameStart;      

        if (Global::frameDelay > Global::frameTime)                   
        {
            SDL_Delay(Global::frameDelay - Global::frameTime);
        }
    }

    delete buttonScore;

    return Global::status;
}

Status Info() 
{
    TextureManager::Instance()->Draw("info", 0, 0, Global::SCREEN_WIDTH, Global::SCREEN_HEIGHT, 1.0, 0, Global::renderer);

    Button* buttonInfo = new Button("img/back_button.png", "img/backS_button.png", 90, 52, 40, 540, *Global::renderer);


    while (Global::status == Status::INFO)
    {
        Global::frameStart = SDL_GetTicks();                        

        while (SDL_PollEvent(&Global::input) > 0)
        {
            switch (Global::input.type) 
            {
                case SDL_QUIT: Global::status = Status::EXIT;
                    break;
            }
        }

        buttonInfo->Clicked(Global::input);
        buttonInfo->ShowButton();

        if (Mode::selected == buttonInfo->Clicked(Global::input))
            Global::status = Status::MENU;

        SDL_RenderPresent(Global::renderer);

        Global::frameTime = SDL_GetTicks() - Global::frameStart;      

        if (Global::frameDelay > Global::frameTime) 
        {                 
            SDL_Delay(Global::frameDelay - Global::frameTime);
        }
    }

    delete buttonInfo;

    return Global::status;
}

Status Config()
{
    TextureManager::Instance()->Draw("config", 0, 0, Global::SCREEN_WIDTH, Global::SCREEN_HEIGHT, 1.0, 0, Global::renderer);
    TextureManager::Instance()->Draw("inputBox", 235, 304, 158, 55, 1.0, 0, Global::renderer);

    Button* buttonBack  = new Button("img/back_button.png", "img/backS_button.png", 100, 52, 250, 540, *Global::renderer);
    Button* buttonStart = new Button("img/Start_button.png", "img/StartS_button.png", 100, 52, 410, 540, *Global::renderer);

    CheckBox* checkboxSmall  = new CheckBox("img/s_button.png", "img/sS_button.png", " ", "img/arial.ttf", 36, 0, 40, 224, 366, *Global::renderer);
    CheckBox* checkboxMedium = new CheckBox("img/m_button.png", "img/mS_button.png", " ", "img/arial.ttf", 36, 0, 40, 278, 366, *Global::renderer);
    CheckBox* checkboxLarge  = new CheckBox("img/L_button.png", "img/LS_button.png", " ", "img/arial.ttf", 36, 0, 42, 337, 366, *Global::renderer);
    CheckBox* checkboxWindF  = new CheckBox("img/Stable_button.png", "img/FickleS_button.png", "", "img/arial.ttf", 36, 0, 52, 526, 303, *Global::renderer);

    std::string Maps[3]    = { "skocznia-1", "skocznia-2", "skocznia-3" };
    std::string Weather[4] = { "rain", "sun",  "snow", "wind" };

    RollBox* rollBoxMaps = new RollBox(Maps, 120, 80, 240, 420, 2, Global::renderer);
    RollBox* rollBoxWeather = new RollBox(Weather, 120, 80, 535, 365, 3, Global::renderer);

    int weightCheck = 1;

    checkboxSmall->setMode(CheckboxMode::selected);

    std::string  playerName ="";
    SDL_Texture* text;
    SDL_Rect  dest;
    TTF_Font* font;
    SDL_Color colorText{ 0, 0, 240, 255 };    

    font = TTF_OpenFont("img/arial.ttf", 36);

    if (!font)
    {
        std::cerr << " Brak pliku czcionki 'arial.ttf' : " << TTF_GetError() << '\n';
    }

    SDL_StartTextInput();


    while (Global::status == Status::CONFIG)
    {
        Global::frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&Global::input) > 0)
        {
            switch (Global::input.type)
            {
                case SDL_QUIT: Global::status = Status::EXIT;
                    break;
                case SDL_TEXTINPUT:                
                    if (playerName.size() < Global::NAME_LENGTH)
                        playerName += Global::input.text.text;
                    Global::player->setName(playerName);  
                    break;
                case SDL_KEYDOWN:
                    if (Global::input.key.keysym.sym == SDLK_BACKSPACE && playerName.size())
                        playerName.pop_back();
                    if (Global::input.key.keysym.sym == SDLK_RETURN && playerName.size())
                        Global::player->setName(playerName);
                    break;
            }
        }

        rollBoxMaps->Show();
        rollBoxWeather->Show();

        buttonBack->ShowButton();
        buttonStart->ShowButton();

        checkboxSmall->ShowButton();
        checkboxSmall->Clicked(Global::input);
        checkboxMedium->ShowButton();
        checkboxMedium->Clicked(Global::input);
        checkboxLarge->ShowButton();
        checkboxLarge->Clicked(Global::input);

        checkboxWindF->ShowButton();
        checkboxWindF->Clicked(Global::input);

        rollBoxMaps->Clicked(Global::input);
        rollBoxWeather->Clicked(Global::input);

        if (Mode::selected == buttonBack->Clicked(Global::input))
            Global::status = Status::MENU;
        if (Mode::selected == buttonStart->Clicked(Global::input))
            Global::status = Status::START;

        if ((checkboxSmall->Clicked(Global::input) == CheckboxMode::selected) && (weightCheck == 1)) {
            weightCheck = 0;
        }

        if (checkboxSmall->Clicked(Global::input) == CheckboxMode::selected && weightCheck == 0) {
            weightCheck = 1;
        }
        else checkboxSmall->setMode(CheckboxMode::normal);

        if (checkboxMedium->Clicked(Global::input) == CheckboxMode::selected && weightCheck == 2) {
            weightCheck = 0;
        }
        if (checkboxMedium->Clicked(Global::input) == CheckboxMode::selected && weightCheck == 0) {
            weightCheck = 2;
        }
        else checkboxMedium->setMode(CheckboxMode::normal);

        if (checkboxLarge->Clicked(Global::input) == CheckboxMode::selected && weightCheck == 3) {
            weightCheck = 0;
        }
        if (checkboxLarge->Clicked(Global::input) == CheckboxMode::selected && weightCheck == 0) {
            weightCheck = 3;
        }
        else checkboxLarge->setMode(CheckboxMode::normal);

        if (playerName.size())
        {
            SDL_Surface* textSurface = TTF_RenderText_Blended(font, playerName.c_str(), colorText);

            text = SDL_CreateTextureFromSurface(Global::renderer, textSurface);

            dest.x = 242;
            dest.y = 308;
            dest.w = textSurface->w;
            dest.h = textSurface->h;

            TextureManager::Instance()->Draw("inputBox", 235, 304, 158, 55, 1.0, 0, Global::renderer);

            SDL_RenderCopy(Global::renderer, text, NULL, &dest);

            SDL_DestroyTexture(text);
            SDL_FreeSurface(textSurface);
        }

        SDL_RenderPresent(Global::renderer);

        Global::frameTime = SDL_GetTicks() - Global::frameStart;

        if (Global::frameDelay > Global::frameTime)
        {
            SDL_Delay(Global::frameDelay - Global::frameTime);
        }
    }

    if (checkboxSmall->Clicked(Global::input)  == CheckboxMode::selected) Global::player->setWeight(50.0);
    if (checkboxMedium->Clicked(Global::input) == CheckboxMode::selected) Global::player->setWeight(60.0);
    if (checkboxLarge->Clicked(Global::input)  == CheckboxMode::selected) Global::player->setWeight(70.0);

    if(!Global::player->getWeight())
        Global::player->setWeight(80.0);

    if(Global::player->getName().length() < 1)
        Global::player->setName("Anonim");

    checkboxWindF->Clicked(Global::input) == CheckboxMode::selected ? Global::player->setWind(1) :        
                                                           Global::player->setWind(0);

    Global::player->setWeather(rollBoxWeather->GetSelected());
    Global::player->setMaps(rollBoxMaps->GetSelected());

    TTF_CloseFont(font);

    SDL_StopTextInput();    

    delete buttonBack;
    delete buttonStart;

    delete checkboxSmall;
    delete checkboxMedium;
    delete checkboxLarge;
    delete checkboxWindF;

    delete rollBoxMaps;
    delete rollBoxWeather;

    return Global::status;
}
void LoadingResources()  // wczytuje grafiki do Texture Managera, sprawdza obecnosc plikow 
{
    TextureManager::Instance()->Load("img/Splash.png", "splash", Global::renderer);
    TextureManager::Instance()->Load("img/Menu.png", "menu", Global::renderer);
    TextureManager::Instance()->Load("img/bilboard.jpg", "score", Global::renderer);
    TextureManager::Instance()->Load("img/Info.png", "info", Global::renderer);
    TextureManager::Instance()->Load("img/under_con.png", "start", Global::renderer);
    TextureManager::Instance()->Load("img/Configure.png", "config", Global::renderer);
    TextureManager::Instance()->Load("img/ico-skocznia-1.jpg", "skocznia-1", Global::renderer);
    TextureManager::Instance()->Load("img/ico-skocznia-2.png", "skocznia-2", Global::renderer);
    TextureManager::Instance()->Load("img/ico-skocznia-3.png", "skocznia-3", Global::renderer);
    TextureManager::Instance()->Load("img/weather-rain.png", "rain", Global::renderer);
    TextureManager::Instance()->Load("img/weather-sun.png", "sun", Global::renderer);
    TextureManager::Instance()->Load("img/weather-snow.png", "snow", Global::renderer);
    TextureManager::Instance()->Load("img/weather-wind.png", "wind", Global::renderer);
    TextureManager::Instance()->Load("img/arrows-right.png", "przewijacz", Global::renderer);
    TextureManager::Instance()->Load("img/inputBox.png", "inputBox", Global::renderer);
}

void ReleseResources()  
{
    delete Global::physics;
    delete Global::player;

    SDL_DestroyRenderer(Global::renderer);

    Global::renderer = nullptr;

    SDL_DestroyWindow(Global::window);
    
    Global::window = nullptr;
    
    TextureManager::Instance()->DestroyInstance();     

    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

Uint32 Callback(Uint32 interval, void* param)
{
    Global::physics->setGammaW(Global::physics->randWind(135.0f, 225.0f)); // (RAND -45/+45 stopni) strzalka w kierunku na skoczka     

    return Uint32(interval);
}

void Null()
{
    ;
};
