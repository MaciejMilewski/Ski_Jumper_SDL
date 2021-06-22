#include "Particle.h"

SDL_Texture* Particle::getDefaultTexture()
{
    static SDL_Texture* t = IMG_LoadTexture(_renderer, "img/fire.png");
    if (!t)
        std::cerr << " Brak pliku 'img/fire.png'- " << SDL_GetError() << '\n';
    return t;
}

void Particle::setStyle(Style style)
{
    if (style_ == style)
    {
        return;
    }
    style_ = style;


    if (style == Style::NONE)
    {
        stopSystem();
    }
    if (_texture == nullptr)
    {
        setTexture(getDefaultTexture());
    }

    switch (style)
    {    
    case Particle::Style::SUN:
    {
        initWithTotalParticles(350);
        _duration = DURATION_INFINITY;

        // Tryb grawitacja - opadanie czastek
        setEmitterMode(Mode::GRAVITY);
        setGravity(Vec2(0, 0));

        // a czastek
        setRadialAccel(0);
        setRadialAccelVar(0);

        // v czasteczek
        setSpeed(-20);
        setSpeedVar(5);

        // katy
        _angle = 90;
        _angleVar = 360;

        // zycie czasteczek
        _life = 1;
        _lifeVar = 0.5f;

        // wielkosc pixeli
        _startSize = 30.0f;
        _startSizeVar = 10.0f;
        _endSize = START_SIZE_EQUAL_TO_END_SIZE;

        // emisja na sekunde
        _emissionRate = _totalParticles / _life;

        // kolory czastek
        _startColor.r = 0.76f;
        _startColor.g = 0.25f;
        _startColor.b = 0.12f;
        _startColor.a = 1.00f;
        _startColorVar.r = 0.0f;
        _startColorVar.g = 0.0f;
        _startColorVar.b = 0.0f;
        _startColorVar.a = 0.0f;
        _endColor.r = 0.94f;
        _endColor.g = 0.92f;
        _endColor.b = 0.25f;
        _endColor.a = 1.0f;
        _endColorVar.r = 0.0f;
        _endColorVar.g = 0.0f;
        _endColorVar.b = 0.0f;
        _endColorVar.a = 0.0f;

        _posVar = { 0, 0 };
        break;
    }    
    case Particle::Style::SNOW:
    {
        initWithTotalParticles(700);        
        _duration = DURATION_INFINITY;        
        setEmitterMode(Mode::GRAVITY);       
        setGravity(Vec2(0, 1));
        setSpeed(-35);
        setSpeedVar(5);

        setRadialAccel(0);
        setRadialAccelVar(1);
        
        setTangentialAccel(0);
        setTangentialAccelVar(1);
     
        _angle = -90;
        _angleVar = 5;
        
        _life = 45;
        _lifeVar = 15;
        
        _startSize = 10.0f;
        _startSizeVar = 5.0f;
        _endSize = START_SIZE_EQUAL_TO_END_SIZE;
        
        _emissionRate = 10;
        
        _startColor.r = 1.0f;
        _startColor.g = 1.0f;
        _startColor.b = 1.0f;
        _startColor.a = 1.0f;
        _startColorVar.r = 0.0f;
        _startColorVar.g = 0.0f;
        _startColorVar.b = 0.0f;
        _startColorVar.a = 0.0f;
        _endColor.r = 1.0f;
        _endColor.g = 1.0f;
        _endColor.b = 1.0f;
        _endColor.a = 0.0f;
        _endColorVar.r = 0.0f;
        _endColorVar.g = 0.0f;
        _endColorVar.b = 0.0f;
        _endColorVar.a = 0.0f;

        _posVar = { 1.0f * x_, 0.0f };
        break;
    }
    case Particle::Style::RAIN:
    {
        initWithTotalParticles(1500);       
        _duration = DURATION_INFINITY;
        setEmitterMode(Mode::GRAVITY);       
        setGravity(Vec2(10, 10));
      
        setRadialAccel(0);
        setRadialAccelVar(2.5);
        
        setTangentialAccel(0);
        setTangentialAccelVar(1);

        setSpeed(-180);
        setSpeedVar(20);
       
        _angle = -90;
        _angleVar = 5;
       
        _life = 4.5f;
        _lifeVar = 0;
        
        _startSize = 3.0f;
        _startSizeVar = 2.0f;
        _endSize = START_SIZE_EQUAL_TO_END_SIZE;
        
        _emissionRate = 40;
       
        _startColor.r = 0.2f;
        _startColor.g = 0.2f;
        _startColor.b = 1.0f;
        _startColor.a = 1.0f;
        _startColorVar.r = 0.0f;
        _startColorVar.g = 0.0f;
        _startColorVar.b = 0.0f;
        _startColorVar.a = 0.0f;
        _endColor.r = 0.7f;
        _endColor.g = 0.8f;
        _endColor.b = 1.0f;
        _endColor.a = 0.5f;
        _endColorVar.r = 0.0f;
        _endColorVar.g = 0.0f;
        _endColorVar.b = 0.0f;
        _endColorVar.a = 0.0f;

        _posVar = { 1.0f * x_, 0.0f };
        break;
    }
    default:
        break;
    }
}
