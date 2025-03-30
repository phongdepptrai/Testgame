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
    Uint32 timer = 0;
    SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;
    SpriteComponent() = default;
    std::string currentEntity; 

    SpriteComponent(std::string id){
        currentEntity = id;
        setTex(id);
    }
    SpriteComponent(std::string id, bool isAnimated){
        animated = isAnimated;
        currentEntity = id;
        if(id == "player"){
            Animation Idle = Animation(0, 10, 100);
            Animation Run = Animation(1, 16, 100);
            Animation Attack = Animation(2, 7, 100);
            Animation Hurt = Animation(3, 4, 100);

            animations.emplace("Idle", Idle);
            animations.emplace("Run", Run);
            animations.emplace("Attack", Attack);
            animations.emplace("Hurt", Hurt);

            play("Idle");
        }
        else if(id == "yellowGolem"|| id == "blueGolem"){
            Animation Run = Animation(0,10,200);
            Animation Attack = Animation(1,11,200);
            Animation Hurt = Animation(2,12,100);

            animations.emplace("Run", Run);
            animations.emplace("Attack", Attack);
            animations.emplace("Hurt", Hurt); 

            play("Run");
        }
        else if( id == "Player_projectile"){
            Animation projectile = Animation(0, 4, 100);
            animations.emplace("projectile", projectile);
            play("projectile");
        }
        

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
        if(currentEntity == "Player_projectile"){
            destRect.h = destRect.w = 128;
        }
    }
      
    void update() override{

        if (animated){
            srcRect.x = srcRect.w * static_cast<int>(((SDL_GetTicks() - timer) / speed) % frames);
        }
        if(animated && currentEntity == "player" && frames <= 1){
            srcRect.x = srcRect.w * static_cast<int>(((SDL_GetTicks()) / speed) % frames);
        }
        srcRect.y = animIndex * transform->height ;


        destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
        // std::cout << position->x() << std::endl;
        destRect.y = static_cast<int>( transform->position.y) - Game::camera.y;
        // std::cout << position->y() << std::endl; 
        destRect.w = transform->width * transform->scale;
        destRect.h = transform->height * transform->scale;
        if(currentEntity == "Player_projectile"){
            destRect.h = destRect.w = 128;
            // std::cout<<"projectile"<<std::endl;
        }
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

