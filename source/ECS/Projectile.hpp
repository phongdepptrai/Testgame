#pragma once

#include "EntityComponentSystem.hpp"
#include "Components.hpp"
#include "../TextureManager.hpp"
#include "../Vector.hpp"

class ProjectileComponent: public Component{
public:
    ProjectileComponent(int mrange, int mspeed, Vector2D mvelocity): range(mrange), speed(mspeed), velocity(mvelocity){

    };
    ~ProjectileComponent(){

    };
    void init() override{
        transform = &entity->getComponent<TransformComponent>();
        transform->velocity = velocity;
    }

    void update() override{       
        distance += speed;     
        if(distance > range){
            std::cout<<"Projectile out of range"<<std::endl;
            entity->destroy();
        }   // Check if the projectile is out of the camera
        else if (   transform->position.x > Game::camera.x + Game::camera.w || // right handside
                    transform->position.x < Game::camera.x || //left
                    transform->position.y > Game::camera.y + Game::camera.h ||  // up
                    transform->position.y < Game::camera.y //down
                ){
            std::cout<<"Projectile out of camera"<<std::endl;
            entity->destroy();
        }
            
    }
private:
    TransformComponent* transform;
    int range = 0;
    int speed = 0;
    int distance= 0;
    Vector2D velocity = Vector2D(0,0);
};