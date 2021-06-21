#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <memory>


namespace sdl2 {

    // Very useful function from Eric Scott Barr:
    // https://eb2.co/blog/2014/04/c-plus-plus-14-and-sdl2-managing-resources/

    template <typename Creator, typename Destructor, typename... Arguments>
    auto make_resource(Creator c, Destructor d, Arguments&&... args)
    {
        using std::decay_t;
        using std::forward;
        using std::unique_ptr;

        auto r = c(forward<Arguments>(args)...);
        return unique_ptr<decay_t<decltype(*r)>, decltype(d)>(r, d);
    }

    // The "internal type" of the SDL System
    using SDL_System = int;
    using SDLImage_System = int;
    using SDLTTF_System = int;

    // SDL_CreateSDL initiates the use of SDL.
    // The given flags are passed to SDL_Init.
    // The returned value contains the exit code.
    inline SDL_System* SDL_CreateSDL(Uint32 flags)
    {
        auto init_status = new SDL_System;
        *init_status = SDL_Init(flags);
        return init_status;
    }

    // SDL_DestroySDL ends the use of SDL
    inline void SDL_DestroySDL(SDL_System* init_status)
    {
        delete init_status; // Delete the int that contains the return value from SDL_Init
        SDL_Quit();
    }

    inline SDLImage_System* IMG_CreateSDL(Uint32 flags) {
        auto initted = new SDLImage_System;
        *initted = IMG_Init(flags);
        return initted;
    }

    inline void IMG_DestroySDL(SDLImage_System* init_status) {
        delete init_status;
        IMG_Quit();
    }

    inline SDLTTF_System* TTF_CreateSDL() {
        auto initted = new SDLTTF_System;
        *initted = TTF_Init();
        return initted;
    }

    inline void TTF_DestroySDL(SDLTTF_System* init_status) {
        delete init_status;
        TTF_Quit();
    }

    using sdlsystem_ptr_t = std::unique_ptr<SDL_System, decltype(&SDL_DestroySDL)>;
    using window_ptr_t = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>;
    using renderer_ptr_t = std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>;
    using surf_ptr_t = std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>;
    using texture_ptr_t = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>;
    using font_ptr_t = std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)>;

    // Initialize SDL (the returned int* contains the return value from SDL_Init)
    inline sdlsystem_ptr_t make_sdlsystem(Uint32 flags)
    {
        return make_resource(SDL_CreateSDL, SDL_DestroySDL, flags);
    }

    inline sdlsystem_ptr_t make_sdlttfsystem()
    {
        return make_resource(TTF_CreateSDL, TTF_DestroySDL);
    }

    inline sdlsystem_ptr_t make_sdlimagesystem(Uint32 flags)
    {
        return make_resource(IMG_CreateSDL, IMG_DestroySDL, flags);
    }


    // Create a window (that contains both a SDL_Window and the destructor for SDL_Windows)
    inline window_ptr_t make_window(const char* title, int x, int y, int w, int h, Uint32 flags)
    {
        return make_resource(SDL_CreateWindow, SDL_DestroyWindow, title, x, y, w, h, flags);
    }

    // Create a renderer given a window, containing both the renderer and the destructor
    inline renderer_ptr_t make_renderer(SDL_Window* win, int x, Uint32 flags)
    {
        return make_resource(SDL_CreateRenderer, SDL_DestroyRenderer, win, x, flags);
    }

    // Create a surface from a bmp file, containing both the surface and the destructor
    inline surf_ptr_t make_bmp(SDL_RWops* sdlfile)
    {
        // May throw an exception if sdlfile is nullptr
        return make_resource(SDL_LoadBMP_RW, SDL_FreeSurface, sdlfile, 1);
    }

    // Create a surface from a bmp file, containing both the surface and the destructor
    inline surf_ptr_t make_jpg(SDL_RWops* sdlfile)
    {
        // May throw an exception if sdlfile is nullptr
        return make_resource(IMG_LoadJPG_RW, SDL_FreeSurface, sdlfile);
    }

    // Create a texture from a renderer and a surface
    inline texture_ptr_t make_texture(SDL_Renderer* ren, SDL_Surface* surf)
    {
        return make_resource(SDL_CreateTextureFromSurface, SDL_DestroyTexture, ren, surf);
    }

    inline font_ptr_t make_font(const std::string& name, int size)
    {
        return sdl2::make_resource(TTF_OpenFont, TTF_CloseFont, name.c_str(), size);
    }

} 

