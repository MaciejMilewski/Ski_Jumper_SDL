#include <iostream>

#include "Physics.h"

Physics:: ~Physics()
{

};

double Physics::get_GammaW()
{
    return GammaW;
}
void Physics::set_GammaW(double angle)
{
    GammaW = angle;
}

bool Physics::get_Wybicie() noexcept
{
    return Wybicie;
}

void Physics::set_Wybicie(bool b) noexcept
{
    Wybicie = b;
};

void Physics::setVelocity(double v) noexcept
{
    Velocity = v;
}

double Physics::getVelocity() noexcept
{
    return Velocity;
}

double Physics::Road_Ramp(Uint32 startTime) noexcept
{
    // Droga w ruchu jednostajnie przyspieszonym prostoliniowym bez prêdkoœci pocz¹tkowej na skoczni:
    //    S = sinus(alfa) * g * t * t * 0.5 ; WZÓR: bez oporu powietrza i tarcia !!!

    constexpr double sinus_alfa_30 = 0.50;         // skocznia pochylona pod k¹tem 30 stopni

    constexpr double g = 9.81;                     // przyœpieszenie ziemskie

    const Uint32 dt = SDL_GetTicks() - startTime;  // up³yw czasu   

    const double t = dt * 0.001;                   // przelicznik z milisekund na sekundy

    Velocity = g * t;                              // prêdkoœæ na rampie [m/s]    

    return sinus_alfa_30 * g * t * t * 0.5;        // [metry]
}

bool Physics::check_Collision(SDL_Surface* surface, int x, int y) 
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
double Physics::rand_Wind(double min, double max)
{
    return ((max - min) * ((double)rand() / RAND_MAX)) + min;
}
