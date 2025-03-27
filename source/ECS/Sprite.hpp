#pragma once

#include "Components.hpp"
#include "SDL2/SDL.h"
#include "../TextureManager.hpp"

class SpriteComponent : public Component
{
public:

    SpriteComponent() = default;

    SpriteComponent(const char* path){
        setTex(path);
    }
    SpriteComponent(const char* path, int nFrames, int mSpeed){
        animated = true;
        frames = nFrames;
        speed = mSpeed;
        setTex(path);
    }
    ~SpriteComponent(){
        SDL_DestroyTexture(texture);
    }
    void init() override{
        transform = &entity->getComponent<TransformComponent>(); 

        srcRect.x = srcRect.y = 0;
        srcRect.w = transform->width;
        srcRect.h = transform->height;
        destRect.w = transform->width * transform->scale;
        destRect.h = transform->height * transform->scale;
    }
      
    void update() override{

        if (animated){
            srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
            
        }
        

        destRect.x = static_cast<int>(transform->position.x);
        // std::cout << position->x() << std::endl;
        destRect.y = static_cast<int>( transform->position.y);
        // std::cout << position->y() << std::endl;
        destRect.w = transform->width * transform->scale;
        destRect.h = transform->height * transform->scale;
    }

    void draw() override{
        TextureManager::Draw(texture, srcRect, destRect);
    }

    void setTex(const char* path){
        texture = TextureManager::LoadTexture(path);
    }

private:
    TransformComponent* transform;
    SDL_Rect srcRect, destRect;
    SDL_Texture* texture;
    bool animated = false;
    int frames = 0;
    int speed = 100;


};
