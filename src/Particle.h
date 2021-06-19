#pragma once

#include <iostream>

#include <SDL2/SDL_image.h>

#include "ParticleSystem.h"

class Particle : public ParticleSystem
{
public:
    Particle() {}
    virtual ~Particle() {}

    enum class Style
    {
         RAIN, SUN, SNOW, NONE              
    };

    Style style_ = Style::NONE;

    void setStyle(Style style);

    SDL_Texture* getDefaultTexture();   
};

