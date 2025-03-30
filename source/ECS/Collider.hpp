#pragma once

#include <string>
#include "SDL2/SDL.h"
#include "Components.hpp"
#include "../TextureManager.hpp"

class ColliderComponent : public Component{
public:
    SDL_Rect collider;
    std::string tag;
    SDL_Rect srcR, destR;

    TransformComponent* transform;
    ColliderComponent(std::string t){
        tag = t;
    }

    ColliderComponent(std::string t, int xpos, int ypos, int size){
        tag = t;
        collider.x = xpos;
        collider.y = ypos;
        collider.h = collider.w = size;
    }
    void init() override{
        if(!entity->hasComponent<TransformComponent>()){
            entity->addComponent<TransformComponent>();
        }
        transform = &entity->getComponent<TransformComponent>();
        srcR = {0, 0, 32, 32};
        destR = {collider.x, collider.y, collider.w, collider.h};
    }

    void update() override{
        if (tag != "terrain"){
            collider.x = static_cast<int>(transform->position.x);
            collider.y = static_cast<int>(transform->position.y);
            collider.w = transform->width * transform->scale;
            collider.h = transform->height * transform->scale;
        }
        if (tag == "player"){
            collider.x = static_cast<int>(transform->position.x + 39 * transform->scale);
            collider.y = static_cast<int>(transform->position.y + 58 * transform->scale);
            collider.w = (transform->width - 71) * transform->scale;
            collider.h = (transform->height - 72) * transform->scale;
        }
        if(tag == "yellowGolem" || tag == "blueGolem"){
            collider.x = static_cast<int>(transform->position.x + 29 * transform->scale);
            collider.y = static_cast<int>(transform->position.y + 34 * transform->scale);
            collider.w = (transform->width - 60) * transform->scale;
            collider.h = (transform->height - 34) * transform->scale;
        }
        if( tag == "Player_projectile"){
            collider.x = static_cast<int>(transform->position.x + 92 * transform->scale);
            collider.y = static_cast<int>(transform->position.y + 84 * transform->scale);
            collider.w = (transform->width - 168) * transform->scale;
            collider.h = (transform->height - 168) * transform->scale;
        }
        destR.x = collider.x - Game::camera.x;
        destR.y = collider.y - Game::camera.y;
    }
    
    // void draw() override{
    //     TextureManager::Draw(tex, srcR, destR, SDL_FLIP_NONE);
    // }
};