#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Texture_Manager.h"
#include "Button.h"
#include "CheckBox.h"
#include "Player.h"
#include "RollBox.h"


enum class Status { MENU, START, SCORE, INFO, CONFIG, EXIT };

namespace Global 
{
    const int  SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    SDL_Window* window      = NULL;
    SDL_Renderer* renderer  = NULL;
    
    SDL_Event input;

    Player player;

    Status status = Status::MENU;  
   
    const int FPS = 24;
    const int frameDelay = 1000 / FPS;
    Uint32    frameStart;
    int       frameTime;
}

void Loading_Resourses();
void Release_Resourses();

void Splash();

Status Menu();
Status Start();
Status Scoreboard();
Status Info();
Status Config();

std::string InputTextBox(int lengthText, int highText, int xPos, int yPos);

int main(int argc, char* argv[]) ////// Początek i podłączenie kolejnych bibliotek /////
{
    try {        
       
        if (SDL_Init(SDL_INIT_EVERYTHING)) {
            throw SDL_GetError();
            return 1;
        }

        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {	                    // Ustawia filtrowanie textury na 'linear' - b.dobra jakość        
            throw " Uwaga: filtrowanie 'linear' textur nie jest ustawione ! \n";
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

        Global::renderer = SDL_CreateRenderer(Global::window, -1,  SDL_RENDERER_PRESENTVSYNC|SDL_RENDERER_ACCELERATED);
        
        if (Global::renderer == NULL) {
            SDL_GetError();
            return 5;
        }
                
        Loading_Resourses();        

        Splash();       
        
        // ------------------------------------ MAIN LOOP ---------------------------------------------------- //

        bool mousedown = 0;

        while (Global::status != Status::EXIT)
        {              
            while (SDL_PollEvent(&Global::input) > 0)
            {
                switch (Global::input.type) {
                case SDL_QUIT: Global::status = Status::EXIT;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    mousedown = 1;
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (mousedown) {
                        /* The SDL_MOUSEBUTTONUP event will be ignored if a
                           previous SDL_MOUSEBUTTONDOWN event has not occurred. */                        
                    }
                    mousedown = 0;
                    break;
                }
            }      
            
            switch (Global::status)
            {
                case Status::MENU      : Menu();       break;   //  Wywołanie procedur odpowiednio do wykonywanej funkcji
                case Status::START     : Start();      break;   //  każda procedura zwraca status:: który kieruje wywołania 
                case Status::SCORE     : Scoreboard(); break;   //  do kolejnych procedur lub klass, ustawienie zmiennej
                case Status::INFO      : Info();       break;   // 'Global::Status = Status::Exit;' kończy program ... .
                case Status::CONFIG    : Config();     break;               
            }            
        }        

        Release_Resourses();
        
    } catch (const char* msg)   // Obsługa wyjątków
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

    SDL_RenderPresent(Global::renderer);                        

    SDL_Delay(3000);                                            

    TextureManager::Instance()->clearFromTextureMap("splash");  // usuwa 'splash' z pamięci kontenera map
}

Status Menu()   // zarządza Menu Głównym
{    
    TextureManager::Instance()->Draw("menu", 0, 0, Global::SCREEN_WIDTH, Global::SCREEN_HEIGHT, 1.0, 0, Global::renderer);

    Button* buttonInfos = new Button("img/help_button.png",  "img/helpS_button.png",  100, 52,  50, 530, *Global::renderer);
    Button* buttonStart = new Button("img/Start_button.png", "img/StartS_button.png", 100, 52, 210, 530, *Global::renderer);
    Button* buttonStore = new Button("img/score_button.png", "img/scoreS_button.png", 130, 60, 410, 530, *Global::renderer);
    Button* buttonExits = new Button("img/Exits_button.png", "img/ExitsS_button.png", 100, 52, 610, 530, *Global::renderer);       

    bool mousedown = 0;

    while (Global::status == Status::MENU)
    {
        Global::frameStart = SDL_GetTicks();                        // przechowuje czas startu pętli

        while (SDL_PollEvent(&Global::input) > 0)
        {
            switch (Global::input.type) {
            case SDL_QUIT: Global::status = Status::EXIT;
                break;
            case SDL_MOUSEBUTTONDOWN:
                mousedown = 1;
                break;
            case SDL_MOUSEBUTTONUP:
                if (mousedown) {
                    /* The SDL_MOUSEBUTTONUP event will be ignored if a
                       previous SDL_MOUSEBUTTONDOWN event has not occurred. */                   
                }
                mousedown = 0;
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

        if (tryb::selected == buttonInfos->Clicked(Global::input))
            Global::status = Status::INFO;
        if (tryb::selected == buttonStart->Clicked(Global::input))
            Global::status = Status::CONFIG;
        if (tryb::selected == buttonStore->Clicked(Global::input))
            Global::status = Status::SCORE;
        if (tryb::selected == buttonExits->Clicked(Global::input))
            Global::status = Status::EXIT;

        SDL_RenderPresent(Global::renderer);

        Global::frameTime = SDL_GetTicks() - Global::frameStart;      // ile czasu zabrała iteracja

        if (Global::frameDelay > Global::frameTime)                   // zabezpiecza przed wyświetleniem więcej klatek niż 60/Second
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
Status Start() // zarządza START - under constraction !
{    
    TextureManager::Instance()->Draw("start", 0, 0, Global::SCREEN_WIDTH, Global::SCREEN_HEIGHT, 1.0, 0, Global::renderer);

    Button* buttonMenu = new Button("img/back_button.png", "img/backS_button.png", 100, 52, 50, 530, *Global::renderer);       
    
    bool mousedown = 0;
    
    while (Global::status == Status::START)
    {
        Global::frameStart = SDL_GetTicks();                        // przechowuje czas startu pętli
        
        while (SDL_PollEvent(&Global::input) > 0)
        {
            switch (Global::input.type) {
            case SDL_QUIT: Global::status = Status::EXIT;
                break;
            case SDL_MOUSEBUTTONDOWN:
                mousedown = 1;
                break;
            case SDL_MOUSEBUTTONUP:
                if (mousedown) {
                    /* The SDL_MOUSEBUTTONUP event will be ignored if a
                       previous SDL_MOUSEBUTTONDOWN event has not occurred. */
                }
                mousedown = 0;
                break;
            }
        }

        buttonMenu->Clicked(Global::input);
        buttonMenu->ShowButton();

        if (tryb::selected == buttonMenu->Clicked(Global::input))
            Global::status = Status::MENU;

        SDL_RenderPresent(Global::renderer);     
        
        Global::frameTime = SDL_GetTicks() - Global::frameStart;      // ile czasu zabrała iteracja

        if (Global::frameDelay > Global::frameTime)                   // zabezpiecza przed wyświetleniem więcej klatek niż 60/Second
        {
            SDL_Delay(Global::frameDelay - Global::frameTime);
        }           
    }

    delete buttonMenu;

    return Global::status;
}

Status Scoreboard() // zarządza SCOREBOARD 
{    
    TextureManager::Instance()->Draw("score", 0, 0, Global::SCREEN_WIDTH, Global::SCREEN_HEIGHT, 1.0, 0, Global::renderer);

    Button* buttonScore = new Button("img/back_button.png", "img/backS_button.png", 100, 52, 40, 535, *Global::renderer);     

    bool mousedown = 0;

    while (Global::status == Status::SCORE)
    {
        Global::frameStart = SDL_GetTicks();                        // przechowuje czas startu pętli
        
        while (SDL_PollEvent(&Global::input) > 0)
        {
            switch (Global::input.type) {
            case SDL_QUIT: Global::status = Status::EXIT;
                break;
            case SDL_MOUSEBUTTONDOWN:
                mousedown = 1;
                break;
            case SDL_MOUSEBUTTONUP:
                if (mousedown) {
                    /* The SDL_MOUSEBUTTONUP event will be ignored if a
                       previous SDL_MOUSEBUTTONDOWN event has not occurred. */
                }
                mousedown = 0;
                break;
            }
        }

        buttonScore->Clicked(Global::input);
        buttonScore->ShowButton();

        if (tryb::selected == buttonScore->Clicked(Global::input))
            Global::status = Status::MENU;

        SDL_RenderPresent(Global::renderer);   

        Global::frameTime = SDL_GetTicks() - Global::frameStart;      // ile czasu zabrała iteracja

        if (Global::frameDelay > Global::frameTime)                   // zabezpiecza przed wyświetleniem więcej klatek niż 60/Second
        {
            SDL_Delay(Global::frameDelay - Global::frameTime);
        }            
    }

    delete buttonScore;
    
    return Global::status;
}

Status Info() // zarządza ekranem INFO
{
    TextureManager::Instance()->Draw("info", 0, 0, Global::SCREEN_WIDTH, Global::SCREEN_HEIGHT, 1.0, 0, Global::renderer);

    Button* buttonInfo = new Button("img/back_button.png", "img/backS_button.png", 90, 52, 40, 540, *Global::renderer);    
    
    bool mousedown = 0;

    while (Global::status == Status::INFO)
    {
        Global::frameStart = SDL_GetTicks();                        // przechowuje czas startu pętli
        
        while (SDL_PollEvent(&Global::input) > 0)
        {
            switch (Global::input.type) {
            case SDL_QUIT: Global::status = Status::EXIT;
                break;
            case SDL_MOUSEBUTTONDOWN:
                mousedown = 1;
                break;
            case SDL_MOUSEBUTTONUP:
                if (mousedown) {
                    /* The SDL_MOUSEBUTTONUP event will be ignored if a
                       previous SDL_MOUSEBUTTONDOWN event has not occurred. */
                }
                mousedown = 0;
                break;
            }
        }

        buttonInfo->Clicked(Global::input);
        buttonInfo->ShowButton();

        if (tryb::selected == buttonInfo->Clicked(Global::input))
            Global::status = Status::MENU;

        SDL_RenderPresent(Global::renderer);      

        Global::frameTime = SDL_GetTicks() - Global::frameStart;      // ile czasu zabrała iteracja

        if (Global::frameDelay > Global::frameTime)                   // zabezpiecza przed wyświetleniem więcej klatek niż 60/Second
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

    Button* buttonBack = new Button("img/back_button.png", "img/backS_button.png", 100, 52, 250, 540, *Global::renderer);
    Button* buttonStart = new Button("img/Start_button.png", "img/StartS_button.png", 100, 52, 410, 540, *Global::renderer);

    CheckBox* chb_Small = new CheckBox("img/s_button.png", "img/sS_button.png", " ", "img/arial.ttf", 36, 0, 40, 224, 366, *Global::renderer);
    CheckBox* chb_Medium = new CheckBox("img/m_button.png", "img/mS_button.png", " ", "img/arial.ttf", 36, 0, 40, 278, 366, *Global::renderer);
    CheckBox* chb_Large = new CheckBox("img/L_button.png", "img/LS_button.png", " ", "img/arial.ttf", 36, 0, 42, 337, 366, *Global::renderer);
    CheckBox* chb_WindF = new CheckBox("img/Stable_button.png", "img/FickleS_button.png", "", "img/arial.ttf", 36, 0, 52, 526, 303, *Global::renderer);
    
    std::string Mapy[3]    = { "skocznia-1", "skocznia-2", "skocznia-3" };
    std::string Weather[4] = { "rain", "sun",  "snow", "wind" };

    RollBox* roll_Maps    = new RollBox(Mapy   , 120, 80, 240, 420, 2, Global::renderer);
    RollBox* roll_Weather = new RollBox(Weather, 120, 80, 535, 365, 3, Global::renderer);

    int check_weight = 1;

    chb_Small->set_Mode(tryby::selected);

    Global::player.set_Name(InputTextBox(8, 36, 244, 304));

    bool mousedown = 0;

    while (Global::status == Status::CONFIG)
    {
        Global::frameStart = SDL_GetTicks();                        // przechowuje czas startu pętli

        while (SDL_PollEvent(&Global::input) > 0)
        {
            switch (Global::input.type) {
            case SDL_QUIT: Global::status = Status::EXIT;
                break;
            case SDL_MOUSEBUTTONDOWN:
                mousedown = 1;
                break;
            case SDL_MOUSEBUTTONUP:
                if (mousedown) {
                    /* The SDL_MOUSEBUTTONUP event will be ignored if a
                       previous SDL_MOUSEBUTTONDOWN event has not occurred. */
                }
                mousedown = 0;
                break;
            }
        }
        
        roll_Maps->Show();
        roll_Weather->Show();

        buttonBack->ShowButton();
        buttonStart->ShowButton();

        chb_Small->ShowButton();
        chb_Small->Clicked(Global::input);
        chb_Medium->ShowButton();
        chb_Medium->Clicked(Global::input);
        chb_Large->ShowButton();
        chb_Large->Clicked(Global::input);

        chb_WindF->ShowButton();
        chb_WindF->Clicked(Global::input);

        roll_Maps->Clicked(Global::input);
        roll_Weather->Clicked(Global::input);

        if (tryb::selected == buttonBack->Clicked(Global::input))
            Global::status = Status::MENU;
        if (tryb::selected == buttonStart->Clicked(Global::input))
            Global::status = Status::START;

        if ((chb_Small->Clicked(Global::input) == tryby::selected) && (check_weight == 1)) {
            check_weight = 0;
        }

        if (chb_Small->Clicked(Global::input) == tryby::selected && check_weight == 0) {
            check_weight = 1;
        }
        else chb_Small->set_Mode(tryby::normal);

        if (chb_Medium->Clicked(Global::input) == tryby::selected && check_weight == 2) {
            check_weight = 0;
        }
        if (chb_Medium->Clicked(Global::input) == tryby::selected && check_weight == 0) {
            check_weight = 2;
        }
        else chb_Medium->set_Mode(tryby::normal);

        if (chb_Large->Clicked(Global::input) == tryby::selected && check_weight == 3) {
            check_weight = 0;
        }
        if (chb_Large->Clicked(Global::input) == tryby::selected && check_weight == 0) {
            check_weight = 3;
        }
        else chb_Large->set_Mode(tryby::normal);

        SDL_RenderPresent(Global::renderer);

        Global::frameTime = SDL_GetTicks() - Global::frameStart;      // ile czasu zabrała iteracja

        if (Global::frameDelay > Global::frameTime)                   // zabezpiecza przed wyświetleniem więcej klatek niż 60/Second
        {
            SDL_Delay(Global::frameDelay - Global::frameTime);            
        }      
    }
    
    if (chb_Small ->Clicked(Global::input) == tryby::selected) Global::player.set_Weight(50);
    if (chb_Medium->Clicked(Global::input) == tryby::selected) Global::player.set_Weight(60);
    if (chb_Large ->Clicked(Global::input) == tryby::selected) Global::player.set_Weight(70);

    chb_WindF->Clicked(Global::input) == tryby::selected ? Global::player.set_Weather(1) : Global::player.set_Weather(0);

    delete buttonBack;
    delete buttonStart;

    delete chb_Small;
    delete chb_Medium;
    delete chb_Large; 
    delete chb_WindF;   

    delete roll_Maps;
    delete roll_Weather;

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
    TextureManager::Instance()->Load("img/ico-skocznia-1.jpg", "skocznia-1", Global::renderer);
    TextureManager::Instance()->Load("img/ico-skocznia-2.png", "skocznia-2", Global::renderer);
    TextureManager::Instance()->Load("img/ico-skocznia-3.png", "skocznia-3", Global::renderer);
    TextureManager::Instance()->Load("img/weather-rain.png", "rain", Global::renderer);
    TextureManager::Instance()->Load("img/weather-sun.png", "sun", Global::renderer);
    TextureManager::Instance()->Load("img/weather-snow.png", "snow", Global::renderer);
    TextureManager::Instance()->Load("img/weather-wind.png", "wind", Global::renderer);
    TextureManager::Instance()->Load("img/arrows-right.png", "przewijacz", Global::renderer);
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
std::string InputTextBox(int lengthText, int highText, int xPos, int yPos)
{
    std::string  input;
    SDL_Texture* texture, *text;
    SDL_Event e;
    SDL_Rect  dest;  
    TTF_Font* font;

    SDL_Surface* buffer = IMG_Load("img/inputBox.png");
    if (!buffer) {
        std::cout << "Error loading image inputBox.png: " << SDL_GetError() << '\n';
    }

    texture = SDL_CreateTextureFromSurface(Global::renderer, buffer);

    SDL_FreeSurface(buffer);

    buffer = NULL;

    if (!texture) {
        std::cout << "Error creating texture: " << SDL_GetError() << '\n';
    }

    font = TTF_OpenFont("img/arial.ttf", highText);
    if (!font) {
        std::cout << "Error loading font: " << TTF_GetError() << '\n';
    }

    SDL_StartTextInput();

    SDL_Color colorText{ 0, 0, 240, 255 };

    SDL_Rect background{ xPos - 12, yPos - 4, 158, 50 };

    bool quit = false;

    bool mousedown = 0;

    while (!quit)
    {
        Global::frameStart = SDL_GetTicks();                        // przechowuje czas startu pętli
        
        SDL_RenderCopy(Global::renderer, texture, NULL, &background);
        SDL_RenderPresent(Global::renderer);

        while (SDL_PollEvent(&e) != 0) {
            switch (e.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_TEXTINPUT:
                input += e.text.text;
                if (input.size() == lengthText)
                    quit = true;
                break;
            case SDL_KEYDOWN:
                if (e.key.keysym.sym == SDLK_BACKSPACE && input.size())
                    input.pop_back();
                if (e.key.keysym.sym == SDLK_RETURN && input.size())
                    quit = true;
                break;                        
            case SDL_MOUSEBUTTONDOWN:
                mousedown = 1;
                break;
            case SDL_MOUSEBUTTONUP:
                if (mousedown) {
                    /* The SDL_MOUSEBUTTONUP event will be ignored if a
                       previous SDL_MOUSEBUTTONDOWN event has not occurred. */
                }
                mousedown = 0;
                break;
            }
        }

        if (input.size()) {
            SDL_Surface* text_surf = TTF_RenderText_Solid(font, input.c_str(), colorText);
            text = SDL_CreateTextureFromSurface(Global::renderer, text_surf);

            dest.x = xPos;
            dest.y = yPos;
            dest.w = text_surf->w;
            dest.h = text_surf->h;

            SDL_RenderCopy(Global::renderer, text, NULL, &dest);

            SDL_RenderPresent(Global::renderer);

            SDL_DestroyTexture(text);
            SDL_FreeSurface(text_surf);  

            Global::frameTime = SDL_GetTicks() - Global::frameStart;      // ile czasu zabrała iteracja

            if (Global::frameDelay > Global::frameTime)                   // zabezpiecza przed wyświetleniem więcej klatek niż 60/Second
            {
                SDL_Delay(Global::frameDelay - Global::frameTime);
            }
            SDL_Delay(500);
        }
    }
    SDL_DestroyTexture(texture);
    SDL_StopTextInput();

    return input;
}
