#pragma once

#include "EntityComponentSystem.hpp"
#include "Transform.hpp"
#include "SDL2/SDL.h"
#include "Sprite.hpp"
#include<String>

class TileComponent : public Component{
public:
    TransformComponent *transform;
    SpriteComponent *sprite;
    SDL_Rect srcRect, destRect;
    SDL_Texture *texture;

    SDL_Rect tileRect;

    Vector2D position;
    const char* path;

    TileComponent() = default;
    ~TileComponent(){
        SDL_DestroyTexture(texture);
    }


    TileComponent(int srcX, int srcY, int xpos, int ypos, int tsize, int tscale, const char* path) {
        texture = TextureManager::LoadTexture(path);
        srcRect.x = srcX;
        srcRect.y = srcY;
        srcRect.w = srcRect.h = tsize;
        
        destRect.x = xpos;
        destRect.y = ypos;
        destRect.w = destRect.h = tsize * tscale;

        position.x = xpos;
        position.y = ypos;

    }
    void update() override{
        destRect.x = position.x - Game::camera.x;
        destRect.y = position.y - Game::camera.y;
    }
    void draw() override{
        TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
    }

 
    





private:

};