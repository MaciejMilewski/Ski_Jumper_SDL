#include <iostream>
#include "Texture_Manager.h"

TextureManager* TextureManager::pInstance = nullptr;

bool TextureManager::Load(std::string fileName, std::string id, SDL_Renderer* renderer)
{
    SDL_Surface* tempSurf = IMG_Load(fileName.c_str());

    if (!tempSurf) {
        std::cout << "  ! Nie wczytuje pliku : " << fileName.c_str() << '\n';
        return false;
    } 
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurf);

        SDL_FreeSurface(tempSurf);
   
        if (!texture) {
            std::cout << " ! Nie tworzy textury z surface : " << fileName.c_str() << '\n';
            return false;
        } 
     
     std::cout << " Textura wczytana : " << fileName.c_str() << '\n';

     textureMap[id] = texture;
   
  return true;   
}

void TextureManager::Draw(std::string id, int x, int y, int w, int h, float scale, int rotation,
    SDL_Renderer* renderer, SDL_RendererFlip flip)
{
    SDL_Rect srcRect, destRect;

    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = destRect.w = w;
    srcRect.h = destRect.h = h;
    destRect.x = x;
    destRect.y = y;
    destRect.w = int(destRect.w * scale);
    destRect.h = int(destRect.h * scale);

    SDL_RenderCopyEx(renderer, textureMap[id], &srcRect, &destRect, rotation, NULL, flip);
}

void TextureManager::DrawFrame(std::string id, int x, int y, int w, int h, float scale, int currentRow,
    int currentFrame, int rotation, SDL_Renderer* renderer, SDL_RendererFlip flip)
{
    SDL_Rect srcRect, destRect;

    srcRect.x = w * currentFrame;
    srcRect.y = h * currentRow;
    srcRect.w = destRect.w = w;
    srcRect.h = destRect.h = h;
    destRect.x = x;
    destRect.y = y;
    destRect.w = int(destRect.w * scale);
    destRect.h = int(destRect.h * scale);

    SDL_RenderCopyEx(renderer, textureMap[id], &srcRect, &destRect, rotation, NULL, flip);
}

void TextureManager::clearFromTextureMap(std::string id)
{
    textureMap.erase(id);
}
