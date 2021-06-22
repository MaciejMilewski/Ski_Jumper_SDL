#pragma once

#include <SDL2/SDL.h>

class Physics 
{
public:
    Physics() noexcept {};
    ~Physics();     

      bool checkCollision(SDL_Surface* surface, int x, int y);
     float angleBetweenTwoPoints(float x1, float y1, float x2, float y2);
    double randWind(double min, double max);
    double roadOnRamp(Uint32 startTime) noexcept;
    double getVelocity() noexcept;
      void setVelocity(double v) noexcept;   
      void setJumpStart(bool b) noexcept;
      bool getJumpStart() noexcept; 
    
     const double Deg2Rad = double(M_PI / 180);
     const double Rad2Deg = double(180 / M_PI);

    double getGammaW();
      void setGammaW(double angle); 
      
      const double Sun  = 1.0;
      const double Rain = 0.999;
      const double Snow = 0.997;
      const double Wind = 0.999;

private:

        double gammaW = 0.0;
        double velocity = 0.0;
        bool   jumpStart = false;    // FALSE to za wczesne wybicie lub jego brak - skok rzut pionowy, TRUE - rzut ukosny   
};