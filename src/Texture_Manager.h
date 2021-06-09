#pragma once

#include <string>
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class TextureManager
{
public:

    static TextureManager* Instance()
    {
        if (pInstance == nullptr)
        {
            pInstance = new TextureManager();
        }

        return pInstance;
    }
    void DestroyInstance()
    {
        delete pInstance;
               pInstance = nullptr;
    };

    bool Load(std::string fileName, std::string id, SDL_Renderer* renderer);

    void Draw(std::string id, int x, int y, int w, int h, float scale, int rotation, SDL_Renderer* renderer,
        SDL_RendererFlip flip = SDL_FLIP_NONE);

    void DrawFrame(std::string id, int x, int y, int w, int h, float scale, int currentRow, int currentFrame, int rotation,
        SDL_Renderer* renderer, SDL_RendererFlip flip = SDL_FLIP_NONE);

    void clearFromTextureMap(std::string id);

    std::map<std::string, SDL_Texture*> textureMap;

private:

    static TextureManager* pInstance;

     TextureManager() {}                                        // zabezpiecza przed stworzeniem nowej instancji
    ~TextureManager() {}                                        // zabezpiecza przed niechcian� likwidacj�    
    TextureManager(const TextureManager&);                      // zabezpiecza przed stworzeniem poprzez kopiowanie
    TextureManager& operator=(const TextureManager&) = delete;  // zabezpiecza przed do��czeniem poprzez =
};

typedef TextureManager _TextureManager;