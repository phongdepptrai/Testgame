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
        Animation Attack = Animation(2, 7, 100);
        Animation Hurt = Animation(3, 4, 100);

        animations.emplace("Idle", Idle);
        animations.emplace("Run", Run);
        animations.emplace("Attack", Attack);
        

        play("Idle");

        setTex(id);
    }
    ~SpriteComponent(){
    }
    void init() override{
        if(!entity->hasComponent<TransformComponent>()){
            entity->addComponent<TransformComponent>();
        }
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
        if(transform->direction) spriteFlip = SDL_FLIP_HORIZONTAL;
        else spriteFlip = SDL_FLIP_NONE;
        if(texture == nullptr){
            std::cout << "texture is null" << std::endl;
            return;
        }
        TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
    }

    void setTex(std::string id){
        texture = Game::assets->getTexture(id);
        if(texture == nullptr){
            std::cout << "tsprite is null" << std::endl;
            return;
        }
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

