#pragma

#include "../Game.hpp"
#include "EntityComponentSystem.hpp"
#include "Components.hpp"

class Controller : public Component{
public:
    TransformComponent* transform;
    
    void init(){
        transform = &entity->getComponent<TransformComponent>();
    }

    void update(){
        if(Game::event.type == SDL_KEYDOWN && Game::event.key.repeat == 0){
            switch(Game::event.key.keysym.sym){
                case SDLK_w:
                    transform->velocity.y = -1;
                    break;
                case SDLK_s:
                    transform->velocity.y = 1;
                    break;
                case SDLK_a:
                    transform->velocity.x = -1;
                    break;
                case SDLK_d:
                    transform->velocity.x = 1;
                    break;
            }
        }
        else if(Game::event.type == SDL_KEYUP && Game::event.key.repeat == 0){
            switch(Game::event.key.keysym.sym){
                case SDLK_w:
                    transform->velocity.y = 0;
                    break;
                case SDLK_s:
                    transform->velocity.y = 0;
                    break;
                case SDLK_a:
                    transform->velocity.x = 0;
                    break;
                case SDLK_d:
                    transform->velocity.x = 0;
                    break;
            }
        }
    }
};