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

    TileComponent() = default;
    SDL_Rect tileRect;
    int tileID;

    const char* path;


    TileComponent(int x, int y, int w, int h, int id){
        tileRect.x = x;
        tileRect.y = y;
        tileRect.w = w;
        tileRect.h = h;
        tileID = id;

        switch(tileID){
            case 0:
                path = "assets/MapTexture/Test/dirt.png";
                break;
            case 1:
                path = "assets/MapTexture/Test/grass.png";
                break;
            case 2:
                path = "assets/MapTexture/Test/water.png";
                break;
            default:
                break;
        }
    }

    void init() override{
        entity->addComponent<TransformComponent>(static_cast<float>(tileRect.x), static_cast<float>(tileRect.y), static_cast<float>(tileRect.w), static_cast<float>(tileRect.h), 1);
        transform = &entity->getComponent<TransformComponent>();

        entity->addComponent<SpriteComponent>(path);
        sprite = &entity->getComponent<SpriteComponent>();
    }
        
    





private:

};