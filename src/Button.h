#pragma once

#include <iostream>

#include <SDL2/SDL.h>


enum class tryb { normal, pressed, selected };

/* tryb wyliczeniowy 'tryb' obrazuje status przycisku Button */

/*  przycisk Button element GUI    
*/
/* PARAM 'normalImagePath'   - �cie�ka dost�pu zako�czona nazw� pliku do wczytania, 'normalny' przycisk */
/* PARAM 'selectedImagePath' - �cie�ka dost�pu zako�czona nazw� pliku do wczytania, 'selected' przycisk */
/* PARAM 'width'  - szeroko�� przycisku na ekranie, wsp�lna dla obu wczytanych obrazk�w                 */
/* PARAM 'heigth' - wysoko�� przycisku na ekranie, wsp�lna dla obu wczytanych obrazk�w                  */
/* PARAM 'posX'   - koordynata X po�o�enia przycisku na ekranie, wsp�lna dla obu wczytanych obrazk�w    */                  
/* PARAM 'posY'   - koordynata Y po�o�enia przycisku na ekranie, wsp�lna dla obu wczytanych obrazk�w    */
/* PARAM 'render' - wskazuje rendera kt�ry globalnie rysuje wszystko na ekranie                         */

class Button
{
public:
    Button(const char* normalImagePath, const char* selectedImagePath,
        unsigned int width, unsigned int height,
        unsigned int posX, unsigned int posY, SDL_Renderer& render);

    virtual~Button();

    void ShowButton();
    void SetX(int x);
    void SetY(int y);
    int  GetX();
    int  GetY();
    tryb Clicked(SDL_Event& e);

private:
    SDL_Renderer* render = NULL;
    SDL_Surface*  loaded_surface = NULL;
    SDL_Texture*  normalImage, * selectedImage;
    SDL_Point     mouse_XY;

    tryb mode = tryb::normal;

    int width = 0, height = 0, posX = 0, posY = 0;

    std::string imagePath = "", selectedImagePath = "";
};
