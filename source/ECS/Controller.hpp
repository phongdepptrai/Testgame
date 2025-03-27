#pragma

#include "../Game.hpp"
#include "EntityComponentSystem.hpp"
#include "Components.hpp"

class Controller : public Component{
public:
    TransformComponent* transform;
    SpriteComponent* sprite;
    void init(){
        transform = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();
    }

    void update(){
        if(Game::event.type == SDL_KEYDOWN ){
            switch(Game::event.key.keysym.sym){
                case SDLK_w:
                    transform->velocity.y = -1;
                    sprite->play("Run");
                    break;
                case SDLK_s: 
                    transform->velocity.y = 1;
                    sprite->play("Run");
                    break;
                case SDLK_a:
                    transform->velocity.x = -1;
                    sprite->play("Run");
                    sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
                    break;
                case SDLK_d:
                    transform->velocity.x = 1;
                    sprite->play("Run");
                    break;
            }
        }
        else if(Game::event.type == SDL_KEYUP ){
            switch(Game::event.key.keysym.sym){
                case SDLK_w:
                    transform->velocity.y = 0;
                    sprite->play("Idle");
                    break;
                case SDLK_s:
                    transform->velocity.y = 0;
                    sprite->play("Idle");
                    break;
                case SDLK_a:
                    transform->velocity.x = 0;
                    sprite->play("Idle");
                    sprite->spriteFlip = SDL_FLIP_NONE;
                    break;
                case SDLK_d:
                    transform->velocity.x = 0;
                    sprite->play("Idle");
                    break;
                case SDLK_ESCAPE:
                    Game::isRunning = false;
                    break;
                default:
                    break;
            }
        }
    }
};