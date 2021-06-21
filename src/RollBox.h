#pragma once

#include <iostream>

#include <SDL2/SDL.h>

#include "TextureManager.h"

/* przycisk RollBox element GUI                                                                         */
/* PARAM 'tab[]'  - tablica z nazwami textur kt�re s� w kontenerze map, obrazki kolejno b�d�            */
/* PARAM            wy�wietlane po naci�ni�ciu przycisku 'przewi�'                                      */
/* PARAM 'width'  - szeroko�� przycisku na ekranie, wsp�lna dla obu wczytanych obrazk�w                 */
/* PARAM 'heigth' - wysoko�� przycisku na ekranie, wsp�lna dla obu wczytanych obrazk�w                  */
/* PARAM 'posX'   - koordynata X po�o�enia przycisku na ekranie, wsp�lna dla obu wczytanych obrazk�w    */
/* PARAM 'posY'   - koordynata Y po�o�enia przycisku na ekranie, wsp�lna dla obu wczytanych obrazk�w    */
/* PARAM 'render' - wskazuje rendera kt�ry globalnie rysuje wszystko na ekranie                         */

class RollBox
{
public:
    RollBox(std::string tab[],
        unsigned int width, unsigned int height,
        unsigned int posX, unsigned int posY, int size, SDL_Renderer* render);

    virtual~RollBox();

    void Show();

    void Clicked(SDL_Event& e);
    int get_Selected();

private:
    SDL_Renderer* render = NULL;
    SDL_Texture* texture = NULL;

    SDL_Point mouse_XY{ 0,0 };

    std::string* t = nullptr;

    int width = 0, height = 0, posX = 0, posY = 0, sizeTab = 0, nr_Pict = 0;
};

