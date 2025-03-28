#pragma once

#include "Components.hpp"
#include "SDL2/SDL.h"
#include "../TextureManager.hpp"
#include "../AssetManager.hpp"
#include "Animation.hpp"

#include <map>


class SpriteComponent : public Component
{
public:

    int animIndex = 0;
    std::map<const char*, Animation> animations;

    SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;
    SpriteComponent() = default;

    SpriteComponent(std::string id){
        setTex(id);
    }
    SpriteComponent(std::string id, bool isAnimated){
        animated = isAnimated;
        Animation Idle = Animation(0, 10, 100);
        Animation Run = Animation(1, 16, 100);

        animations.emplace("Idle", Idle);
        animations.emplace("Run", Run);

        play("Idle");

        setTex(id);
    }
    ~SpriteComponent(){
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
        srcRect.y = animIndex * transform->height ;

        destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
        // std::cout << position->x() << std::endl;
        destRect.y = static_cast<int>( transform->position.y) - Game::camera.y;
        // std::cout << position->y() << std::endl;
        destRect.w = transform->width * transform->scale;
        destRect.h = transform->height * transform->scale;
    }

    void draw() override{
        TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
    }

    void setTex(std::string id){
        texture = Game::assets->getTexture(id);
    }
    
    void play(const char* animName){
        frames = animations[animName].frames;
        animIndex = animations[animName].index;
        speed = animations[animName].speed;
    }

private:
    TransformComponent* transform;
    SDL_Rect srcRect, destRect;
    SDL_Texture* texture;
    bool animated = false;
    int frames = 0;
    int speed = 100;


};

