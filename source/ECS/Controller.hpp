#pragma

#include "../Game.hpp"
#include "EntityComponentSystem.hpp"
#include "Components.hpp"

class Controller : public Component {
public:
    TransformComponent* transform;
    SpriteComponent* sprite;
    int attackTimer = 0;
    bool isAttacking = false;
    bool isHurt = false;
    void init() {
        transform = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();
    }
    int mouseX = 0, mouseY = 0;
    Uint32 currentTime = 0;
    Uint32 lastAttackTime = 0;
    Uint32 lastHurtTime = 0;
    void update() override {
        currentTime = SDL_GetTicks();

        if(currentTime - lastHurtTime > 500) isHurt = false, lastHurtTime = 0;

        // Mouse click -> attack
        if(currentTime - lastAttackTime > 500) isAttacking = false, lastAttackTime = currentTime;
        if (Game::event.type == SDL_MOUSEBUTTONDOWN && Game::event.button.button == SDL_BUTTON_LEFT) {
            if (!isAttacking) {
                isAttacking = true;
            }
        } 

        // Keyboard
        if (Game::event.type == SDL_KEYDOWN && Game::event.key.repeat == 0) {
            switch (Game::event.key.keysym.sym) {
                case SDLK_w:
                    transform->velocity.y = -1;
                    // sprite->play("Run");
                    break;
                case SDLK_s:
                    transform->velocity.y = 1;
                    // sprite->play("Run");
                    break;
                case SDLK_a:
                    transform->velocity.x = -1;
                    // sprite->play("Run");
                    transform->direction = true;
                    break;
                case SDLK_d:
                    transform->velocity.x = 1;
                    // sprite->play("Run");
                    transform->direction = false;
                    break;
            }
        } else if (Game::event.type == SDL_KEYUP) {
            switch (Game::event.key.keysym.sym) {
                case SDLK_w:
                case SDLK_s:
                    transform->velocity.y = 0;
                    // sprite->play("Idle");
                    break;
                case SDLK_a:

                case SDLK_d:
                    transform->velocity.x = 0;
                    // sprite->play("Idle");
                    break;
                default:
                    break;
            }
        }
        if(isHurt) sprite->play("Hurt");
        else if(isAttacking) sprite->play("Attack");
        else if(transform->velocity.x || transform->velocity.y) sprite->play("Run");
        else sprite->play("Idle");
    }
};