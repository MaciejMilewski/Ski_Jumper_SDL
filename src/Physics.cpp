#include <iostream>

#include "Physics.h"

Physics:: ~Physics()
{

};

double Physics::getGammaW()
{
    return gammaW;
}
void Physics::setGammaW(double angle)
{
    gammaW = angle;
}

bool Physics::getJumpStart() noexcept
{
    return jumpStart;
}

void Physics::setJumpStart(bool b) noexcept
{
    jumpStart = b;
};

void Physics::setVelocity(double v) noexcept
{
    velocity = v;
}

double Physics::getVelocity() noexcept
{
    return velocity;
}

double Physics::roadOnRamp(Uint32 startTime) noexcept
{
    // Droga w ruchu jednostajnie przyspieszonym prostoliniowym bez pr�dko�ci pocz�tkowej na skoczni:
    //    S = sinus(alfa) * g * t * t * 0.5 ; WZ�R: bez oporu powietrza i tarcia !!!

    constexpr double sinus_alfa_30 = 0.50;         // skocznia pochylona pod k�tem 30 stopni

    constexpr double g = 9.81;                     // przy�pieszenie ziemskie

    const Uint32 dt = SDL_GetTicks() - startTime;  // up�yw czasu   

    const double t = dt * 0.001;                   // przelicznik z milisekund na sekundy

    velocity = g * t;                              // pr�dko�� na rampie [m/s]    

    return sinus_alfa_30 * g * t * t * 0.5;        // [metry]
}

bool Physics::checkCollision(SDL_Surface* surface, int x, int y) 
{
    int bpp = surface->format->BytesPerPixel;

    Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

    Uint8 red, green, blue, alpha;  // dla 24-bitowego pliku graficznego

    SDL_GetRGBA(*p, surface->format, &red, &green, &blue, &alpha);

    return (red >= 0 && red < 250 ? true : false);
}

float Physics::angleBetweenTwoPoints(float x1, float y1, float x2, float y2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;

    return -float(atan2(dy, dx) * Rad2Deg);
}
double Physics::randWind(double min, double max)
{
    return ((max - min) * ((double)rand() / RAND_MAX)) + min;
}
