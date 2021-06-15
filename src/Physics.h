#pragma once

#include <SDL2/SDL.h>

class Physics 
{
public:
    Physics() noexcept {};
    ~Physics();     

      bool check_Collision(SDL_Surface* surface, int x, int y);
     float angleBetweenTwoPoints(float x1, float y1, float x2, float y2);
    double rand_Wind(double min, double max);
    double Road_Ramp(Uint32 startTime) noexcept;
    double getVelocity() noexcept;
      void setVelocity(double v) noexcept;   
      void set_Wybicie(bool b) noexcept;
      bool get_Wybicie() noexcept; 
    
     const double Deg2Rad = double(M_PI / 180);
     const double Rad2Deg = double(180 / M_PI);

    double get_GammaW();
      void set_GammaW(double angle); 
      
      const double Sun  = 1.0;
      const double Rain = 0.999;
      const double Snow = 0.997;
      const double Wind = 0.999;

private:

        double GammaW = 0.0;
        double Velocity = 0.0;
        bool   Wybicie = false;    // FALSE to za wczesne wybicie lub jego brak - skok rzut pionowy, TRUE - rzut uko�ny   
};