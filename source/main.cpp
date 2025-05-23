#include <iostream>
#include "Game.hpp"

Game* game = nullptr;
int main(int argc, char* argv[]){
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;
    game = new Game();
    bool fullscreen = false;
    
    game->init("Ohio Survival", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1536, 1024, false);

    while (game->running()){
        frameStart = SDL_GetTicks();

        game->handleEvents();
        game->update();
        game->render();
        
        frameTime = SDL_GetTicks() - frameStart;
        
        if(frameDelay > frameTime){
            SDL_Delay(frameDelay - frameTime);
        }
    }
    game->clean();

    return 0;
}